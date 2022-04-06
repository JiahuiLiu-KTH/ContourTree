//
// Created by jiahui on 6/4/22.
//

#ifndef CONTOURTREE_MERGETREE_H
#define CONTOURTREE_MERGETREE_H

#include <numeric>
#include <vector>
#include "mesh.h"

struct node {
    int id;
    std::vector<struct node *> parents;
    std::vector<struct node *> children;

public:
    int getNumParent() const {
        return (int)parents.size();
    }

    int getNumChildren() const {
        return (int)children.size();
    }

    void addParent(struct node * parentNode) {
        parents.push_back(parentNode);
    }

    void addChildren(struct node * childNode) {
        children.push_back(childNode);
    }
};

class mergeTree {

public:
    mergeTree();
    mergeTree(mesh inputMesh);
    void computeJoinTree(mesh inputMesh);

private:
    struct node augmentedJoinTree;
};


#endif //CONTOURTREE_MERGETREE_H
