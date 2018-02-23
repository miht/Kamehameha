#include "triangle3d.h"

Triangle3D::Triangle3D(Vertex3D v1, Vertex3D v2, Vertex3D v3)
    :   v1(v1), v2(v2), v3(v3)
{
}

std::ostream & operator<<(std::ostream & Str, const Triangle3D& t) {
    //print something from v to str, e.g: Str << v.getX();
    return Str << t.v1 << ", " << t.v2 << ", " << t.v3;
}

QDebug operator<< (QDebug d, const Triangle3D &t) {
    return d << t.v1 << ", " << t.v2 << ", " << t.v3;
}
