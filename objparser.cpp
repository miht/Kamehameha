#include "objparser.h"
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QString>

ObjParser::ObjParser()
{
}

Model3D ObjParser::parse(QString path)
{
    Model3D model = Model3D();

    model.materials = parseMaterial(path);

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<Vector3D> temp_vertices;
    std::vector<Vector2D> temp_uvs;
    std::vector<Vector3D> temp_normals;

    QFile file(path + ".obj");
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
           qDebug("Could not open object file.\n");
           return model;
       }

       QString current_material = "";

    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        QStringList sl = line.split(" ");

        if (sl.value(0) == "v"){
            Vector3D vertex = Vector3D(sl.value(1).toDouble(),
                            -sl.value(2).toDouble(), //negative y value to turn things upside down
                             -sl.value(3).toDouble()); //negative z value to compensate for export y-direction in blender
            temp_vertices.push_back(vertex);
        } else if (sl.value(0) == "vt") {
            Vector2D uv = Vector2D(sl.value(1).toDouble(), sl.value(2).toDouble());
            temp_uvs.push_back(uv);
        } else if (sl.value(0) == "vn") {
            Vector3D normal = Vector3D(sl.value(1).toDouble() ,sl.value(2).toDouble(), sl.value(3).toDouble());
            temp_normals.push_back(normal);
        } else if (sl.value(0) == "usemtl"){
            current_material = sl.value(1);
        } else if (sl.value(0) == "f") {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            for(int i = 0; i < 3; i++) {
                QStringList vertex_line = sl.value(1 + i).split("/");
                vertexIndex[i] = vertex_line.value(0).toInt();
                uvIndex[i] = vertex_line.value(1).toInt();
                normalIndex[i] = vertex_line.value(2).toInt();
            }

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);

            Vertex3D v1 = Vertex3D(Vector3D(temp_vertices[vertexIndex[0] - 1]), Vector3D(temp_normals[normalIndex[0] - 1]));
            Vertex3D v2 = Vertex3D(Vector3D(temp_vertices[vertexIndex[1] - 1]), Vector3D(temp_normals[normalIndex[1] - 1]));
            Vertex3D v3 = Vertex3D(Vector3D(temp_vertices[vertexIndex[2] - 1]), Vector3D(temp_normals[normalIndex[2] - 1]));

            model.triangles.push_back(Triangle(v1, v2, v3, current_material));
//            qDebug() << current_material;

            //TODO implement UV mappings

            //          uvIndices.push_back(uvIndex[0]);
            //          uvIndices.push_back(uvIndex[1]);
            //          uvIndices.push_back(uvIndex[2]);

            //          normalIndices.push_back(normalIndex[0]);
            //          normalIndices.push_back(normalIndex[1]);
            //          normalIndices.push_back(normalIndex[2]);
        } else if(sl.value(0) == "s") {
            model.smooth = sl.value(1) != "off";
        }
    }
    file.close ();
    return model;
}

QMap<QString, Material> ObjParser::parseMaterial(QString path) {
    QMap<QString, Material> materials;
    QFile file(path + ".mtl");
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
           qDebug("Could not open material file.\n");
           return materials;
       }

       while (!file.atEnd()) {
           QString line = file.readLine();
           QStringList sl = line.split(" ");

           if(sl.value(0) == "newmtl") {
               QString mat_name = sl.value(1).trimmed();
               Material material;

               material.spec_exp = file.readLine ().trimmed().split(' ').value(1).toDouble ();

               QList<QByteArray> line_ambient = file.readLine().trimmed().split(' ');
               material.ambient = Vector3D(line_ambient.value(1).toDouble(),
                                           line_ambient.value(2).toDouble(), line_ambient.value(3).toDouble());

               QList<QByteArray> line_diffuse = file.readLine().trimmed().split(' ');
               material.diffuse = Vector3D(line_diffuse.value(1).toDouble(),
                                           line_diffuse.value(2).toDouble(), line_diffuse.value(3).toDouble());

              QList<QByteArray> line_specular = file.readLine().trimmed().split(' ');
               material.specular = Vector3D(line_specular.value(1).toDouble(),
                                           line_specular.value(2).toDouble(), line_specular.value(3).toDouble());

           materials.insert(mat_name, material);
           }
       }

       file.close();
       return materials;
}


