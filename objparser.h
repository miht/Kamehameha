#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <QString>
#include <QMap>

#include "model3d.h"
#include "vector2d.h"
#include "vector3d.h"
#include "vertex3d.h"

class ObjParser
{
public:
    static Model3D parse(QString path);

private:
    ObjParser();
    static QMap<QString, Material> parseMaterial(QString path);
};

#endif // OBJPARSER_H
