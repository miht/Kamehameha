#include "rasterizer.h"

Rasterizer::Rasterizer(QWidget *parent, Scene *scene, Settings *settings)
    :   Renderer(parent, scene, settings)
{
    mode = Renderer::Rasterizer;
}

QImage Rasterizer::generate(QImage image) {
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";
    image.fill(QColor(0, 0, 0));

    float b, t, l, r;
    float near = scene->camera.clippingPlane[0];
    float far = scene->camera.clippingPlane[1];
    calculatePerspective (scene->camera.angleOfView,
                          viewportWidth/(float) viewportHeight,
                          near, far, b, t, l, r);
    projMatrix = getPerspectiveFrustrum(b, t, l, r, near, far);

    Matrix4x4 camToWorld = scene->camera.world;

    Matrix4x4 worldToCam;
    Matrix4x4::inverse(camToWorld, worldToCam);

    Matrix4x4 camToWorld_transposed;
    Matrix4x4::transpose(camToWorld, camToWorld_transposed);

    int zDepthSize = image.width () * image.height ();
    float depthBuffer[zDepthSize];
    std::fill_n(depthBuffer, zDepthSize, (float) INT_MAX);

    // Iterate over all pixels in image
    for(Face* face : scene->model.root->faces) {

        Material m = scene->model.materials.value (face->material);
        Vertex3D v1 = face->vertices[0];
        Vertex3D v2 = face->vertices[1];
        Vertex3D v3 = face->vertices[2];

        float cameraZ = (worldToCam * Vector3D(0, 0, 0)).z;

        Vector3D p1, p2, p3;
        p1 = v1.position;
        p2 = v2.position;
        p3 = v3.position;

        Vector3D p1Cam, p2Cam, p3Cam;

        p1Cam = worldToCam * p1;
        p2Cam = worldToCam * p2;
        p3Cam = worldToCam * p3;

        Vector3D n = Vector3D::cross_prod(p2Cam - p1Cam, p3Cam - p1Cam).normalized ();

        Vector3D p1View, p2View, p3View;
        p1View = worldToViewport (projMatrix * p1Cam);
        p2View = worldToViewport (projMatrix * p2Cam);
        p3View = worldToViewport (projMatrix * p3Cam);

        p1View.z = 1 / p1View.z,
        p2View.z = 1 / p2View.z,
        p3View.z = 1 / p3View.z;

        float area = edgeFunction(p1View, p2View, p3View);

        //calculate bounding box
        Vector3D max, min;
        max.x = fmin(viewportWidth - 1, fmax(0, fmax(p1View.x, fmax(p2View.x, p3View.x))));
        max.y = fmin(viewportHeight - 1, fmax(0, fmax(p1View.y, fmax(p2View.y, p3View.y))));
        min.x = fmax(0.0, fmin(p1View.x, fmin(viewportWidth - 1, fmin(p2View.x, p3View.x))));
        min.y = fmax(0.0, fmin(p1View.y, fmin(viewportHeight - 1, fmin(p2View.y, p3View.y))));

        for (int x = (int) min.x; x <= (int) max.x; ++x) {
            for (int y = (int) min.y; y <= (int) max.y; ++y) {
                Vector3D pixelSample(x + 0.5, y + 0.5, 0);

                float w0 = edgeFunction(p2View, p3View, pixelSample);
                float w1 = edgeFunction(p3View, p1View, pixelSample);
                float w2 = edgeFunction(p1View, p2View, pixelSample);

                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    //Intersects triangle
                    w0 /= area;
                    w1 /= area;
                    w2 /= area;
                    float oneOverZ = p1View.z * w0 + p2View.z * w1 + p3View.z * w2;
                    float z = 1 / oneOverZ;

//                    if(z > near) continue;

                    if (z < depthBuffer[y * image.width () + x]) {

                        float px = (p1Cam.x/-p1Cam.z) * w0 + (p2Cam.x/-p2Cam.z) * w1 + (p3Cam.x/-p3Cam.z) * w2;
                        float py = (p1Cam.y/-p1Cam.z) * w0 + (p2Cam.y/-p2Cam.z) * w1 + (p3Cam.y/-p3Cam.z) * w2;

                        Vector3D pt(px * z, py * z, -z); // pt is in camera space

                        depthBuffer[y * image.width () + x] = z;

//                        Vector3D pt = worldToCam * (p1* w0 + p2 * w1 + p3 * w2);
                        Vector3D viewDirection = -pt.normalized ();

//                        if(Vector3D::dot_prod (n, viewDirection) < 0) continue;

                        Vector3D n = Vector3D::cross_prod(p2Cam - p1Cam, p3Cam - p1Cam).normalized ();

                        n = v1.normal.normalized () * w0 + v2.normal.normalized () * w1 + v3.normal.normalized () * w2;
                        n = (camToWorld_transposed * n).normalized ();

                        if(x == 300 && y == 300) {
                            qDebug() << n;
                            qDebug() << Vector3D::dot_prod(n, viewDirection);
                        }

                        float nDotView = std::max(0.f, Vector3D::dot_prod(n, viewDirection));

                        Vector3D color = nDotView * m.diffuse;
//                        color = m.diffuse;
                        image.setPixelColor (x, y, Color(color).asQColor ());
                    }
                }
            }
        }
    }
    return image;
}

