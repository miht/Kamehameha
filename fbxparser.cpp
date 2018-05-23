#include "fbxparser.h"


//EXPORT Forward: Y FORWARD Up: Z UP
FbxParser::FbxParser()
{

}

void FbxParser::process(FbxNode *node, Scene *scene) {
    int children = node->GetChildCount ();
    if(children <= 0) {
        return;
    }

    for(int i = 0; i < children; i ++) {
        FbxNode* child = node->GetChild (i);

        switch(child->GetNodeAttribute ()->GetAttributeType ()) {
            case FbxNodeAttribute::eLight: {

                FbxLight* light = child->GetLight ();
                processLight(light, scene->lights);

                scene->model.metadata.numLights ++;
                qDebug() << "Parsed eLight.";
                break;
            }
            case FbxNodeAttribute::eMesh: {

                FbxMesh* mesh = child->GetMesh ();

                processMesh(mesh, scene->model.root->faces);
                scene->model.metadata.numLights ++;


                processMaterials (mesh, scene->model.materials);


                scene->model.metadata.numFaces += mesh->GetPolygonCount ();
                scene->model.metadata.numVertices += mesh->GetControlPointsCount ();

                scene->model.metadata.numObjects ++;
                qDebug() << "Parsed eMesh.";

                break;
            }
            default: {
                break;
            }
        }
        process(child, scene);
    }

}

void FbxParser::processMesh(const FbxMesh *mesh, std::vector<Face*> &faces) {
    if(mesh->IsTriangleMesh ()) {
        for(int i = 0; i < mesh->GetPolygonCount (); i++) {
            FbxVector4 p1, p2, p3;
            p1 = mesh->GetControlPointAt (mesh->GetPolygonVertex (i, 0));
            p2 = mesh->GetControlPointAt (mesh->GetPolygonVertex (i, 1));
            p3 = mesh->GetControlPointAt (mesh->GetPolygonVertex (i, 2));

            //TODO: what to do if these return false?
            FbxVector4 n1, n2, n3;
            mesh->GetPolygonVertexNormal(i, 0, n1);
            mesh->GetPolygonVertexNormal(i, 1, n2);
            mesh->GetPolygonVertexNormal(i, 2, n3);

            //TODO: what to do if these return false? Furthermore, how to handle unmapped = true?
            FbxVector2 uv1, uv2, uv3;
            bool um1, um2, um3;
            mesh->GetPolygonVertexUV (i, 0, "UVMap", uv1, um1);
            mesh->GetPolygonVertexUV (i, 1, "UVMap", uv2, um2);
            mesh->GetPolygonVertexUV (i, 2, "UVMap", uv3, um3);



            Vertex3D v1 = Vertex3D(Vector3D(p1[0], p1[1], p1[2]),
                    Vector3D(n1[0], n1[1], n1[2]).normalized (), Vector2D(uv1[0], uv1[1]));
            Vertex3D v2 = Vertex3D(Vector3D(p2[0], p2[1], p2[2]),
                    Vector3D(n2[0], n2[1], n2[2]).normalized (), Vector2D(uv2[0], uv2[1]));
            Vertex3D v3 = Vertex3D(Vector3D(p3[0], p3[1], p3[2]),
                    Vector3D(n3[0], n3[1], n3[2]).normalized (), Vector2D(uv3[0], uv3[1]));

            int materialIndex = mesh->GetElementMaterial()->GetIndexArray ().GetAt(i);

            const char* mat = mesh->GetNode ()->GetMaterial(materialIndex)->GetName();

            faces.push_back (new Triangle(v1, v2, v3, mat));
        }
    }
}

void FbxParser::processLight(const FbxLight *light, std::vector<Light> &lights) {
    // Get the lights's default global transformation matrix.
    FbxAMatrix& lGlobalTransform = light->GetNode ()->EvaluateGlobalTransform();
    FbxAMatrix& lLocalTransform = light->GetNode ()->EvaluateLocalTransform ();
    //    FbxVector4& lLocalTranslation = light->GetNode ()->EvaluateLocalTranslation ();
    //    FbxVector4 lGlobalTranslation = lGlobalTransform.MultT (lLocalTranslation);

    FbxVector4 lGlobalTranslation = lGlobalTransform.GetT ();

    const Vector3D pos(lGlobalTranslation[0], lGlobalTranslation[1], lGlobalTranslation[2]);
    const float intensity = light->Intensity.Get ()/100.0; //Divide by because FBX default is 100

    const FbxDouble3 col = light->Color.Get();
    lights.push_back (Light(pos, intensity, Color(col[0], col[1], col[2])));
}

void FbxParser::processMaterials(FbxMesh *mesh, QMap<const char*, Material> &materials) {
    FbxNode* node = mesh->GetNode ();
    for(int i = 0; i < node->GetMaterialCount(); i++) {
        Material m;
        FbxSurfaceMaterial* lMaterial = node->GetMaterial (i);
        int materialIndex = node->GetMaterialIndex (lMaterial->GetName ());
        const char* mat = lMaterial->GetName ();
        materialIndex = i;

        FbxDouble3 ambient, diffuse, emissive, specular, reflective, transparent;
        FbxDouble ambientFactor, diffuseFactor, emissiveFactor, reflectiveFactor, transparentFactor, specularFactor;

        if(lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId) )
        {
            m.type = Material::Lambertian;
            FbxSurfaceLambert* lambert = (FbxSurfaceLambert*) lMaterial;

            ambient =  lambert->Ambient.Get ();
            diffuse = lambert->Diffuse.Get ();
            emissive = lambert->Emissive.Get ();
            transparent = lambert->TransparencyFactor.Get ();
            transparentFactor = lambert->TransparencyFactor.Get ();

            ambientFactor = lambert->AmbientFactor.Get ();
            diffuseFactor = lambert->DiffuseFactor.Get ();
            emissiveFactor = lambert->EmissiveFactor.Get ();

            if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
            {
                m.type = Material::Phong;

                FbxSurfacePhong* phong = (FbxSurfacePhong*) lambert;

                reflective = phong->Reflection.Get ();
                reflectiveFactor = phong->ReflectionFactor.Get();

                specular = phong->Specular;
                specularFactor = phong->SpecularFactor;

                FbxDouble shiny = phong->Shininess.Get ();
                m.spec_exp = 10*shiny; //because FBX requires multiplied by 10?
            }
        }
        m.ambient = Vector3D(ambient[0], ambient[1], ambient[2]) * (float) ambientFactor;

        m.diffuse = Vector3D(diffuse[0], diffuse[1], diffuse[2]) * (float)diffuseFactor;

        m.specular = Vector3D(specular[0], specular[1], specular[2]) * (float) specularFactor;

        m.emissive = Vector3D(emissive[0], emissive[1], emissive[2]) * (float)emissiveFactor;
        //TODO m.transparency = transparency;

        m.reflective = Vector3D(reflective[0], reflective[1], reflective[2]) * (float) reflectiveFactor;
        m.reflectiveFactor = reflectiveFactor;

        materials.insert(mat, m);
    }

}
