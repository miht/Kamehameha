#include "raytracer.h"
#include "math.h"

#include <QThread>
#include <QProgressBar>

RayTracer::RayTracer(Scene *scene)
    :   Renderer(scene)
{
}

RayTracer::RayTracer(int w, int h, Scene *scene)
    :   Renderer(w, h, scene)
{
}

QImage RayTracer::generate(QProgressBar *progress, int xOffset, int yOffset, int width, int height)
{
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";
    QImage img = QImage(width, height, QImage::Format_RGB32);
    img.setOffset (QPoint(xOffset, yOffset));

    // Iterate over all pixels in image
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Vector3D worldPos = screenToWorldCoordinates (Vector3D(x + xOffset, y + yOffset, 0));
            Ray ray(worldPos, Vector3D(0, 0, 1));
            Color color = trace(ray, 0);

            img.setPixelColor(x, y, color.asQColor ());
            //progress->setValue(progress->value() + 1); //update progress bar
        }
    }
    return img;
}

Color RayTracer::trace(Ray ray, int depth) {
    Vector3D col_tot = Vector3D(0,0,0);
    float t0_0 = 0;
    float t1_0 = 20000;
    Intersection intersection = findIntersection (ray, t0_0, t1_0);

    if(intersection.didHit()) {
        //material of intersected face
        Material m = scene->model.materials.value(intersection.material);
        Vector3D c = 255 * scene->ambient_intensity * m.ambient;

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
                //diff_tot = (1.0/(t1_1*t1_1)) * diff_tot; //ATTENUATION FOR POINT LIGHT DISTANCE

//                //specular
                Vector3D bisector = (-1*ray.direction.normalized ()+ ray_towards_light.direction.normalized ()).normalized();
                specular = Vector3D::dot_prod (bisector, intersection.normal);
                specular = std::powf(fmax(0, specular), m.spec_exp);
                Vector3D spec_tot =  specular * m.specular;

                c = c + 255*light.intensity* diff_tot + 255* light.intensity * spec_tot;
            }
        }
        return Color(c);
    }
    else {
        return scene->ambient_color;
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

