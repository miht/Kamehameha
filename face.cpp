#include "face.h"

Face::Face(QString material)
    :   material(material)
{

}

bool Face::intersects(Ray ray, float &t0, float &t1, Intersection &intersection, bool smooth) {
    return false;
}

std::vector<Vertex3D> Face::getVertices() const {
    return vertices;
}

std::vector<Vector3D> Face::getPoints() const {
    std::vector<Vector3D> points;
    for(Vertex3D vertex : vertices) {
        points.push_back (vertex.position);
    }
    return points;
}


