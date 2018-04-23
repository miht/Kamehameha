#include "renderer.h"

Renderer::Renderer(Scene *scene)
    :   scene(scene)
{
}

void Renderer::setScene(Scene *scene) {
    this->scene = scene;
}

QImage Renderer::generate(QProgressBar *progress, QImage image) {
    return image;
}

QPointF convertToQPointF(Vector3D point) {
    return QPointF(point.x, point.y);
}

Vector3D Renderer::screenToWorldCoordinates(const Vector3D vec) {
    float x = (vec.x - ((float) scene->camera.imageWidth) / 2)/scene->camera.imageWidth;
    float y = (vec.y - ((float) scene->camera.imageHeight) / 2)/scene->camera.imageHeight;
    float z = scene->camera.position.z + scene->camera.depth; //z = 0 ???

    return Vector3D(x,y,z);
}

Vector3D Renderer::worldToScreenCoordinates(const Vector3D vec) {
    float x = vec.x * scene->camera.imageWidth + ((float) scene->camera.imageWidth)/2;
    float y = vec.y * scene->camera.imageHeight + ((float) scene->camera.imageHeight)/2;
    float z = 0;  //z = 0 ???

    return Vector3D(x,y,z);
}
