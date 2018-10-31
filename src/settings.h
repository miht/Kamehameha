#ifndef SETTINGS_H
#define SETTINGS_H

#include "geometry.h"

class Settings
{
public:
    //General
    int width, height;
    int depth = 2;
    bool antiAliasing = false;
    bool directLighting = true;
    bool indirectLighting = false;

    //Ray/path exclusive
    int subdivisions = 4;

    //Path tracer
    int samples = 32;
    bool globalIllumination;

    Settings();
};

#endif // SETTINGS_H
