#include "triangle.h"

Triangle::Triangle(Vertex3D v1, Vertex3D v2, Vertex3D v3, QString material)
    :  Shape(material)
{
    type = Shape::triangle;
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
}

bool Triangle::intersects(Ray ray, float &dist, Intersection &intersection) {
    float epsilon = 0.00001;
    Vector3D vec0 = vertices[0].position;
    Vector3D vec1 = vertices[1].position;
    Vector3D vec2 = vertices[2].position;

    Vector3D edge1, edge2, tvec, pvec, qvec;
        float det,inv_det,u,v;
        edge1 = vec1 - vec0;
        edge2 = vec2 - vec0;

        Vector3D normal = Vector3D::cross_prod (edge1, edge2).normalized();
        pvec = Vector3D::cross_prod(ray.direction, edge2);

        det = Vector3D::dot_prod (edge1, pvec); //det
        if (det < epsilon)
            return false;
        //qDebug() << "check 2";

        if (fabs(det) < epsilon) return false;
        inv_det = 1/det; //inv det

        tvec = ray.origin - vec0;
        u = inv_det * Vector3D::dot_prod (tvec, pvec);
        if (u < 0.0 || u > 1.0) return false;

        //qDebug() << "check 3";

        qvec = Vector3D::cross_prod (tvec, edge1); //qvec
        v = inv_det * Vector3D::dot_prod (ray.direction, qvec);
        if (v < 0.0 || u + v > 1.0) return false;

        //qDebug() << "check 4";

        float t = inv_det * Vector3D::dot_prod (edge2, qvec);
        if(t <= 0) return false; //distance negative => object is behind ray

        dist = t;
        intersection = Intersection(ray.pointOnRay (dist), normal, material);
        intersection.hit = true;

        //intersection = RayIntersection(ray,shared_from_this(), t, normal,Vector3D(0,0,0));

        return true;
}

std::ostream & operator<<(std::ostream & Str, const Triangle& t) {
    //print something from v to str, e.g: Str << v.getX();
    return Str << t.vertices[0] << ", " << t.vertices[1] << ", " << t.vertices[2];
}

QDebug operator<< (QDebug d, const Triangle &t) {
    return d << "tri: " << t.vertices[0] << ", " << t.vertices[1] << ", " << t.vertices[2] << ", mat: " << t.material;
}
