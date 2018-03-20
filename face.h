#ifndef FACE_H
#define FACE_H

#include <QString>
#include <vector>

#include "ray.h"
#include "vertex3d.h"
#include "intersection.h"

class Face
{
public:
    enum Geometry {triangle, quadrilateral, sphere, polygon};
    Geometry type;
    Face(QString material);

    virtual bool intersects(Ray ray, float &t0, float &t1, Intersection &intersection, bool smooth);
    virtual std::vector<Vertex3D> getVertices() const;
    virtual std::vector<Vector3D> getPoints() const;

protected:
    std::vector<Vertex3D> vertices;
    QString material;
};

#endif // FACE_H
