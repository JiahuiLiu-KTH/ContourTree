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

double calculateHeight(const int id, const std::unordered_map<int, struct node *>& mergeTree) {
    node* currentNode = mergeTree.at(id);
    node* parent;
    if (mergeTree.at(id)->parents.size() == 1){
        parent = *mergeTree.at(id)->parents.begin();

    } else {
        std::cout << "ERROR: parents container size is not equal to 1!" << std::endl;
        return -1;
    }

    return currentNode->val-parent->val;
}

std::unordered_map<int, struct node *> mergeTree::simplifyMergeTree(const std::unordered_map<int, struct node *> &mergeTree) {

    struct nodeIdAndWeight {
        int nodeId;
        double weight;
    };

    auto simplifiedTree = copyTree(mergeTree);

    // use inQueueCount to keep track of how many times does each node been insert in to the priority queue,
    // so we can know if the popped item is valid.
    std::unordered_map<int, int> inQueueCount;
    for (auto node : simplifiedTree) {
        inQueueCount.insert({node.first, 0});
    }

    // priority queue for prune the edge.
    auto comp = [](nodeIdAndWeight a, nodeIdAndWeight b) {
        return a.weight > b.weight;
    };
    std::priority_queue<nodeIdAndWeight, std::vector<nodeIdAndWeight>, decltype(comp)> pruneQueue(comp);

    // plug any weight function to this.
    double (*calculateWeight)(const int id, const std::unordered_map<int, struct node *>& mergeTree);
    calculateWeight = &calculateHeight;

    // find all leaf nodes and put them into the prune queue
    for(auto p : simplifiedTree) {
        if(p.second->children.empty()) { // if this node does not have children
            nodeIdAndWeight newNIW{p.first, calculateWeight(p.first, simplifiedTree)};
            pruneQueue.push(newNIW);
            inQueueCount[p.first] ++;
        }
    }

    // main loop
    while (pruneQueue.size() > 1) {
        int id = pruneQueue.top().nodeId;
        pruneQueue.pop();
        inQueueCount[id]--;
        if (inQueueCount[id] == 0) { // if this item is valid
            int changedNodeId = prune(id, simplifiedTree);

            // if we have a collapsed node and the node is a leaf node
            if (changedNodeId >= 0 && simplifiedTree[changedNodeId]->children.empty()) {
                nodeIdAndWeight newNIW{changedNodeId, calculateWeight(changedNodeId, simplifiedTree)};
                pruneQueue.push(newNIW);
                inQueueCount[changedNodeId]++;
            }

        }
    }

    return simplifiedTree;
}

std::unordered_map<int, node *> mergeTree::copyTree(std::unordered_map<int, node *> mergeTree) {
    std::unordered_map<int, node *> newTree;
    for(auto p : mergeTree) {
        node * newNode = new node();
        newNode->id = p.first;
        newNode->val = p.second->val;
        newTree.insert({p.first, newNode});
    }

    for(auto p : mergeTree) {
        auto node = newTree[p.first];
        for(auto parent : p.second->parents) {
            node->addParent(newTree[parent->id]);
        }
        for(auto child : p.second->children) {
            node->addChildren(newTree[child->id]);
        }
    }

    return newTree;
}

int mergeTree::prune(int nodeId, std::unordered_map<int, struct node *> &mergeTree) {
    int changedNodeId = -1;
    auto currentNode = mergeTree[nodeId];
    auto parentNode = *currentNode->parents.begin();

    parentNode->children.erase(currentNode);
    mergeTree.erase(nodeId);
    delete currentNode;

    // if there is only one child left on the parent node,
    // link the child node and grandparent node and delete the parent node.
    if (parentNode->children.size() == 1) {
        auto grandparentNode = *parentNode->parents.begin();
        auto childNode =*parentNode->children.begin();

        childNode->addParent(grandparentNode);
        childNode->parents.erase(parentNode);
        grandparentNode->addChildren(childNode);
        grandparentNode->children.erase(parentNode);

        mergeTree.erase(parentNode->id);
        delete parentNode;

        changedNodeId = childNode->id;
    }


    return changedNodeId;
}

void mergeTree::test() {
    auto a = simplifyMergeTree(this->joinTree);
    for(auto p : a) {
        std::cout << "Node value: " << p.second->val << std::endl;
        std::cout << "Number of parents: " << p.second->parents.size() << std::endl;
        for(auto par : p.second->parents){
            std::cout << par->val << " ";
        }
        std::cout << std::endl;
        std::cout << "Number of children: " << p.second->children.size() << std::endl;
        for(auto par : p.second->children){
            std::cout << par->val << " ";
        }
        std::cout << std::endl;
        std::cout << "---------------------" << std::endl;
    }
}
