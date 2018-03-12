#include "raytracer.h"

#include <QProgressBar>

RayTracer::RayTracer(Scene &scene)
    :   Renderer(scene)
{
}

RayTracer::RayTracer(int w, int h, Scene &scene)
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
    Color ret = Color(0,0,0);
    Intersection intersection = findIntersection (ray);

    if(intersection.didHit()) {
        Material m = scene.model.materials.value(intersection.material);
        ret = Color(255*m.diffuse.x, 255*m.diffuse.y, 255*m.diffuse.z);

        //SECOND TRACE, TBD TBD, FOR EACH LIGHT AND SO ON
        for(Light light : scene.lights) {
            Vector3D hitPoint = intersection.point + 0.00001*intersection.normal;
            Ray ray_towards_light = Ray(hitPoint, light.position - hitPoint);
            float dist2 = 20000;
            Intersection intersect2;
            for(Shape shape_shadow : scene.model.shapes) {
                if(shape_shadow.intersects (ray_towards_light,
                                          dist2, intersect2)) { // check dist2, if 0 then it collids with its own collision point
                    Material m2 = scene.model.materials.value (intersect2.material);
                    //ret = Color(255*m2.diffuse.x, 255*m2.diffuse.y, 255*m2.diffuse.z);
                    ret.r *= 0.5;
                    ret.g *= 0.5;
                    ret.b *= 0.5;
                    break;
                }
            }
        }

    }
    return ret;
}

Intersection RayTracer::findIntersection (Ray ray) {
    Intersection final_intersection;
    float shortest = 20000;

    for(Shape shape : scene.model.shapes) {
        float dist = 20000;
        Intersection intersect1;
        if(shape.intersects (ray, dist, intersect1)) {
            if(dist < shortest) {
                final_intersection = intersect1;
                shortest = dist;
            }
        }
    }

    return final_intersection;
}

