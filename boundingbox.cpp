#include "boundingbox.h"

BoundingBox::BoundingBox(Vector3D min, Vector3D max)
    :   min(min), max(max)
{
}

bool BoundingBox::intersects(Ray ray, float &t0, float &t1) {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    if (ray.direction.x >= 0) {
        tmin = (min.x - ray.origin.x) / ray.direction.x;
        tmax = (max.x - ray.origin.x) / ray.direction.x;
    } else {
        tmin = (max.x - ray.origin.x) / ray.direction.x;
        tmax = (min.x - ray.origin.x) / ray.direction.x;
    }
    if (ray.direction.y >= 0) {
        tymin = (min.y - ray.origin.y) / ray.direction.y;
        tymax = (max.y - ray.origin.y) / ray.direction.y;
    } else {
        tymin = (max.y - ray.origin.y) / ray.direction.y;
        tymax = (min.y - ray.origin.y) / ray.direction.y;
    }
    if ((tmin > tymax) || (tymin > tmax) )
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
    if (ray.direction.z >= 0) {
        tzmin = (min.z - ray.origin.z) / ray.direction.z;
        tzmax = (max.z - ray.origin.z) / ray.direction.z;
    } else {
        tzmin = (max.z - ray.origin.z) / ray.direction.z;
        tzmax = (min.z - ray.origin.z) / ray.direction.z;
    }
    if ( (tmin > tzmax) || (tzmin > tmax) )
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
    return ( (tmin < t1) && (tmax > t0) );
}
