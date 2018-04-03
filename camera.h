#ifndef CAMERA_H
#define CAMERA_H

#include "vector3d.h"

class Camera
{
public:
    //TODO: FIND OUT A WAY TO CONVERT PROPERLY
    static const int DEFAULT_VIEWPORT_WIDTH = 128;
    static const int DEFAULT_VIEWPORT_HEIGHT = 128;

    enum Mode {perspective, ortographic};
    Mode mode = ortographic;
    Vector3D position = Vector3D(0,0,0);
    int viewportWidth = DEFAULT_VIEWPORT_WIDTH;
    int viewportHeight = DEFAULT_VIEWPORT_HEIGHT;

    float depth = 3;

    Camera();
    Camera(Vector3D position, Mode mode);
};

#endif // CAMERA_H
