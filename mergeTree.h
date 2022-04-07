//
// Created by jiahui on 6/4/22.
//

#ifndef CONTOURTREE_MERGETREE_H
#define CONTOURTREE_MERGETREE_H

#include <numeric>
#include <vector>
#include <set>
#include <unordered_set>
#include "mesh.h"
#include "unionFind.h"

struct node {
    int id;
    double val;
    std::unordered_set<struct node *> parents;
    std::unordered_set<struct node *> children;

public:
    int getNumParent() const {
        return (int)parents.size();
    }

    int getNumChildren() const {
        return (int)children.size();
    }

    void addParent(struct node * parentNode) {
        if (parentNode->id == this->id) return;
        parents.insert(parentNode);
    }

    void addChildren(struct node * childNode) {
        if (childNode->id == this->id) return;
        children.insert(childNode);
    }
};

class mergeTree {

public:
    mergeTree();
    mergeTree(mesh inputMesh);
    void computeJoinTree(mesh inputMesh);
    static std::vector<int> sortIndex(const std::vector<double> val);
    void addEdgeToJoinTree(int pId, int cId);
    void printJoinTree();

private:
    std::vector<struct node> augmentedJoinTree;
};


#endif //CONTOURTREE_MERGETREE_H
