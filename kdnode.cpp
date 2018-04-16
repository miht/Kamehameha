#include "kdnode.h"

KDNode::KDNode()
{
}

bool KDNode::hit(Ray &ray, float &t0, float &t1, Intersection &intersection) {
    if(bbox.intersects (ray, t0, t1, intersection)) {
        if(left->faces.size () > 0 || right->faces.size () > 0) {
            bool hitLeft = left->hit(ray, t0, t1, intersection);
            bool hitRight = right->hit(ray, t0, t1, intersection);
            return hitLeft || hitRight;
        }
        else {
            for(size_t i = 0; i < faces.size(); i++) {
                Intersection intersection2;
                if(faces[i]->intersects (ray, t0, t1, intersection2, true)) {
                    intersection = intersection2;
                }
            }
            return intersection.hit;
        }
        return false;
    }
    return false;
}

KDNode* KDNode::build(std::vector<Face*>& fcs, int depth) {
    KDNode* node = new KDNode();
    node->faces = fcs;
    node->left = NULL;
    node->right = NULL;

    if(fcs.size() == 0) return node;

    if(fcs.size() == 1) {
        node->bbox = fcs[0]->bbox;
        node->left = new KDNode();
        node->right = new KDNode();
        node->left->faces = std::vector<Face*>();
        node->right->faces = std::vector<Face*>();
        return node;
    }

    node->bbox = fcs[0]->bbox;

    for(size_t i = 1; i < fcs.size(); i++) {
        node->bbox.expand(fcs[i]->bbox);
    }

    Vector3D midpt;
    for(size_t i = 0; i < fcs.size(); i++) {
        midpt = midpt + fcs[i]->midpoint/(float)fcs.size();
    }

    std::vector<Face*> left_fcs;
    std::vector<Face*> right_fcs;
    int axis = node->bbox.getLongestAxis ();
    for(size_t i = 0; i < fcs.size(); i++) {
        switch(axis) {
        case 0:
            midpt.x >= fcs[i]->midpoint.x ? right_fcs.push_back (fcs[i]) : left_fcs.push_back (fcs[i]);
            break;
        case 1:
            midpt.y >= fcs[i]->midpoint.y ? right_fcs.push_back (fcs[i]) : left_fcs.push_back (fcs[i]);
            break;
        case 2:
            midpt.z >= fcs[i]->midpoint.z ? right_fcs.push_back (fcs[i]) : left_fcs.push_back (fcs[i]);
            break;
        }
    }

    if(left_fcs.size() == 0 && right_fcs.size() > 0) left_fcs = right_fcs;
    if(right_fcs.size () == 0 && left_fcs.size () > 0) right_fcs = left_fcs;

    int matches = 0;
    for(size_t i = 0; i < left_fcs.size (); i++) {
        for(size_t j = 0; j < right_fcs.size(); j++) {
            if(left_fcs[i] == right_fcs[j]) {
                matches ++;
            }
        }
    }

    if((float) matches / left_fcs.size() < 0.5 && (float) matches / right_fcs.size() < 0.5) {
        node->left = build(left_fcs, depth + 1);
        node->right = build(right_fcs, depth + 1);
    } else {
        node->left = new KDNode();
        node->right = new KDNode();
        node->left->faces = std::vector<Face*>();
        node->right->faces = std::vector<Face*>();
    }

    return node;
}
