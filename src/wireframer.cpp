#include "wireframer.h"

Wireframer::Wireframer(QWidget *parent, Scene *scene, Settings *settings)
    :   Renderer(parent, scene, settings)
{
    mode = Renderer::Wireframer;
}

QImage Wireframer::generate(QImage image) {
    // Plain PPM format
    //out << "P3\n" << w << ' ' << h << ' ' << "255\n";

    QPainter painter (&image);
    painter.fillRect (0,0,image.width (), image.height (), Qt::black);
    painter.setPen (Qt::white);

    Matrix4x4 camToWorld = scene->camera.world;

    Matrix4x4 worldToCam;
    Matrix4x4::inverse(camToWorld, worldToCam);

    //Transposed camToWorld used for transforming normal vectors
    Matrix4x4 camToWorld_transposed;
    Matrix4x4::transpose(camToWorld, camToWorld_transposed);

    float b, t, l, r;
    float near = scene->camera.clippingPlane[0];
    float far = scene->camera.clippingPlane[1];
    float imageAspectRatio = viewportWidth/(float) viewportHeight;
    if(scene->camera.mode == Camera::perspective) {
        calculatePerspective (scene->camera.angleOfView,
                              imageAspectRatio,
                              near, far, b, t, l, r);
        projMatrix = getPerspectiveFrustrum(b, t, l, r, near, far);
    }
    else {
        calculateOrthographic (scene->camera.angleOfView,
                              imageAspectRatio,
                              near, far, b, t, l, r);

        projMatrix = getOrthographicFrustrum (b, t, l, r, near, far);

        Matrix4x4 m;
        Matrix4x4::transpose (projMatrix, m);
        projMatrix = m;
    }

    // Iterate over all pixels in image
    for(Face* face : scene->model.root->faces) {
        std::vector<Vector2D> points;

        int vertices = face->getPoints ().size();
        for(size_t i = 0; i < vertices; i++) {
            Vector3D pCam = worldToCam * face->getPoints ()[i];
            Vector3D pProj = projMatrix * pCam;
            Vector2D pView = worldToViewport (pProj).asVector2D ();

            points.push_back (pView);
        }

        drawEdges(painter, points);
    }

    painter.end ();
    return image;
}

void Wireframer::drawEdges(QPainter &painter, const std::vector<Vector2D> points) {
    int n = points.size();
    for(int i = 0; i < n; i++) {
        Vector2D p1 = points[i];
        Vector2D p2 = points[(i + 1) % n];

        //  TODO CHECK IF OUTSIDE OF SCREEN BOUNDS
        painter.drawLine(p1.x, p1.y, p2.x, p2.y);
    }
}

