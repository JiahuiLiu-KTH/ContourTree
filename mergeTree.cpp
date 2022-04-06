//
// Created by jiahui on 6/4/22.
//

#include <algorithm>
#include "mergeTree.h"

mergeTree::mergeTree() {

}

mergeTree::mergeTree(mesh inputMesh) {

}

void mergeTree::computeJoinTree(mesh inputMesh) {
    int numV = inputMesh.getM() * inputMesh.getN(); // number of vertex
    std::vector<int> comp(numV); //use to keep the union-find set
    //initialize union-find structure
    std::iota(comp.begin(), comp.end(), 0);
    
}

std::vector<int> srotIndex(const std::vector<double> val) {
    std::vector<int> idx(val.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::stable_sort(idx.begin(), idx.end(),[&val](size_t i1, size_t i2) {return val[i1] < val[i2];});

    return idx;
}
