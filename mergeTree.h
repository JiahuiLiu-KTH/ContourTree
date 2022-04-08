//
// Created by jiahui on 6/4/22.
//

#ifndef CONTOURTREE_MERGETREE_H
#define CONTOURTREE_MERGETREE_H

#include <numeric>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
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
    mergeTree(const mesh& inputMesh);
    void computeAugJoinTree(const mesh& inputMesh);
    void computeJoinTree(const mesh& inputMesh);

    static std::vector<int> sortIndex(const std::vector<double> val);
    void addEdgeToJoinTree(int pId, int cId);
    void printJoinTree();

private:
    mesh inputMesh;
    std::vector<struct node> augmentedJoinTree;
    std::unordered_map<int, struct node *> joinTree;

    const int numNeighbour = 6;
    const int neighbours[6][2] = {{-1, -1},
                            {-1, 0},
                            {0,  -1},
                            {0,  1},
                            {1,  0},
                            {1,  1}};
};


#endif //CONTOURTREE_MERGETREE_H
