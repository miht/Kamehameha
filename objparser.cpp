#include "objparser.h"
#include "model3d.h"
#include "vertex3d.h"

ObjParser::ObjParser()
{
}

model3d ObjParser::parse(String path)
{
  Model3D m = new Model3D();

  vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  vector<Vector3D> temp_vertices;
  vector<Vector3D> temp_uvs;
  vector<Vector3D> temp_normals;

  FILE * file = fopen(path, "r");
  if( file == NULL ){
      printf("Could not open the file.\n");
      return null;
  }

  while( 1 ){
      char lineHeader[128];
      // read the first word of the line
      int res = fscanf(file, "%s", lineHeader);
      if (res == EOF)
          break; // EOF = End Of File. Quit the loop.

      // else : parse lineHeader
      if ( strcmp( lineHeader, "v" ) == 0 ){
          glm::vec3 vertex;
          fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
          temp_vertices.push_back(vertex);
      }else if ( strcmp( lineHeader, "vt" ) == 0 ){
          glm::vec2 uv;
          fscanf(file, "%f %f\n", &uv.x, &uv.y );
          temp_uvs.push_back(uv);
      }else if ( strcmp( lineHeader, "vn" ) == 0 ){
          glm::vec3 normal;
          fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
          temp_normals.push_back(normal);
      }else if ( strcmp( lineHeader, "f" ) == 0 ){
          std::string vertex1, vertex2, vertex3;
          unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
          int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
          if (matches != 9) {
              printf("File can't be read by our simple parser : ( Try exporting with other options\n");
              return null;
          }
          vertexIndices.push_back(vertexIndex[0]);
          vertexIndices.push_back(vertexIndex[1]);
          vertexIndices.push_back(vertexIndex[2]);

          Vertex3D v1 = new Vertex3D(new Vector3D(temp_vertices[vertexIndex[0] - 1]), new Vector3D(temp_normals[normalIndex[0] - 1]));
          Vertex3D v2 = new Vertex3D(new Vector3D(temp_vertices[vertexIndex[1] - 1]), new Vector3D(temp_normals[normalIndex[1] - 1]));
          Vertex3D v3 = new Vertex3D(new Vector3D(temp_vertices[vertexIndex[2] - 1]), new Vector3D(temp_normals[normalIndex[2] - 1]));

          m.triangles.push_back(new Triangle3D(v1, v2, v3));

//          uvIndices.push_back(uvIndex[0]);
//          uvIndices.push_back(uvIndex[1]);
//          uvIndices.push_back(uvIndex[2]);

//          normalIndices.push_back(normalIndex[0]);
//          normalIndices.push_back(normalIndex[1]);
//          normalIndices.push_back(normalIndex[2]);
      }

      return m;
}
