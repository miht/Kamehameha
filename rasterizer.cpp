#include "rasterizer.h"

Rasterizer::Rasterizer(Scene *scene)
    :   Renderer(scene)
{
    mode = Renderer::Rasterizer;
}

QImage Rasterizer::generate(QProgressBar *progress, QImage image) {
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";
    QPainter painter (&image);
    painter.fillRect (0,0,image.width (), image.height (), Qt::black);
    painter.setPen (Qt::white);

    int zDepthSize = scene->camera.viewportWidth*scene->camera.viewportHeight;
    float depthBuffer[zDepthSize];
    std::fill_n(depthBuffer, zDepthSize, (float) INT_MAX);

    // Iterate over all pixels in image
    for(Face* face : scene->model.root->faces) {
        Color color = scene->model.materials.value (face->material).diffuse;
        painter.setPen(color.asQColor());

        std::vector<Vector3D> points = face->getPoints ();
        Vector2D v1, v2, v3;
        v1 = worldToViewport (points[0]);
        v2 = worldToViewport (points[1]);
        v3 = worldToViewport (points[2]);

        //calculate bounding box
        Vector3D max, min;
        max.x = fmax(v1.x, fmax(v2.x, v3.x));
        max.y = fmax(v1.y, fmax(v2.y, v3.y));

        min.x = fmin(v1.x, fmin(v2.x, v3.x));
        min.y = fmin(v1.y, fmin(v2.y, v3.y));

        //iterate over pixels of bounding box
        for(int i = min.x; i < max.x; i++) {
            for(int j = min.y; j < max.y; j++) {
                if(Triangle::contains (v1, v2, v3, Vector2D(i, j))) {
//                    float depth = Vector3D::interpolate(points[0], Vector3D::interpolate(points[1], points[2])).z;

                    painter.drawPoint(i, j);
                }
            }
        }
    }

    painter.end ();
    return image;
}
