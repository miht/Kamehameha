#ifndef SETTINGS_H
#define SETTINGS_H

#include "geometry.h"

class Settings
{
public:

    int subdivisions, depth, samples;
    bool directLighting, indirectLighting, antiAliasing, globalIllumination;
    Settings();
};

#endif // SETTINGS_H
