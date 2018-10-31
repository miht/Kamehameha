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
Vector3D Renderer::viewportToWorld(const Vector3D vec) {
    Vector3D ret;
    ret.x = vec.x / (0.5 * viewportWidth) - 1;
    ret.y = -(vec.y / viewportHeight - 1) / 0.5 - 1;
    ret.z = vec.z;
    return ret;
}

Vector3D Renderer::worldToViewport(const Vector3D vec) {
    Vector3D ret;
    ret.x = (vec.x + 1) * 0.5 * viewportWidth;
    ret.y = (1 - (vec.y + 1) * 0.5) * viewportHeight;
    ret.z = vec.z;
    return ret;
}

void Renderer::calculatePerspective(const float angleOfView, const float imageAspectRatio, const float n,
                                    const float f, float &b, float &t, float &l, float &r) {
    float scale = tanf(angleOfView * 0.5 * M_PI / 180) * n;
    r = imageAspectRatio * scale;
    l = -r;
    t = scale;
    b = -t;
}

void Renderer::calculateOrthographic(const float angleOfView, const float imageAspectRatio, const float n,
                                    const float f, float &b, float &t, float &l, float &r) {
    float scale = tanf(angleOfView * 0.5 * M_PI / 180);
    r = imageAspectRatio * scale;
    l = -r;
    t = scale;
    b = -t;
}

Matrix4x4 Renderer::getPerspectiveFrustrum(float b, float t, float l, float r, float n, float f) {
    Matrix4x4 mat;
    mat(0, 0) = 2.f * n / (r - l);
    mat(1, 1) = 2.f * n / (t - b);
    mat(2, 0) = (r + l) / (r - l);
    mat(2, 1) = (t + b) / (t - b);
    mat(2, 2) = -(f + n) / (f - n);
    mat(2, 3) = -1.f;
    mat(3, 2) = -2.f * f * n / (f - n);

    return mat;
}

Matrix4x4 Renderer::getOrthographicFrustrum(float b, float t, float l, float r, float n, float f) {
    Matrix4x4 mat;
    mat(0, 0) = 2.f / (r - l);
    mat(1, 1) = 2.f / (t - b);
    mat(2, 2) = -2.f / (f - n);
    mat(3, 0) = -(r + l) / (r - l);
    mat(3, 1) = -(t + b) / (t - b);
    mat(3, 2) = -(f + n) / (f - n);
    mat(3, 3) = 1.f;

    return mat;
}

float Renderer::edgeFunction(const Vector3D v1, const Vector3D v2, const Vector3D c) {
    return (c.x - v1.x) * (v2.y - v1.y) - (c.y - v1.y) * (v2.x - v1.x);
}
