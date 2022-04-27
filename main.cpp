#include <iostream>
#include <math.h>
#include <numeric>
#include <unordered_set>
#include "mesh.h"
#include "mergeTree.h"

/**
                |     set             | unordered_set
---------------------------------------------------------
Ordering        | increasing  order   | no ordering
                | (by default)        |

Implementation  | Self balancing BST  | Hash Table
                | like Red-Black Tree |

search time     | log(n)              | O(1) -> Average
                |                     | O(n) -> Worst Case

Insertion time  | log(n) + Rebalance  | Same as search

Deletion time   | log(n) + Rebalance  | Same as search
 */

void test() {
//    std::vector<double> value = {2.0, 3.0, 5.0, 1.0};
//    auto res = mergeTree::sortIndex(value);
//    for (int i = 0; i < res.size(); ++i) {
//        std::cout << res[i] << " ";
//    }
//    std::cout << std::endl;
//-----------------------------------------------------------
//    std::unordered_set<int> uoset = {1,2,3,4};
//    auto rtn = uoset.insert(3);
//    std::cout << rtn.second << std::endl;
//    rtn = uoset.insert(6);
//    std::cout << rtn.second << std::endl;
//-----------------------------------------------------------
    std::unordered_map<int, int> inQueueCount;
    for (int i = 0; i < 10; ++i) {
        inQueueCount.insert({i, i});
    }
    inQueueCount[0]++;
    std::cout << inQueueCount[0] << std::endl;
}

void test2() {
    struct nodeIdAndWeight {
        int nodeId;
        double weight;
    };

    auto comp = [](nodeIdAndWeight a, nodeIdAndWeight b) {
        return a.weight > b.weight;
    };
    std::priority_queue<nodeIdAndWeight, std::vector<nodeIdAndWeight>, decltype(comp)> pruneQueue(comp);

    nodeIdAndWeight newNIW2{3, 3.0};
    nodeIdAndWeight newNIW0{1, 1.0};
    nodeIdAndWeight newNIW3{4, 4.0};
    nodeIdAndWeight newNIW1{2, 2.0};
    pruneQueue.push(newNIW0);
    pruneQueue.push(newNIW1);
    pruneQueue.push(newNIW2);
    pruneQueue.push(newNIW3);

    while(!pruneQueue.empty()) {
        auto i = pruneQueue.top();
        std::cout << i.nodeId << std::endl;
        pruneQueue.pop();
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    mesh testMesh("../data/example1.dat");
    mergeTree testMergeTree(testMesh);
    testMergeTree.test();
    testMergeTree.printJoinTree();

    return 0;
}