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
    std::unordered_set<int> uoset = {1,2,3,4};
    auto rtn = uoset.insert(3);
    std::cout << rtn.second << std::endl;
    rtn = uoset.insert(6);
    std::cout << rtn.second << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    mesh testMesh("../data/example1.dat");
    mergeTree testMergeTree(testMesh);
    testMergeTree.printJoinTree();

    return 0;
}