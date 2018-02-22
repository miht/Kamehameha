#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <string>
#include "model3d.h"

class ObjParser
{
public:
    static Model3D parse(std::string path);

private:
    ObjParser();
};

#endif // OBJPARSER_H
