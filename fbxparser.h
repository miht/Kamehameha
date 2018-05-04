#ifndef FBXPARSER_H
#define FBXPARSER_H

#include "fbxsdk.h"
#include "geometry.h"
#include "face.h"
#include "material.h"
#include "geometry.h"
#include "light.h"
#include <QMap>

class FbxParser
{
public:
    FbxParser();

    static void process(FbxNode *node, std::vector<Face*> &faces, std::vector<Light> &lights, QMap<const char*, Material> &materials);

    static void processMaterials(FbxMesh *mesh, QMap<const char*, Material> &materials);
    static void processMesh(const FbxMesh *mesh, std::vector<Face*> &faces);
    static void processLight(const FbxLight *light, std::vector<Light> &lights);

};

#endif // FBXPARSER_H
