#ifndef KDNODE_H
#define KDNODE_H

#include <vector>
#include <QDebug>

#include "boundingbox.h"
#include "face.h"

class KDNode
{

public:
    BoundingBox bbox;
    KDNode* left;
    KDNode* right;
    std::vector<Face*> faces;

    KDNode();

    bool hit(Ray &ray, float &t0, float &t1, Intersection &intersection);

    static KDNode* build(std::vector<Face*> &fcs, int depth);
};

#endif // KDNODE_H
