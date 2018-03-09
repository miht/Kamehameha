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

void RayTracer::trace(QTextStream &out, QProgressBar *progress, QPixmap &pixmap)
{
    // Plain PPM format
    out << "P3\n" << w << ' ' << h << ' ' << "255\n";

    //load model here ? ugly yes, working yes
    //this should be done elsewhere, too heavy to load model each trace attempt
    Model3D mod = ObjParser::parse("/Users/leifthysellsundqvist/Library/Mobile Documents/com~apple~CloudDocs/Skolan/Y4/VisGraf18/ProjectTest/cube");

    //initialize the pixmap, draw a simple grey background
    pixmap = QPixmap(w, h);
    QPainter pixpaint(&pixmap);
    pixpaint.setPen (Qt::gray);
    pixpaint.drawRect(0,0,w, h);

    // Iterate over all pixels in image
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {

            //Color pixelColor = Color();

            // TODO: Decide color of pixel

//            out << (int) pixelColor.r << ' ' <<
//                   (int) pixelColor.g << ' ' <<
//                   (int) pixelColor.b << '\n';


            QString set_material = "None";
            double shortest = 20000;

            Vector3D worldPos = screenToWorldCoordinates (Vector3D(x, y, 0));
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
                Vector3D col = 255*mod.materials.value (set_material).diffuse;
                QColor color = QColor::fromRgb (col.x, col.y, col.z);
                pixpaint.setPen(color);
//                qDebug() << col;
            }
            else {
                pixpaint.setPen(Qt::white);
            }
            pixpaint.drawPoint (x, y);                  //draw the pixel
            progress->setValue(progress->value() + 1); //update progress bar
        }
    }
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

