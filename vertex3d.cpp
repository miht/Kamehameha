#include "vertex3d.h"
#include <vector3d.h>
#include <iostream>

Vertex3D::Vertex3D(Vector3D position, Vector3D normal)
    :   position(position), normal(normal)
{

}

std::ostream & operator<<(std::ostream & Str, Vertex3D const & v) {
    Str << "hello";
    //Str << "pos: " << v.position << ", nor: " << v.normal;
    return Str;
}
