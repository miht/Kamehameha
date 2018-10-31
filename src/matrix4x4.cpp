#include "matrix4x4.h"

Matrix4x4::Matrix4x4()
{
}

float& Matrix4x4::operator()(const int i, const int j) {
    return elements[i*4 + j];
}

Matrix4x4 operator*(Matrix4x4 &m1, Matrix4x4 &m2) {
    Matrix4x4 res;
    for(int i = 0; i < sizeof(elements) / sizeof(float); i++) {}
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            res(i, j) = m1(i,j)*m2(i,j);
        }
    }
    return res;
}


void Matrix4x4::translate(Vector3D vector) {

}

/*
* @brief rotate
* @param axis so far only defined for whole integer axises
* @param eulerAngles
*/
void Matrix4x4::rotate(Vector3D axis, float eulerAngles) {

}
