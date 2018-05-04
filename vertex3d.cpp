#include "vertex3d.h"

Vertex3D::Vertex3D(Vector3D position, Vector3D normal, Vector2D uv)
    :   position(position), normal(normal), uv(uv)
{

}

std::ostream & operator<<(std::ostream & Str, const Vertex3D& v) {
    return Str << "pos: " << v.position << ", nor: " << v.normal;
}

QDebug operator<< (QDebug d, const Vertex3D &v) {
    return d << "pos:" << v.position << ", nor:" << v.normal;
}
