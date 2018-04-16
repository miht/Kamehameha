#include "tracer.h"

Tracer::Tracer(Scene *scene)
    :   Renderer(scene)
{

}

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-1, 1);

QImage Tracer::generate(QProgressBar *progress, QImage image)
{
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";
    // Iterate over all pixels in image
    for (int x = 0; x < image.width (); x++) {
        for (int y = 0; y < image.height (); y++) {
            Vector3D col(0,0,0);
            //Test antialiasing here!!!
            if(antiAliasing) {
                int stochasticSamplingNumber = 4;
                for(int i = 0; i < stochasticSamplingNumber; i++) {
                    float r1 = distribution(generator);
                    float r2 = distribution(generator);

                    Ray ray;
                    if(scene->camera.mode == Camera::ortographic) {
                        Vector3D worldPos = screenToWorldCoordinates (Vector3D(x + r1 + image.offset ().x (), y + r2 + image.offset ().y (), 0));
                        ray = Ray(worldPos, scene->camera.forward);
                    }
                    else if(scene->camera.mode == Camera::perspective) {
                        Vector3D worldPos = screenToWorldCoordinates (Vector3D(x + r1 + image.offset ().x (), y + r2 + image.offset ().y (), scene->camera.position.z + scene->camera.depth));
                        ray = Ray(scene->camera.position, worldPos - scene->camera.position);
                    }
                    col = col + trace(ray, depth).asVector3D ()/stochasticSamplingNumber;
                }
            }
            else {

                //TODO CLEAN UP REDUNDANT CODE REPETITION
                Ray ray;
                if(scene->camera.mode == Camera::ortographic) {
                    Vector3D worldPos = screenToWorldCoordinates (Vector3D(x + image.offset ().x (), y + image.offset ().y (), 0));
                    ray = Ray(worldPos, scene->camera.forward);
                }
                else if(scene->camera.mode == Camera::perspective) {
                    Vector3D worldPos = screenToWorldCoordinates (Vector3D(x + image.offset ().x (), y+ image.offset ().y (), scene->camera.position.z + scene->camera.depth));
                    ray = Ray(scene->camera.position, worldPos - scene->camera.position);
                }
                col = trace(ray, depth).asVector3D ();
            }

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

