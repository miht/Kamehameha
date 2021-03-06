#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"
#include "transform.h"

class Camera : public Transform
{
public:
    //TODO: FIND OUT A WAY TO CONVERT PROPERLY
    static const int DEFAULT_VIEWPORT_WIDTH = 128;
    static const int DEFAULT_VIEWPORT_HEIGHT = 128;

    float apertureWidth = 0, apertureHeight = 0;
    int viewportWidth = DEFAULT_VIEWPORT_WIDTH, viewportHeight = DEFAULT_VIEWPORT_HEIGHT;
    float angleOfView = 45;
    float clippingPlane[2] = {0.1, 100};
    float window[4] = {0,0,0,0};
    float filmGateAspectRatio = 4/3;
    float resGateAspectRatio = 4/3;
    float focalLength = 35;
    float depth = 3;

    enum Mode {perspective, ortographic};
    Mode mode = ortographic;

    Camera();

    void lookAt(Vector3D target);

private:
    void updateAxises();
};

#endif // CAMERA_H


