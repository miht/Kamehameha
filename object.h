#ifndef OBJECT_H
#define OBJECT_H

#include <QString>

#include "boundingbox.h"
#include "triangle.h"

class Object
{
public:
    BoundingBox bbox;
    std::vector<Face*> faces;
    bool smooth = false;
    QString name = "Unnamed";

    Object(QString name);
};

#endif // OBJECT_H
