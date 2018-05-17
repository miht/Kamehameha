#include "rasterizer.h"

Rasterizer::Rasterizer(Scene *scene)
    :   Renderer(scene)
{
    mode = Renderer::Rasterizer;
}

QImage Rasterizer::generate(QProgressBar *progress, QImage image) {
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";

    image.fill(QColor(0, 0, 0));

    Matrix4x4 camToWorld = scene->camera.world;
    Matrix4x4 worldToCam;
    Matrix4x4::inverse(camToWorld, worldToCam);

    int zDepthSize = scene->camera.viewportWidth*scene->camera.viewportHeight;
    float depthBuffer[zDepthSize];
    std::fill_n(depthBuffer, zDepthSize, (float) INT_MAX);

    // Iterate over all pixels in image
    for(Face* face : scene->model.root->faces) {
        Material m = scene->model.materials.value (face->material);
        Vertex3D v1 = face->vertices[0];
        Vertex3D v2 = face->vertices[1];
        Vertex3D v3 = face->vertices[2];

        Vector3D p1, p2, p3; //v#Raster
        p1 = Vector3D(worldToViewport (v1.position));
        p2 = Vector3D(worldToViewport (v2.position));
        p3 = Vector3D(worldToViewport (v3.position));

        float cameraZ = (worldToCam * Vector3D(0,0,0)).z;

        p1.z = 1 / -(worldToCam*v1.position).z;
        p2.z = 1 / -(worldToCam*v2.position).z;
        p3.z = 1 / -(worldToCam*v3.position).z;

        Vector2D uv1, uv2, uv3;
        uv1 = v1.uv;
        uv2 = v2.uv;
        uv3 = v3.uv;

        uv1 = uv1 * p1.z;
        uv2 = uv2 * p2.z;
        uv3 = uv3 * p3.z;

        //calculate bounding box
        Vector3D max, min;
        max.x = fmax(p1.x, fmax(p2.x, p3.x));
        max.y = fmax(p1.y, fmax(p2.y, p3.y));
        min.x = fmin(p1.x, fmin(p2.x, p3.x));
        min.y = fmin(p1.y, fmin(p2.y, p3.y));

        // the triangle is out of screen
        if (min.x > image.width () - 1 || max.x < 0 || min.y > image.height ()- 1 || max.y < 0) continue;

        float area = edgeFunction(p1, p2, p3);
        for (int x = std::roundf(min.x); x <= max.x; ++x) {
            for (int y = std::roundf(min.y); y <= max.y; ++y) {
                // the pixel is out of screen
                if (x > image.width () - 1 || x < 0 || y < 0 || y > image.height () - 1)
                    continue;

                Vector3D pixelSample(x, y, 0);

                float w0 = edgeFunction(p2, p3, pixelSample);
                float w1 = edgeFunction(p3, p1, pixelSample);
                float w2 = edgeFunction(p1, p2, pixelSample);

                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    w0 /= area;
                    w1 /= area;
                    w2 /= area;
                    float oneOverZ = p1.z * w0 + p2.z * w1 + p3.z * w2;
                    float z = 1 / oneOverZ;

                    if(z < cameraZ) continue;
                    if (z < depthBuffer[y * image.width ()+ x]) {
                        depthBuffer[y * image.height ()+ x] = z;

                        Vector2D uv = uv1 * w0 + uv2 * w1 + uv3 * w2;

                        uv = uv * z;

                        Vector3D p1Cam, p2Cam, p3Cam;
                        p1Cam = worldToCam * v1.position;
                        p2Cam = worldToCam * v2.position;
                        p3Cam = worldToCam * v3.position;

                        float px = (p1Cam.x/-p1Cam.z) * w0 + (p2Cam.x/-p2Cam.z) * w1 + (p3Cam.x/-p3Cam.z) * w2;
                        float py = (p1Cam.y/-p1Cam.z) * w0 + (p2Cam.y/-p2Cam.z) * w1 + (p3Cam.y/-p3Cam.z) * w2;

                        Vector3D pt(px * z, py * z, -z); // pt is in camera space
                        Vector3D viewDirection = -pt.normalized ();

                        //                        nt = worldToCam * nt;
                        Vector3D nt = v1.normal * w0 + v2.normal * w1 + v3.normal * w2;
                        nt = nt.normalized ();

                        Vector3D n = Vector3D::cross_prod (p2Cam - p1Cam, p3Cam - p1Cam).normalized ();
                        n = nt;

                        pt = camToWorld * pt;

                        Vector3D diffuse, specular;
                        for(Light light : scene->lights) {
                            Vector3D lightDir = (light.position - pt).normalized ();

                            diffuse = diffuse + std::max(0.f, Vector3D::dot_prod (n, lightDir))
                                    * light.intensity * light.color.asVector3D ();

                            Vector3D bisector = (pt + lightDir).normalized();
                            specular = specular + std::powf(fmax(0, Vector3D::dot_prod (bisector, nt)), m.spec_exp)
                                    * light.intensity*light.color.asVector3D ();
                        }

                        float nDotView = std::max(0.f, Vector3D::dot_prod (n, viewDirection));
                        const int M = 10;
                        float checker = (fmod(uv.x * M, 1.0) > 0.5) ^ (fmod(uv.y * M, 1.0) < 0.5);
                        float c = 0.3 * (1 - checker) + 0.7 * checker;
                        //                        nDotView *= c;
                        Vector3D color = diffuse * m.diffuse + specular*m.specular;
                        image.setPixelColor (x, y, Color(color).asQColor ());
                    }
                }
            }
        }
    }
    return image;
}
