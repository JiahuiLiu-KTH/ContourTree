//
// Created by jiahui on 6/4/22.
//

#include <algorithm>
#include <cmath>
#include "mergeTree.h"

mergeTree::mergeTree() {

}

mergeTree::mergeTree(const mesh& inputMesh) {
    this->inputMesh = inputMesh;
//    computeAugJoinTree(inputMesh);
    computeJoinTree(inputMesh);
}

void mergeTree::computeJoinTree(const mesh& inputMesh) {

    int numV = inputMesh.getM() * inputMesh.getN(); // number of vertex
    unionFind unionFind(numV);

    //sort vertex in descending order.
    std::vector<int> descendingIndex = sortIndex(inputMesh.getMeshVal());
    std::reverse(descendingIndex.begin(), descendingIndex.end());

    for (int v = 0; v < descendingIndex.size(); ++v) {
        int idx = descendingIndex[v];
        int i,j; // 2d index for vertex
        i = floor(idx / inputMesh.getN());
        j = idx % inputMesh.getN();

        //neighbour component
        std::unordered_set<int> neiComp;

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
                neiComp.insert(u);
            }
        }
        int numNeiComp = neiComp.size();
        switch (numNeiComp) {
            case 0:
            {
                node * newNode = new node();
                newNode->id = idx;
                newNode->val = inputMesh.getMeshVal()[idx];
                joinTree.insert({idx, newNode});

                unionFind.newComp(idx);

                break;
            }
            case 1:
            {
                if (v == descendingIndex.size()-1) { //the last one
                    node * newNode = new node();
                    newNode->id = idx;
                    newNode->val = inputMesh.getMeshVal()[idx];


                    unionFind.newComp(idx);

                    unionFind.join(*neiComp.begin(), idx);

                    joinTree[*neiComp.begin()]->addParent(newNode);
                    newNode->addChildren(joinTree[*neiComp.begin()]);
                    joinTree.insert({idx, newNode});

                } else {
                    unionFind.newComp(idx);
                    unionFind.join(idx, *neiComp.begin());
                }
                break;
            }
            default:
            {
                node * newNode = new node();
                newNode->id = idx;
                newNode->val = inputMesh.getMeshVal()[idx];

                unionFind.newComp(idx);
                for (int nei:neiComp) {
                    unionFind.join(nei, idx);
                    joinTree[nei]->addParent(newNode);
                    newNode->addChildren(joinTree[nei]);
                }
                joinTree.insert({idx, newNode});
            }
        }
    }
}

void mergeTree::computeAugJoinTree(const mesh& inputMesh) {

    int numV = inputMesh.getM() * inputMesh.getN(); // number of vertex
    unionFind unionFind(numV);
    unionFind.initialize();

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

        printf("parents:\n");
        for (auto v : augmentedJoinTree[i].parents) {
            printf("id: %d, value:%3.0f|", v->id, v->val);
        }

        printf("\nchildren:\n");
        for (auto v : augmentedJoinTree[i].children) {
            printf("id: %d, value:%3.0f|", v->id, v->val);
        }

        std::cout << "\n--------------------------------" << std::endl;
    }
}
