#include "renderer.h"

Renderer::Renderer(Scene *scene)
    :   scene(scene)
{
    w = DEFAULT_WIDTH;
    h = DEFAULT_HEIGHT;
}

Renderer::Renderer(int width, int height, Scene *scene)
    :   w(width), h(height), scene(scene)
{
}

void Renderer::setScene(Scene *scene) {
    this->scene = scene;
}

QImage Renderer::generate(QProgressBar *progress, int xOffset, int yOffset, int width, int height) {
    return QImage(width,height, QImage::Format_RGB32);
}

QPointF convertToQPointF(Vector3D point) {
    return QPointF(point.x, point.y);
}

Vector3D Renderer::screenToWorldCoordinates(const Vector3D vec) {
    float x = (vec.x - ((float) w) / 2)/w;
    float y = (vec.y - ((float) h) / 2)/h;
    float z = 0; //z = 0 ???

    return Vector3D(x,y,z);
}

Vector3D Renderer::worldToScreenCoordinates(const Vector3D vec) {
    float x = vec.x * w + ((float) w)/2;
    float y = vec.y * h + ((float) h)/2;
    float z = 0;  //z = 0 ???

    return Vector3D(x,y,z);
}
