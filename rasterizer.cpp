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

    float width = (float) image.width ();
    float height = (float) image.height ();

    float canvasWidth = width/height, canvasHeight = 1;

    Matrix4x4 worldToCam;
    bool inverted = Matrix4x4::inverse(scene->camera.camToWorld, worldToCam);
    // Iterate over all pixels in image
    for(Face* face : scene->model.root->faces) {
        std::vector<Vector2D> points;

        std::vector<Vector2D> leftPixels;
        std::vector<Vector2D> rightPixels;

        int vertices = face->getPoints ().size();
        for(size_t i = 0; i < vertices; i++) {
            Vector3D pWorld = face->getPoints()[i];
            Vector3D pCamera = worldToCam*pWorld;
            Vector2D pScreen(pCamera.x / -pCamera.z, pCamera.y / -pCamera.z);

//            if (std::abs(pScreen.x) <= canvasWidth || std::abs(pScreen.y) <= canvasHeight) {
                Vector2D pNDC((pScreen.x + canvasWidth * 0.5) / canvasWidth, (pScreen.y + canvasHeight * 0.5) / canvasHeight);

                int pX = (int)(pNDC.x * width);
                int pY = (int)((1 - pNDC.y) * height);

                points.push_back (Vector2D(pX, pY));
//            }
        }

        computePolygonRows (points, leftPixels, rightPixels);
        interpolate (points[0], points[1], leftPixels);
        interpolate(points[0], points[2], rightPixels);

        Color color = scene->model.materials.value (face->material).diffuse;
        painter.setPen(color.asQColor());

        drawPolygon(painter, leftPixels, rightPixels);
    }

    painter.end ();
    return image;
}

void Rasterizer::computePolygonRows(const std::vector<Vector2D>& vertexPixels, std::vector<Vector2D>& leftPixels, std::vector<Vector2D>& rightPixels) {
    int maxY, minY;
    for(int i = 0; i < vertexPixels.size (); i++) {
        Vector2D p = vertexPixels[i];
        if(fabs(p.y) > maxY)
            maxY = p.y;
        if(fabs(p.y) < minY)
            minY = p.y;
    }

    int size = (int) fabs(maxY - minY);

    leftPixels.resize (size);
    rightPixels.resize (size);

    for(int i = 0; i < size; i++) {
        rightPixels[i].y = i + maxY;
        leftPixels[i].y = i + maxY;
        leftPixels[i].x = INT_MAX;
        rightPixels[i].x = INT_MIN;
    }
}

void Rasterizer::drawPolygon (QPainter &painter, std::vector<Vector2D> &leftPixels, std::vector<Vector2D> &rightPixels) {
    for(int i = 0; i < leftPixels.size(); i++) {
        Vector2D p1 = leftPixels[i], p2 = rightPixels[i];
        qDebug() << p1 << ", " << p2;
        painter.drawLine(p1.x, p1.y, p2.x, p2.y);
    }
}

