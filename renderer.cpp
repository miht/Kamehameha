#include "renderer.h"

Renderer::Renderer(QWidget *parent, Scene *scene, Settings *settings)
    :   QMainWindow(parent), scene(scene), settings(settings)
{
}

void Renderer::setScene(Scene *scene) {
    this->scene = scene;
}

void Renderer::render() {
    return;
}


QImage Renderer::generate(QImage image) {
    return image;
}

QPointF convertToQPointF(Vector3D point) {
    return QPointF(point.x, point.y);
}

//TODO TODO TODO
Vector3D Renderer::viewportToWorld(const Vector2D vec) {
    int width = viewportWidth;
    int height = viewportHeight;
    float canvasWidth = (float)width/(float)height, canvasHeight = 1;

    Vector2D pNDC(vec.x / width, -vec.y/height + 1);
    Vector2D pScreen(pNDC.x * canvasWidth - 0.5 * canvasWidth, pNDC.y * canvasHeight - 0.5 * canvasHeight);
    Vector3D pCamera(pScreen.x, pScreen.y , -1);

    Vector3D pos = scene->camera.world * pCamera;

    return pos;
}

Vector3D Renderer::worldToViewport(const Vector3D vec) {
    Vector3D ret;
    ret.x = (vec.x + 1) * 0.5 * viewportWidth;
    ret.y = (1 - (vec.y + 1) * 0.5) * viewportHeight;
    ret.z = vec.z;
    return ret;
}

void Renderer::setProjectionMatrix() {
    float fov = scene->camera.angleOfView;
    float near = scene->camera.clippingPlane[0];
    float far = scene->camera.clippingPlane[1];

    float scale = 1.0 / tanf(fov * 0.5f * M_PI / 180);
    projMatrix(0, 0) = scale;
    projMatrix(1, 1) = scale;
    projMatrix(2, 2) = -far / (far - near);
    projMatrix(3, 2) = -far * near / (far - near);
    projMatrix(2, 3) = -1;
    projMatrix(3, 3) = 0;
}

void Renderer::calculatePerspective(const float angleOfView, const float imageAspectRatio, const float n,
                                    const float f, float &b, float &t, float &l, float &r) {
    float scale = tanf(angleOfView * 0.5 * M_PI / 180) *n;
    r = imageAspectRatio * scale;
    l = -r;
    t = scale;
    b = -t;
}

Matrix4x4 Renderer::getPerspectiveFrustrum(float b, float t, float l, float r, float n, float f) {
    Matrix4x4 mat;
    mat(0, 0) = 2 * n / (r - l);
    mat(1, 1) = 2 * n / (t - b);
    mat(2, 0) = (r + l) / (r - l);
    mat(2, 1) = (t + b) / (t - b);
    mat(2, 2) = -(f + n) / (f - n);
    mat(2, 3) = -1;
    mat(3, 2) = -2 * f * n / (f - n);

    return mat;
}

float Renderer::edgeFunction(const Vector3D v1, const Vector3D v2, const Vector3D c) {
    return (c.x - v1.x) * (v2.y - v1.y) - (c.y - v1.y) * (v2.x - v1.x);
}
