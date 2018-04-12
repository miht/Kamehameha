#include "camera.h"

Camera::Camera()
{

}

void Camera::setPosition(Vector3D newPos) {
    position = newPos;

    //recalculate direction vectors
    updateAxises ();
}
void Camera::lookAt(Vector3D newTar) {
    target = newTar;
    updateAxises();

    //Clear camToWorld
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            camToWorld[i][j] = 0;
        }
    }

    camToWorld[0][0] = right.x;
    camToWorld[0][1] = right.y;
    camToWorld[0][2] = right.z;
    camToWorld[1][0] = up.x;
    camToWorld[1][1] = up.y;
    camToWorld[1][2] = up.z;
    camToWorld[2][0] = forward.x;
    camToWorld[2][1] = forward.y;
    camToWorld[2][2] = forward.z;

    camToWorld[3][0] = position.x;
    camToWorld[3][1] = position.y;
    camToWorld[3][2] = position.z;
}

void Camera::updateAxises() {
    //recalculate direction vectors
    forward = (target - position).normalized ();
    right = Vector3D::cross_prod (Vector3D(0,1,0).normalized (), forward);
    up = Vector3D::cross_prod (forward, right);
}

