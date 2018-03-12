#ifndef SCENE_H
#define SCENE_H

#include "model.h"
#include "light.h"
#include "objparser.h"

class Scene
{
public:
    std::vector<Light> lights;
    Model model;

    Scene();
};

#endif // SCENE_H
