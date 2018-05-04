#include "pathtracer.h"

#include <random>
#include <QThread>
#include <QProgressBar>

PathTracer::PathTracer(Scene *scene)
    :   Tracer(scene)
{
    mode = Tracer::Pathtracer;
}

std::default_random_engine generator_path;
std::uniform_real_distribution<float> distribution_path(0, 1);

Color PathTracer::trace(Ray ray, int depth) {
    if (depth < 0) {
//        return Color(Vector3D(1,1,1));  // Bounced enough times, return background
//        return Color(scene->ambient_intensity*scene->ambient_color.asVector3D ());
        return Color(0,0,0);
    }

    float t0_0 = 0.001;
    float t1_0 = 20000;
    Intersection intersection;
    if(scene->model.root->hit(ray, t0_0, t1_0, intersection)) {
        Vector3D normal = intersection.normal;
        //material of intersected face
        Material m = scene->model.materials.value(intersection.material);
        Vector3D c = scene->ambient_intensity*m.diffuse;

        // compute direct illumination
        Vector3D cDir, cDiff, cSpec;
        for (Light light : scene->lights) {
            Ray ray_towards_light = Ray(intersection.point, light.position - intersection.point);
            float t0_1 = 0.001;
            float t1_1 = ray_towards_light.direction.norm ();
            Intersection intersection2;

            if(!scene->model.root->hit(ray_towards_light, t0_1, t1_1, intersection2)) {
                //diffuse
                float diffuse = Vector3D::dot_prod (ray_towards_light.direction.normalized (), intersection.normal);
                diffuse = fmax(0, diffuse);

                cDiff = cDiff + light.intensity*diffuse*light.color.asVector3D ();

//                //specular
                Vector3D bisector = (-1*ray.direction.normalized ()+ ray_towards_light.direction.normalized ()).normalized();
                float specular = Vector3D::dot_prod (bisector, intersection.normal);
                specular = std::powf(fmax(0, specular), m.spec_exp);
                cSpec = cSpec + light.intensity*specular*light.color.asVector3D ();
            }
        }
        cDir = cDiff + cSpec;

        Vector3D cIndir;
        if(globalIllumination) {
            uint32_t N = samples / (depth + 1);
            Vector3D Nt, Nb;
            createCoordinateSystem(normal, Nt, Nb);
            float pdf = 1 / (2 * M_PI);
            for (uint32_t n = 0; n < N; ++n) {
                float r1 = distribution_path(generator_path);
                float r2 = distribution_path(generator_path);
                Vector3D sample = uniformSampleHemisphere(r1, r2);
                Vector3D sampleWorld(
                            sample.x * Nb.x + sample.y * normal.x + sample.z * Nt.x,
                            sample.x * Nb.y + sample.y * normal.y + sample.z * Nt.y,
                            sample.x * Nb.z + sample.y * normal.z + sample.z * Nt.z);
                // don't forget to divide by PDF and multiply by cos(theta)
                cIndir = cIndir + (r1/pdf)*trace(Ray(intersection.point, sampleWorld), depth - 1).asVector3D ();
            }
            // divide by N
            cIndir = cIndir / ((float) N);
        }
//        c = c + cDiff * m.diffuse + cSpec*m.specular;

        c = (cDir/M_PI + 2*cIndir)*m.specular*m.diffuse + m.emissive;
        return Color(c);
    }
    else {
        return Color(scene->ambient_intensity*scene->ambient_color.asVector3D ());
    }
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

//Intersection PathTracer::findIntersection (Ray ray, float &t0, float &t1) {
//    Intersection final_intersection;
//    float shortest = 20000;

//    for(Object object : scene->model.objects) {
//        if(object.bbox.intersects (ray, t0, t1)) { // REPLACE WITH IF OBJECT.BOUNDING BOX INTERSECTS SOME RAY!!!!!
//            for(Face* face : object.faces) {
//                float t1_n = 20000;
//                Intersection intersection;
//                if(face->intersects (ray, t0, t1_n, intersection, true)) {
//                    if(t1_n < shortest) {
//                        final_intersection = intersection;
//                        shortest = t1_n;
//                    }
//                }
//            }
//        }
//    }

//    t1 = shortest;
//    return final_intersection;
//}

