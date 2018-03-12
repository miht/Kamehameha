#include "wireframer.h"

Wireframer::Wireframer(Scene &scene)
    :   Renderer(scene)
{
}

Wireframer::Wireframer(int w, int h, Scene &scene)
    :   Renderer(w, h, scene)
{

}

QImage Wireframer::generate(int width, int height) {
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";
    QImage img = QImage(width, height, QImage::Format_RGB32);

    // Iterate over all pixels in image
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {


            //progress->setValue(progress->value() + 1); //update progress bar
        }
    }
    return img;
}
