#ifndef SCENE_H
#define SCENE_H

#include "model.h"
#include "light.h"
#include "objparser.h"
#include "color.h"

class Scene
{
public:
    float ambient_intensity = 0.2;
    Color ambient_color;
    std::vector<Light> lights;
    Model model;

    Scene(float ambient_intensity, Color ambient_color);
};

#endif // SCENE_H
