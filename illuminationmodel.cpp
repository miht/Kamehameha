#include "illuminationmodel.h"

//0. Color on and Ambient off
//1. Color on and Ambient on
//2. Highlight on
//3. Reflection on and Ray trace on
//4. Transparency: Glass on, Reflection: Ray trace on
//5. Reflection: Fresnel on and Ray trace on
//6. Transparency: Refraction on, Reflection: Fresnel off and Ray trace on
//7. Transparency: Refraction on, Reflection: Fresnel on and Ray trace on
//8. Reflection on and Ray trace off
//9. Transparency: Glass on, Reflection: Ray trace off
//10. Casts shadows onto invisible surfaces

//As a starter, we'll only be working with the first two illumination models.

IlluminationModel::IlluminationModel() {

}

IlluminationModel::IlluminationModel(int model)
{
    switch (model) {
    case 0:
        color = true;
        break;
    case 1:
        color = true;
        ambient = true;
        break;
    case 2:
        highlight = true;
        break;
    case 3:
        reflection = true;
        raytrace = true;
        break;
    case 4:
        //TODO: Transparency
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    default:
        //ERROR IN THE NUMBER?
        break;
    }
}
