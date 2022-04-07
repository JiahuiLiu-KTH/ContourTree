#include <iostream>
#include <math.h>
#include <numeric>
#include "mesh.h"
#include "mergeTree.h"

void test() {
//    std::vector<double> value = {2.0, 3.0, 5.0, 1.0};
//    auto res = mergeTree::sortIndex(value);
//    for (int i = 0; i < res.size(); ++i) {
//        std::cout << res[i] << " ";
//    }
//    std::cout << std::endl;
    std::vector<int> a;
    a.resize(10);
    std::cout << a.size() << std::endl;

    a[6] = 9;

    for (int i = 0; i < a.size(); ++i) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
}

int main() {

    std::cout << "Hello, World!" << std::endl;
    mesh testMesh("../data/example1.dat");
    mergeTree testMergeTree(testMesh);
    testMergeTree.printJoinTree();
    return 0;
}