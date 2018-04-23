#include "boundingbox.h"

BoundingBox::BoundingBox()
    :   min(Vector3D(0,0,0)), max(Vector3D(0,0,0))
{
}

BoundingBox::BoundingBox(Vector3D min, Vector3D max)
    :   min(min), max(max)
{
}

int BoundingBox::getLongestAxis() {
    float diffX = fabs(max.x - min.x);
    float diffY = fabs(max.y - min.y);
    float diffZ = fabs(max.z - min.z);

    if(diffX > diffY && diffX > diffZ) return 0;    //X is longest
    if(diffY > diffX && diffY > diffZ) return 1;    //Y is longest
    if(diffZ > diffX && diffZ > diffY) return 2;    //Z is longest

    return 1;
}

void BoundingBox::expand(BoundingBox bbox) {
    Vector3D tmpMax = bbox.max;
    Vector3D tmpMin = bbox.min;

    max.x = fmax(max.x, tmpMax.x);
    max.y = fmax(max.y, tmpMax.y);
    max.z = fmax(max.z, tmpMax.z);

    min.x = fmin(min.x, tmpMin.x);
    min.y = fmin(min.y, tmpMin.y);
    min.z = fmin(min.z, tmpMin.z);
}


bool BoundingBox::intersects(Ray ray, float &t0, float &t1, Intersection &intersection) {
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
    return ((tmin < t1) && (tmax > t0) );
}
