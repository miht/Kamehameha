#include "raytracer.h"

#include <QThread>
#include <QProgressBar>

RayTracer::RayTracer(Scene *scene)
    :   Tracer(scene)
{
    mode = Tracer::Raytracer;
}

Color RayTracer::trace(Ray ray, int depth) {
    if(depth < 0) {
//        return Color(scene->ambient_intensity*scene->ambient_color.asVector3D ());
        return Color(0,0,0);
    }
    float t0_0 = 0.000001;
    float t1_0 = 20000;
//    Intersection intersection = findIntersection (ray, t0_0, t1_0);
    Intersection intersection;
    if(scene->model.root->hit(ray, t0_0, t1_0, intersection)) {

//    if(intersection.didHit()) {
        Vector3D normal = intersection.normal;
        //material of intersected face
        Material m = scene->model.materials.value(intersection.material);
        Vector3D c = scene->ambient_intensity*m.diffuse;

        // compute lambertian shading and phong shading
        Vector3D cDiff, cSpec;
        for(Light light : scene->lights) {
            Ray ray_towards_light = Ray(intersection.point, light.position - intersection.point);
            float t0_1 = 0.000001;
            float t1_1 = 20000;

            Intersection intersection2;
            if(!scene->model.root->hit(ray_towards_light, t0_1, t1_1, intersection2)) {
                //diffuse
                float diffuse = Vector3D::dot_prod (ray_towards_light.direction.normalized (), normal);
                diffuse = fmax(0, diffuse);
                cDiff = cDiff + light.intensity*diffuse*light.color.asVector3D ();

                //specular
                Vector3D bisector = (-1*ray.direction.normalized ()+ ray_towards_light.direction.normalized ()).normalized();
                float specular = Vector3D::dot_prod (bisector, normal);
                specular = std::powf(fmax(0, specular), m.spec_exp);
                cSpec = cSpec + light.intensity*specular*light.color.asVector3D ();
            }
        }
        Vector3D refl;
        if(m.illModel.reflection) {
            refl = 0.5*trace(Ray(intersection.point, Vector3D::reflect (ray.direction, normal).normalized ()), depth - 1).asVector3D ();
        }

        c = cDiff * m.diffuse + cSpec*m.specular + refl;
        return Color(c);
    }
    else {
        return Color(scene->ambient_intensity*scene->ambient_color.asVector3D ());
    }
}

