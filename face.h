#ifndef FACE_H
#define FACE_H

#include <QString>
#include <vector>

#include "ray.h"
#include "vertex3d.h"
#include "intersection.h"
#include "vector3d.h"
#include "boundingbox.h"

class Face
{
public:
    enum Geometry {triangle, quadrilateral, sphere, polygon};
    Geometry type;
    Face(const char *material);

    const char* material;

    Vector3D midpoint;
    BoundingBox bbox;

    virtual bool intersects(Ray ray, float &t0, float &t1, Intersection &intersection, bool smooth);
    virtual std::vector<Vertex3D> getVertices() const;
    virtual std::vector<Vector3D> getPoints() const;

protected:
    std::vector<Vertex3D> vertices;
};

class Triangle : public Face
{
public:
    Triangle(Vertex3D v1, Vertex3D v2, Vertex3D v3, const char* material);
    bool intersects(Ray ray, float &t0, float &t1, Intersection &intersection, bool smooth) override;
    static bool contains(const Vector2D v1, const Vector2D v2, const Vector2D v3, const Vector2D p);
    friend std::ostream & operator<<(std::ostream & Str, const Triangle& t);
    friend QDebug operator<< (QDebug d, const Triangle &t);
};

#endif // FACE_H
