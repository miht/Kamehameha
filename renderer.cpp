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

//TODO TODO TODO
Vector3D Renderer::viewportToWorld(const Vector2D vec) {
    int width = scene->camera.viewportWidth;
    int height = scene->camera.viewportHeight;
    float canvasWidth = (float)width/(float)height, canvasHeight = 1;

    Vector2D pNDC(vec.x / width, -vec.y/height + 1);
    Vector2D pScreen(pNDC.x * canvasWidth - 0.5 * canvasWidth, pNDC.y * canvasHeight - 0.5 * canvasHeight);
    Vector3D pCamera(pScreen.x * (-1), pScreen.y * (-1), -1);

    Vector3D pos = scene->camera.camToWorld * pCamera;

    return pos;
}

Vector2D Renderer::worldToViewport(const Vector3D vec) {
    int width = scene->camera.viewportWidth;
    int height = scene->camera.viewportHeight;
    float canvasWidth = (float)width/(float)height, canvasHeight = 1;

    Matrix4x4 worldToCam;
    bool inverted = Matrix4x4::inverse(scene->camera.camToWorld, worldToCam);

    Vector3D pCamera = worldToCam*vec;
    Vector2D pScreen(pCamera.x / -pCamera.z, pCamera.y / -pCamera.z);
    Vector2D pNDC((pScreen.x + canvasWidth * 0.5) / canvasWidth, (pScreen.y + canvasHeight * 0.5) / canvasHeight);

    int pX = (int)(pNDC.x * width);
    int pY = (int)((1 - pNDC.y) * height);

    return Vector2D(pX, pY);
}
