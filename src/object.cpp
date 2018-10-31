#include "object.h"

Object::Object(QString name)
    :   name(name), bbox(BoundingBox(Vector3D(9999, 9999, 9999), Vector3D(-9999, -9999, -9999)))
{
    //starter bounding box
}
