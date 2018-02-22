#include "triangle3d.h"
#include <iostream>

Triangle3D::Triangle3D(Vertex3D v1, Vertex3D v2, Vertex3D v3)
    :   v1(v1), v2(v2), v3(v3)
{
}

std::ostream & operator<<(std::ostream & Str, Triangle3D const & t) {
    // print something from v to str, e.g: Str << v.getX();
    //Str << t.v1 << ", " << t.v2 << ", " << t.v3;
    return Str;
}
