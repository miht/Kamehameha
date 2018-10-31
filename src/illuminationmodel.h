#ifndef ILLUMINATIONMODEL_H
#define ILLUMINATIONMODEL_H


class IlluminationModel
{
public:
    bool ambient = false, color = false, raytrace = false,
    reflection = false, highlight = false, transparency = false; //ADD MORE ILLUMINATION MODES HERE

//    bool ambient = false, bool color = false, bool raytrace = false;
//    bool reflection = false;
//    bool highlight = false;
//    bool transparency = false; //ADD MORE ILLUMINATION MODES HERE
    IlluminationModel(int model);
    IlluminationModel();
};

#endif // ILLUMINATIONMODEL_H
