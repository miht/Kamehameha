#include "objparser.h"
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QString>

//DEFAULT BLENDER EXPORT IS -Z FORWARD, Y UP... SO FLIP PARSED Y and Z

ObjParser::ObjParser()
{
}

Model ObjParser::parse(QString absPath, QString filePath)
{
    Model model = Model();

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<Vector3D> temp_vertices;
    std::vector<Vector2D> temp_uvs;
    std::vector<Vector3D> temp_normals;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug("Could not open object file.\n");
        return model;
    }

    QString current_material = "";

    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        QStringList sl = line.split(" ");

        if(sl.value(0) == "mtllib") {
            model.materials = parseMaterial(absPath + sl.value(1));
        }
        else if (sl.value(0) == "o") { //new object
            model.objects.push_back (Object(sl.value(1)));
        }
        else if (sl.value(0) == "v") { //vertex
            Vector3D vertex = Vector3D(sl.value(1).toFloat(),
                                       -sl.value(2).toFloat(), //negative y value to turn things upside down
                                       -sl.value(3).toFloat()); //negative z value to compensate for export y-direction in blender

            //Set the bounding box of the object
            //This code is too large, must clean up
            if(vertex.x < model.objects.back ().bbox.min.x) {
                model.objects.back ().bbox.min.x = vertex.x;
            }
            if(vertex.x > model.objects.back().bbox.max.x) {
                model.objects.back ().bbox.max.x = vertex.x;
            }
            if(vertex.y < model.objects.back ().bbox.min.y) {
                model.objects.back ().bbox.min.y = vertex.y;
            }
            if(vertex.y > model.objects.back().bbox.max.y) {
                model.objects.back ().bbox.max.y = vertex.y;
            }
            if(vertex.z < model.objects.back ().bbox.min.z) {
                model.objects.back ().bbox.min.z = vertex.z;
            }
            if(vertex.z > model.objects.back().bbox.max.z) {
                model.objects.back ().bbox.max.z = vertex.z;
            }
            temp_vertices.push_back(vertex);
        } else if (sl.value(0) == "vt") {
            Vector2D uv = Vector2D(sl.value(1).toFloat(), sl.value(2).toFloat());
            temp_uvs.push_back(uv);
        } else if (sl.value(0) == "vn") {
            Vector3D normal = Vector3D(sl.value(1).toFloat() ,
                                       -sl.value(2).toFloat(),
                                       -sl.value(3).toFloat()); //same here, negative
            temp_normals.push_back(normal);
        } else if (sl.value(0) == "usemtl"){
            current_material = sl.value(1);
        } else if (sl.value(0) == "f") {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            for(int i = 0; i < 3; i++) {
                QStringList vertex_line = sl.value(1 + i).split("/");
                vertexIndex[i] = vertex_line.value(0).toInt();
                uvIndex[i] = vertex_line.value(1).toInt();
                normalIndex[i] = vertex_line.value(2).toInt();
            }

            Vertex3D v1 = Vertex3D(Vector3D(temp_vertices[vertexIndex[0] - 1]), Vector3D(temp_normals[normalIndex[0] - 1]));
            Vertex3D v2 = Vertex3D(Vector3D(temp_vertices[vertexIndex[1] - 1]), Vector3D(temp_normals[normalIndex[1] - 1]));
            Vertex3D v3 = Vertex3D(Vector3D(temp_vertices[vertexIndex[2] - 1]), Vector3D(temp_normals[normalIndex[2] - 1]));

            model.objects.back ().faces.push_back(new Triangle(v1, v2, v3, current_material));
            //            qDebug() << current_material;

            //TODO implement UV mappings

            //          uvIndices.push_back(uvIndex[0]);
            //          uvIndices.push_back(uvIndex[1]);
            //          uvIndices.push_back(uvIndex[2]);


        } else if(sl.value(0) == "s") {
            model.objects.back ().smooth = sl.value(1) != "off";
            //            isCurrentSmooth = sl.value(1) != "off";
        }
    }
    file.close ();
    return model;
}

QMap<QString, Material> ObjParser::parseMaterial(QString path) {
    QMap<QString, Material> materials;
    QFile file(path);
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

            material.spec_exp = file.readLine ().trimmed().split(' ').value(1).toFloat ();

            QList<QByteArray> line_ambient = file.readLine().trimmed().split(' ');
            material.ambient = Vector3D(line_ambient.value(1).toFloat(),
                                        line_ambient.value(2).toFloat(), line_ambient.value(3).toFloat());

            QList<QByteArray> line_diffuse = file.readLine().trimmed().split(' ');
            material.diffuse = Vector3D(line_diffuse.value(1).toFloat(),
                                        line_diffuse.value(2).toFloat(), line_diffuse.value(3).toFloat());

            QList<QByteArray> line_specular = file.readLine().trimmed().split(' ');
            material.specular = Vector3D(line_specular.value(1).toFloat(),
                                         line_specular.value(2).toFloat(), line_specular.value(3).toFloat());

            QList<QByteArray> line_emissive = file.readLine().trimmed().split(' ');
            material.emissive = Vector3D(line_emissive.value(1).toFloat(),
                                         line_emissive.value(2).toFloat(), line_emissive.value(3).toFloat());

            material.optical_density = file.readLine ().trimmed().split(' ').value(1).toFloat ();

            material.dissolved = file.readLine ().trimmed().split(' ').value(1).toFloat ();

            int v = file.readLine ().trimmed().split(' ').value(1).toInt ();
            material.illModel = IlluminationModel(v);

//            qDebug() << material;

            materials.insert(mat_name, material);
        }
    }

    file.close();
    return materials;
}


