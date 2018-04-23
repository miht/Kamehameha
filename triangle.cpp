#include "triangle.h"

Triangle::Triangle(Vertex3D v1, Vertex3D v2, Vertex3D v3, QString material)
    :  Face(material)
{
    type = Face::triangle;
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    //calculate triangle midpoint
    midpoint.x = (v1.position.x + v2.position.x + v3.position.x)/3;
    midpoint.y = (v1.position.y + v2.position.y + v3.position.y)/3;
    midpoint.z = (v1.position.z + v2.position.z + v3.position.z)/3;

    //calculate bounding box
    Vector3D max, min;
    max.x = fmax(v1.position.x, fmax(v2.position.x, v3.position.x));
    max.y = fmax(v1.position.y, fmax(v2.position.y, v3.position.y));
    max.z = fmax(v1.position.z, fmax(v2.position.z, v3.position.z));

    min.x = fmin(v1.position.x, fmin(v2.position.x, v3.position.x));
    min.y = fmin(v1.position.y, fmin(v2.position.y, v3.position.y));
    min.z = fmin(v1.position.z, fmin(v2.position.z, v3.position.z));

    bbox.max = max;
    bbox.min = min;
}

bool Triangle::intersects(Ray ray, float &t0, float &t1, Intersection &intersection, bool smooth) {
    float epsilon = 0.00000001;
    Vector3D vec0 = vertices[0].position;
    Vector3D vec1 = vertices[1].position;
    Vector3D vec2 = vertices[2].position;

    Vector3D edge1, edge2, tvec, pvec, qvec;
    float det,inv_det,u,v;
    edge1 = vec1 - vec0;
    edge2 = vec2 - vec0;

    Vector3D normal = Vector3D::cross_prod (edge1, edge2).normalized();

    pvec = Vector3D::cross_prod(ray.direction, edge2); //vec 0

    det = Vector3D::dot_prod (edge1, pvec); //det

    if (det < epsilon)
        return false;

    if (fabs(det) < epsilon) return false;

    inv_det = 1/det; //inv det

    tvec = ray.origin - vec0;
    u = inv_det * Vector3D::dot_prod (tvec, pvec);
    if (u < 0.0 || u > 1.0) return false;

    qvec = Vector3D::cross_prod (tvec, edge1); //qvec
    v = inv_det * Vector3D::dot_prod (ray.direction, qvec);
    if (v < 0.0 || u + v > 1.0) return false;

    float dist = inv_det * Vector3D::dot_prod (edge2, qvec);
    if(dist <= t0) return false; //distance negative => object is behind ray
    if(dist > t1) return false; //distance too large => object is too far away
    t1 = dist;

    if(smooth) {
        //            normal = vertices[1].normal.normalized();
        normal = u * vertices[1].normal.normalized () + v*vertices[2].normal.normalized ()+ ((float) 1.0 - u - v)*vertices[0].normal.normalized ();
        normal = normal.normalized ();
        //            qDebug() << "normal interpolated: " << normal;
    }

    intersection = Intersection(ray.pointOnRay (t1), normal, material);
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
