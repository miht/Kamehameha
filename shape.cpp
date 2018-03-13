#include "shape.h"

Shape::Shape(QString material)
    :   material(material)
{

}

bool Shape::intersects(Ray ray, float &dist, Intersection &intersection) {
    return false;
}

std::vector<Vector3D> Shape::getPoints() const {
    std::vector<Vector3D> points;
    for(Vertex3D vertex : vertices) {
        points.push_back (vertex.position);
    }
    return points;
}


