#include "scene.h"

Scene::Scene(float ambient_intensity, Color ambient_color)
    :   ambient_intensity(ambient_intensity), ambient_color(ambient_color)
{
    QString path = "/Users/leifthysellsundqvist/Library/Mobile Documents/com~apple~CloudDocs/Skolan/Y4/VisGraf18/ProjectTest/";
    QString file = "sphere";

    isLoaded = true;

//    model = ObjParser::parse(path + file);
}
