#include "raytracer.h"
#include "color.h"

#include <QProgressBar>

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;

RayTracer::RayTracer()
{
    w = DEFAULT_WIDTH;
    h = DEFAULT_HEIGHT;

    mod = ObjParser::parse("/Users/leifthysellsundqvist/Library/Mobile Documents/com~apple~CloudDocs/Skolan/Y4/VisGraf18/ProjectTest/well_raytracer");
}

RayTracer::RayTracer(int w, int h) :
    w(w), h(h)
{
    //load model here ? ugly yes, working yes
    //this should be done elsewhere, too heavy to load model each trace attempt
    mod = ObjParser::parse("/Users/leifthysellsundqvist/Library/Mobile Documents/com~apple~CloudDocs/Skolan/Y4/VisGraf18/ProjectTest/well_raytracer");

}

QImage RayTracer::trace(QProgressBar *progress, int xOffset, int yOffset, int width, int height)
{
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";
    QImage img = QImage(width, height, QImage::Format_RGB32);
    img.setOffset (QPoint(xOffset, yOffset));

    // Iterate over all pixels in image
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            QColor color;

            QString set_material = "None";
            double shortest = 20000;

            Vector3D worldPos = screenToWorldCoordinates (Vector3D(x + xOffset, y + yOffset, 0));
            Ray ray(worldPos, Vector3D(0, 0, 1));
            for(Triangle tri : mod.triangles) {
                double dist = 20000;
                QString material = "None";
                if(tri.intersects (ray, dist, material)) {
                    //qDebug() << material;
                    if(dist < shortest) {
                        shortest = dist;
                        set_material = material;
                    }
                }
            }

            if(set_material != "None") {
                Vector3D col = mod.materials.value (set_material).diffuse;
                color = QColor(255*col.x, 255*col.y, 255*col.z);
            }
            else {
                color = QColor(0, 0, 0); //black
            }
            img.setPixelColor(x, y, color);
            //progress->setValue(progress->value() + 1); //update progress bar
        }
    }
    return img;
}

Vector3D RayTracer::screenToWorldCoordinates(const Vector3D vec) {
    float x = (vec.x - ((float) w) / 2)/w;
    float y = (vec.y - ((float) h) / 2)/h;
    float z = 0; //z = 0 ???

    return Vector3D(x,y,z);
}

Vector3D RayTracer::worldToScreenCoordinates(const Vector3D vec) {
    float x = vec.x * w + ((float) w)/2;
    float y = vec.y * h + ((float) h)/2;
    float z = 0;  //z = 0 ???

    return Vector3D(x,y,z);
}

