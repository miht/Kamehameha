#ifndef ILLUMINATIONMODEL_H
#define ILLUMINATIONMODEL_H


class IlluminationModel
{
public:
    bool ambient, color, raytrace, reflection, highlight, transparency = false; //ADD MORE ILLUMINATION MODES HERE
    IlluminationModel(int model);
    IlluminationModel();
};

#endif // ILLUMINATIONMODEL_H
