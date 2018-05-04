#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"

class Camera
{
public:
    //TODO: FIND OUT A WAY TO CONVERT PROPERLY
    static const int DEFAULT_VIEWPORT_WIDTH = 128;
    static const int DEFAULT_VIEWPORT_HEIGHT = 128;

    float apertureWidth = 0, apertureHeight = 0;
    int imageWidth = DEFAULT_VIEWPORT_WIDTH, imageHeight = DEFAULT_VIEWPORT_HEIGHT;
    float angleOfView = 45;
    float clippingPlane[2] = {0,0};
    float window[4] = {0,0,0,0};
    float filmGateAspectRatio = 4/3;
    float resGateAspectRatio = 4/3;
    float focalLength = 35;
    float depth = 3;
    Matrix4x4 camToWorld;

    enum Mode {perspective, ortographic};
    Mode mode = ortographic;

    Vector3D position = Vector3D(0,0,0);
    Vector3D target = Vector3D(0,0,0);

    Vector3D forward, right, up;

    Camera();

    void lookAt(Vector3D target);

private:
    void updateAxises();
};

#endif // CAMERA_H
