#ifndef CAMERA_H
#define CAMERA_H

#include "vector3d.h"

class Camera
{
public:
    //TODO: FIND OUT A WAY TO CONVERT PROPERLY
    static const int DEFAULT_VIEWPORT_WIDTH = 128;
    static const int DEFAULT_VIEWPORT_HEIGHT = 128;

    int viewportWidth = DEFAULT_VIEWPORT_WIDTH;
    int viewportHeight = DEFAULT_VIEWPORT_HEIGHT;

    enum Mode {perspective, ortographic};
    Mode mode = ortographic;

    Vector3D position = Vector3D(0,0,-10);
    Vector3D target = Vector3D(0,0,0);

    Vector3D forward, right, up;

    float camToWorld[4][4] = {{0}};

    float depth = 3;

    Camera();

    void setPosition(Vector3D newPos);
    void lookAt(Vector3D target);

private:
    void updateAxises();
};

#endif // CAMERA_H
