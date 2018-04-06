#include "raytracer.h"
#include "math.h"

#include <QThread>
#include <QProgressBar>

RayTracer::RayTracer(Scene *scene)
    :   Renderer(scene)
{
    mode = Renderer::Raytracer;
}

QImage RayTracer::generate(QProgressBar *progress, QImage image)
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

            Color color = trace(ray, 0);
            image.setPixelColor(x, y, color.asQColor ());
            //progress->setValue(progress->value() + 1); //update progress bar
        }
    }
    return image;
}

Color RayTracer::trace(Ray ray, int depth) {
    Vector3D col_tot = Vector3D(0,0,0);
    float t0_0 = 0.001;
    float t1_0 = 20000;
    Intersection intersection = findIntersection (ray, t0_0, t1_0);

    if(intersection.didHit()) {
        //material of intersected face
        Material m = scene->model.materials.value(intersection.material);
        Vector3D c = scene->ambient_intensity * m.ambient;

        //SECOND TRACE, TBD TBD, FOR EACH LIGHT AND SO ON
        for(Light light : scene->lights) {
            Ray ray_towards_light = Ray(intersection.point, light.position - intersection.point);
            float t0_1 = 0.001;
            float t1_1 = 20000;
            Intersection intersection2 = findIntersection (ray_towards_light, t0_1, t1_1);

            if(!intersection2.didHit()) {
                float diffuse, specular;

                //phong
                //diffuse
                diffuse = Vector3D::dot_prod (ray_towards_light.direction.normalized (), intersection.normal);
                diffuse = fmax(0, diffuse);
                Vector3D diff_tot =  diffuse * m.diffuse;

//                //specular
                Vector3D bisector = (-1*ray.direction.normalized ()+ ray_towards_light.direction.normalized ()).normalized();
                specular = Vector3D::dot_prod (bisector, intersection.normal);
                specular = powf(fmax(0, specular), m.spec_exp);
                Vector3D spec_tot =  specular * m.specular;

                c = c + light.intensity*light.color.asVector3D ()*diff_tot + light.intensity *light.color.asVector3D ()*spec_tot;
            }
        }
        if(m.illModel.reflection) {
            if(depth > 0) {
                c = c + 0.2*trace(Ray(intersection.point, Vector3D::reflect (ray.direction, intersection.normal).normalized ()), depth - 1).asVector3D ();
            }
        }
        return Color(c);
    }
    else {
        return Color(scene->ambient_intensity*scene->ambient_color.asVector3D ());
    }
}

Intersection RayTracer::findIntersection (Ray ray, float &t0, float &t1) {
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

