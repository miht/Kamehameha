#include "tracer.h"

Tracer::Tracer(Scene *scene)
    :   Renderer(scene)
{

}

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-1, 1);

QImage Tracer::generate(QProgressBar *progress, QImage image)
{
    //Calculate screen coordinates here
    float width = (float) scene->camera.imageWidth;
    float height = (float) scene->camera.imageHeight;
    float fov = scene->camera.angleOfView;
    fov = 45;
    float imageAspectRatio = width / height;
    float scale = tanf((fov * 0.5 * M_PI)/180.0);
    float focalLength = scene->camera.focalLength;

    Matrix4x4 camToWorld = scene->camera.camToWorld;
    Vector3D rayOrigin = camToWorld*Vector3D(0,0,0);

    // Iterate over all pixels in image
    for (int x = 0; x < image.width (); x++) {
        for (int y = 0; y < image.height (); y++) {
            Vector3D col(0,0,0);

            float px = (2 * (x + image.offset().x() + 0.5) / width - 1) *imageAspectRatio * scale;
            float py = (1 - 2*(y + image.offset().y() + 0.5) / height) * scale;
            Vector3D dir = (camToWorld*Vector3D(px, py, -1) - rayOrigin).normalized();

//            Vector3D dir((x + image.offset ().x ()) - width/2, (y + image.offset ().y()) - height/2, focalLength);
//            dir = (camToWorld*dir).normalized();

            Ray ray;

            ray = Ray(rayOrigin, dir);

            col = trace(ray, depth).asVector3D ();

            image.setPixelColor(x, y, Color(col).asQColor ());

        }
    }
    return image;
}

Color Tracer::trace(Ray ray, int depth) {
    return Color(0,0,0);
}

