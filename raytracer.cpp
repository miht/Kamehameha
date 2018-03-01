#include "raytracer.h"
#include "color.h"

#include <QProgressBar>

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

RayTracer::RayTracer()
{
    w = DEFAULT_WIDTH;
    h = DEFAULT_HEIGHT;
}

RayTracer::RayTracer(int w, int h) :
    w(w), h(h)
{

}

void RayTracer::trace(QTextStream &out, QProgressBar *progress)
{
    // Plain PPM format
    out << "P3\n" << w << ' ' << h << ' ' << "255\n";

    // Iterate over all pixels in image
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Color pixelColor = Color();

            // TODO: Decide color of pixel

            out << (int) pixelColor.r << ' ' <<
                   (int) pixelColor.g << ' ' <<
                   (int) pixelColor.b << '\n';

            progress->setValue(progress->value() + 1);
        }
    }
}
