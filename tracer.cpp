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
    float width = scene->camera.imageWidth;
    float height = scene->camera.imageHeight;
    float fov = scene->camera.angleOfView;
    float imageAspectRatio = width / height;
    float scale = tanf(fov * 0.5 * M_PI/180);
    float focalLength = (width /2)/tanf((M_PI/4));

    Matrix4x4 camToWorld = scene->camera.camToWorld;
    Vector3D rayOrigin = camToWorld*Vector3D(0,0,0);
//    qDebug() << rayOrigin;
//    Vector3D rayOriginWorld = camToWorld*rayOrigin;
//    qDebug() << rayOriginWorld;

    // Iterate over all pixels in image
    for (int x = 0; x < image.width (); x++) {
        for (int y = 0; y < image.height (); y++) {
            Vector3D col(0,0,0);

            float px = (2 * ((x + image.offset().x() + 0.5) / width) - 1) *imageAspectRatio * scale;
            float py = (1 - 2*((y + image.offset().y() + 0.5) / height)) * scale;

//            float px = (2 * ((x + image.offset().x() + 0.5) / width) - 1) * tanf(fov/2 * M_PI/180)*imageAspectRatio * scale;
//            float px = (2 * ((x + image.offset().x() + 0.5) / width) - 1) * tanf(fov/2 * M_PI/180)*imageAspectRatio * scale;

            Vector3D dir = (camToWorld*Vector3D(px, py, -1) - rayOrigin).normalized();
//            qDebug() << dir;

//            Vector3D rayPWorld = camToWorld*(Vector3D(px, py, -1));
//            Vector3D rayDirection = rayPWorld - rayOriginWorld;
//            Vector3D rayDirection = Vector3D(0,0,0) - rayOriginWorld;
//            rayDirection = rayDirection.normalized(); // it's a direction so don't forget to normalize
//            qDebug () << rayOriginWorld;
//            qDebug() << rayDirection;
            Ray ray;
//            if(scene->camera.mode == Camera::ortographic) {
//                Vector3D worldPos = screenToWorldCoordinates (Vector3D(x + image.offset ().x (), y + image.offset ().y (), 0));
//            }
//            else if(scene->camera.mode == Camera::perspective) {
//                Vector3D worldPos = screenToWorldCoordinates (Vector3D(x + image.offset ().x (), y+ image.offset ().y (), 0));
//                ray = Ray(scene->camera.position, worldPos - scene->camera.position);
//            }

            ray = Ray(rayOrigin, dir);


            //  qDebug() << x << ", " << y;
//            qDebug() << ray.origin << ", " << ray.direction;
            col = trace(ray, depth).asVector3D ();

            image.setPixelColor(x, y, Color(col).asQColor ());

            //            Color color = trace(ray, depth);
            //            image.setPixelColor(x, y, color.asQColor ());
        }
    }
    return image;
}

Color Tracer::trace(Ray ray, int depth) {
    return Color(0,0,0);
}

