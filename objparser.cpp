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
    Model3D m = Model3D();

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<Vector3D> temp_vertices;
    std::vector<Vector2D> temp_uvs;
    std::vector<Vector3D> temp_normals;

    QFile file(path);
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
           qDebug("Could not open the file.\n");
           return m;
       }

    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList sl = line.split(" ");

        if (sl.value(0) == "v"){
            Vector3D vertex = Vector3D(sl.value(1).toDouble() ,sl.value(2).toDouble(), sl.value(3).toDouble());
            temp_vertices.push_back(vertex);
        } else if (sl.value(0) == "vt") {
            Vector2D uv = Vector2D(sl.value(1).toDouble(), sl.value(2).toDouble());
            temp_uvs.push_back(uv);
        } else if (sl.value(0) == "vn") {
            Vector3D normal = Vector3D(sl.value(1).toDouble() ,sl.value(2).toDouble(), sl.value(3).toDouble());
            temp_normals.push_back(normal);
        } else if (sl.value(0) == "f"){
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

            m.triangles.push_back(Triangle3D(v1, v2, v3));

            //          uvIndices.push_back(uvIndex[0]);
            //          uvIndices.push_back(uvIndex[1]);
            //          uvIndices.push_back(uvIndex[2]);

            //          normalIndices.push_back(normalIndex[0]);
            //          normalIndices.push_back(normalIndex[1]);
            //          normalIndices.push_back(normalIndex[2]);
        }
    }
    file.close ();
    return m;
}
