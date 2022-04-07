//
// Created by jiahui on 6/4/22.
//

#include <algorithm>
#include <cmath>
#include "mergeTree.h"

mergeTree::mergeTree() {

}

mergeTree::mergeTree(mesh inputMesh) {
    computeJoinTree(inputMesh);
}

void mergeTree::computeJoinTree(mesh inputMesh) {
    int numNeighbour = 6;
    int neighbours[6][2] = {{-1, -1},
                            {-1, 0},
                            {0,  -1},
                            {0,  1},
                            {1,  0},
                            {1,  1}};

    int numV = inputMesh.getM() * inputMesh.getN(); // number of vertex
    unionFind unionFind(numV);

    this->augmentedJoinTree.resize(numV);

    //sort vertex in descending order.
    std::vector<int> descendingIndex = sortIndex(inputMesh.getMeshVal());
    std::reverse(descendingIndex.begin(), descendingIndex.end());

    for (int idx : descendingIndex) {
        int i,j; // 2d index for vertex
        i = floor(idx / inputMesh.getN());
        j = idx % inputMesh.getN();

        struct node newNode;
        newNode.id = idx;
        newNode.val = inputMesh.getMeshVal()[idx];
        augmentedJoinTree[idx] = newNode;

        for (int n = 0; n < numNeighbour; ++n) {
            int ni = i + neighbours[n][0];
            if (ni < 0 || ni >= inputMesh.getM()) {
                continue;
            }
            int nj = j + neighbours[n][1];
            if (nj < 0 || nj >= inputMesh.getN()) {
                continue;
            }

            int nIdx = ni * inputMesh.getN() + nj;

            if (inputMesh.getMeshVal()[nIdx] >= inputMesh.getMeshVal()[idx]) {
                int u = unionFind.find(nIdx);
                addEdgeToJoinTree(idx, u);
                unionFind.join(nIdx,idx);
            }
        }
    }

}

std::vector<int> mergeTree::sortIndex(const std::vector<double> val) {
    std::vector<int> idx(val.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::stable_sort(idx.begin(), idx.end(),
                     [&val](size_t i1, size_t i2) { return val[i1] < val[i2]; } // sort ascending
    );

    return idx;
}

void mergeTree::addEdgeToJoinTree(int pId, int cId) {
    struct node * parentNode = &augmentedJoinTree[pId];
    struct node * childNode = &augmentedJoinTree[cId];
    parentNode->addChildren(childNode);
    childNode->addParent(parentNode);
}

void mergeTree::printJoinTree() {
    for (int i = 0; i < augmentedJoinTree.size(); ++i) {
        printf("ID: %2d, Value: %3.0f\n", augmentedJoinTree[i].id, augmentedJoinTree[i].val);
        printf("number of parents: %d, number of children: %d\n", augmentedJoinTree[i].parents.size(), augmentedJoinTree[i].children.size());
//        for (int j = 0; j < augmentedJoinTree[i].parents.size(); ++j) {
//            std::cout << "parent" << std::endl;
//            printf("parent id: %2d, value :%3.0f\n", augmentedJoinTree[i].parents[j]->id, augmentedJoinTree[i].parents[j]->val);
//        }
//        for (int j = 0; j < augmentedJoinTree[i].children.size(); ++j) {
//            std::cout << "child" << std::endl;
//            printf("parent id: %2d, value :%3.0f\n", augmentedJoinTree[i].children[j]->id, augmentedJoinTree[i].children[j]->val);
//        }
        std::cout << "--------------------------------" << std::endl;
    }
}
