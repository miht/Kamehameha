#include "model.h"

Model::Model()
{
    Material m;
    m.ambient = Vector3D(1, 1, 1);
    m.diffuse = Vector3D(0.5, 0.5, 0.5);
    m.specular = Vector3D(0.5, 0.5, 0.5);
    m.spec_exp = 96.0;
    m.optical_density = 1.0;
    m.dissolved = 1.0;

    materials.insert("None", m);

}
