#include "transform.h"

Transform::Transform()
{
}

void Transform::lookAt(Vector3D newTar) {
    target = newTar;

    forward = (target - position).normalized ();
    right = Vector3D::cross_prod (Vector3D(0,1,0).normalized (), forward);
    up = Vector3D::cross_prod (forward, right);

    //Clear world
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            world(i,j) = 0;
        }
    }

    world(0, 0) = right.x;
    world(0, 1) = right.y;
    world(0,2) = right.z;

    world(1, 0) = up.x;
    world(1, 1) = up.y;
    world(1, 2) = up.z;

    world(2, 0) = forward.x;
    world(2, 1) = forward.y;
    world(2, 2) = forward.z;

    world(3, 0) = position.x;
    world(3, 1) = position.y;
    world(3, 2) = position.z;
}

