#include "pathtracer.h"
#include "math.h"

#include <random>
#include <QThread>
#include <QProgressBar>

PathTracer::PathTracer(Scene *scene)
    :   Renderer(scene)
{
    mode = Renderer::Pathtracer;
}

QImage PathTracer::generate(QProgressBar *progress, QImage image)
{
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";
    // Iterate over all pixels in image
    for (int x = 0; x < image.width (); x++) {
        for (int y = 0; y < image.height (); y++) {
            Ray ray;
            if(scene->camera.mode == Camera::ortographic) {
                Vector3D worldPos = screenToWorldCoordinates (Vector3D(x + image.offset ().x (), y + image.offset ().y (), 0));
                ray = Ray(worldPos, Vector3D(0, 0, 1));
            }
            else if(scene->camera.mode == Camera::perspective) {
                Vector3D worldPos = screenToWorldCoordinates (Vector3D(x + image.offset ().x (), y + image.offset ().y (), scene->camera.position.z + scene->camera.depth));
                ray = Ray(scene->camera.position, worldPos - scene->camera.position);
            }
            Color color = trace(ray, depth);

            image.setPixelColor(x, y, color.asQColor ());
        }
    }
    return image;
}

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0, 1);

Color PathTracer::trace(Ray ray, int depth) {
    if (depth <= 0) {
//        return Color(Vector3D(1,1,1));  // Bounced enough times, return background
        return scene->ambient_color;
    }

     Vector3D hitColor =  Vector3D(0,0,0);

    float t0_0 = 0.001;
    float t1_0 = 20000;
    Intersection intersection = findIntersection (ray, t0_0, t1_0);

    //material of intersected face
    Material m = scene->model.materials.value(intersection.material);

    if(intersection.didHit()) {
        Vector3D normal = intersection.normal;

        Vector3D directLightContrib, indirectLightContrib;
        // compute direct illumination
        for (Light light : scene->lights) {
            Ray ray_towards_light = Ray(intersection.point, light.position - intersection.point);
            float t0_1 = 0.001;
            float t1_1 = 20000;
            Intersection intersection2 = findIntersection (ray_towards_light, t0_1, t1_1);

            if(!intersection2.didHit()) {
                float diff_tot = fmax(0, Vector3D::dot_prod (ray_towards_light.direction.normalized (), normal));
                directLightContrib = directLightContrib +
                        diff_tot*light.intensity*light.color.asVector3D ();
            }
        }

        if(globalIllumination) {
            uint32_t N = 32;// / (depth + 1);
            Vector3D Nt, Nb;
            createCoordinateSystem(normal, Nt, Nb);
            float pdf = 1 / (2 * M_PI);
            for (uint32_t n = 0; n < N; ++n) {
                float r1 = distribution(generator);
                float r2 = distribution(generator);
                Vector3D sample = uniformSampleHemisphere(r1, r2);
                Vector3D sampleWorld(
                            sample.x * Nb.x + sample.y * normal.x + sample.z * Nt.x,
                            sample.x * Nb.y + sample.y * normal.y + sample.z * Nt.y,
                            sample.x * Nb.z + sample.y * normal.z + sample.z * Nt.z);
                // don't forget to divide by PDF and multiply by cos(theta)
                indirectLightContrib = indirectLightContrib + r1 * (1/pdf)*trace(Ray(intersection.point + sampleWorld, sampleWorld), depth - 1).asVector3D ();
            }
    //        qDebug() << indirectLightContrib;
            // divide by N
            indirectLightContrib = indirectLightContrib / ((float) N);
        }

        hitColor = (directLightContrib/M_PI + 2 * indirectLightContrib)*m.diffuse;
    }
    else {
        return Color(scene->ambient_intensity*scene->ambient_color.asVector3D ());
    }
    return hitColor;
}

void PathTracer::createCoordinateSystem(const Vector3D &N, Vector3D &Nt, Vector3D &Nb)
{
    if (std::fabs(N.x) > std::fabs(N.y))
        Nt = Vector3D(N.z, 0, -N.x) / sqrtf(N.x * N.x + N.z * N.z);
    else
        Nt = Vector3D(0, -N.z, N.y) / sqrtf(N.y * N.y + N.z * N.z);
    Nb = Vector3D::cross_prod (N, Nt);
}

Vector3D PathTracer::uniformSampleHemisphere(const float &r1, const float &r2)
{
    // cos(theta) = u1 = y
    // cos^2(theta) + sin^2(theta) = 1 -> sin(theta) = srtf(1 - cos^2(theta))
    float sinTheta = sqrtf(1 - r1 * r1);
    float phi = 2 * M_PI * r2;
    float x = sinTheta * cosf(phi);
    float z = sinTheta * sinf(phi);
    return Vector3D(x, r1, z);
}

Intersection PathTracer::findIntersection (Ray ray, float &t0, float &t1) {
    Intersection final_intersection;
    float shortest = 20000;

    for(Object object : scene->model.objects) {
        if(object.bbox.intersects (ray, t0, t1)) { // REPLACE WITH IF OBJECT.BOUNDING BOX INTERSECTS SOME RAY!!!!!
            for(Face* face : object.faces) {
                float t1_n = 20000;
                Intersection intersection;
                if(face->intersects (ray, t0, t1_n, intersection, true)) {
                    if(t1_n < shortest) {
                        final_intersection = intersection;
                        shortest = t1_n;
                    }
                }
            }
        }
    }

    t1 = shortest;
    return final_intersection;
}

