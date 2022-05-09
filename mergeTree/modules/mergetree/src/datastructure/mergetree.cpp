/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2022 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include <inviwo/mergetree/datastructure/mergetree.h>

namespace inviwo {

mergeTree::mergeTree(const std::shared_ptr<const Volume>& inputVolume)
    : neighbourOffset(neighbourOffset_2D_6) {
    volume = *inputVolume;
}

mergeTree::mergeTree(std::vector<size3_t> neighbourOffset, const std::shared_ptr<const Volume>& inputVolume)
    : neighbourOffset(neighbourOffset) {
    volume = *inputVolume;
}

void mergeTree::test() { computeJoinTree(); }

void mergeTree::computeJoinTree() {

    auto joinTreeSize =
        this->volume.getRepresentation<VolumeRAM>()
            ->dispatch<int, dispatching::filter::Scalars>([this](auto vrPrecision) {
                // get the data type
                using T = util::PrecisionValueType<decltype(vrPrecision)>;

                // calculate the total number of vertex
                size3_t dims = vrPrecision->getDimensions();
                int numV = dims.x * dims.y * dims.z;

                // initialize unionFind
                unionFind jtUnionFind(numV);

                // initialize the index list
                std::vector<int> indexList(numV);
                std::iota(indexList.begin(), indexList.end(), 0);

                // get data
                const T* data = vrPrecision->getDataTyped();

                // sort the index according to the data value (descending order)
                std::stable_sort(indexList.begin(), indexList.end(), [&data](size_t i1, size_t i2) {
                    return *(data + i1) < *(data + i2);  // ascending order
                });
                std::reverse(indexList.begin(), indexList.end());

                //                printOut(indexList, data);

                // traversal of all vertices in descending order
                for (int v = 0; v < indexList.size(); ++v) {
                    int vertexIdx = indexList[v];                   // vertex index
                    size3_t vertexPos = idxToPos(vertexIdx, dims);  // vertex position

                    std::unordered_set<int> neighbourComponent;

                    for (auto offset : neighbourOffset) {
                        size3_t neighbourPos = vertexPos + offset;

                        // if neighbours' position is valid
                        if (inBox(neighbourPos, {0, 0, 0}, dims)) {
                            int neighbourIdx = posToIdx(neighbourPos, dims);

                            // if neighbour value >= vertex value, which means the neighbour is already in a component
                            if (*(data + neighbourIdx) >= *(data + vertexIdx)) {
                                int componentIdx = jtUnionFind.find(neighbourIdx);
                                neighbourComponent.insert(componentIdx);
                            }
                        }
                    }  // END traversal of all neighbours

                    // now we collect all the neighbour components
                    int numNeiComp = neighbourComponent.size();
                    switch (numNeiComp) {
                        case 0:  // there is no neighbour component, which means it is a local extrema
                        {
                            node newNode{vertexIdx};                // create new node
                            joinTree.insert({vertexIdx, newNode});  // add this node to join tree
                            break;
                        }
                        case 1:  // there is 1 neighbour component, extend the component (except the global extrema)
                        {
                            int component = *neighbourComponent.begin(); // the only component
                            if (v == indexList.size() - 1) {  // if this is the last vertex (global extrema)
                                node newNode{vertexIdx};  // create a new node

                                // link the component root node and the new node
                                joinTree[component].addParent(vertexIdx);
                                newNode.addChildren(component);

                                joinTree.insert({vertexIdx, newNode});  // add this node to join tree

                                jtUnionFind.join(component, vertexIdx);  // set component root to the new vertex
                            } else {                        // regular vertex, extend the component
                                jtUnionFind.join(vertexIdx, component);
                            }
                            break;
                        }
                        default:  // there are more than 1 neighbour component, which means it is a saddle
                        {
                            node newNode{vertexIdx}; // create a new node

                            for (auto component:neighbourComponent) {

                                // link all component to the new vertex
                                joinTree[component].addParent(vertexIdx);
                                newNode.addChildren(component);

                                jtUnionFind.join(component, vertexIdx); // set all component root to the new vertex
                            }
                            joinTree.insert({vertexIdx, newNode});
                        }
                    }

                }  // END traversal of all vertices

                return joinTree.size();
            });

//    printJoinTree();
    std::cout << "Join tree size: " << joinTreeSize << std::endl;
}

int mergeTree::prune(int nodeId) {
    int changedNodeId = -1;

    auto node = &joinTree[nodeId];
    auto parentNode = &joinTree[*node->parents.begin()];

    // delete the node
    parentNode->children.erase(nodeId);
    joinTree.erase(nodeId);

    // if there is only one child left on the parent node,
    // link the child node and grandparent node and delete the parent node.
    if (parentNode->children.size() == 1) {
        auto grandparentNode = &joinTree[*parentNode->parents.begin()];
        auto childNode = &joinTree[*parentNode->children.begin()];

        // link between grandparent node and child node
        childNode->addParent(grandparentNode->id);
        grandparentNode->addChildren(childNode->id);

        // delete parent node
        childNode->parents.erase(parentNode->id);
        grandparentNode->children.erase(parentNode->id);
        joinTree.erase(parentNode->id);
//        delete parentNode;

        changedNodeId = childNode->id;
    }

    return changedNodeId;
}

double mergeTree::calculateWeight(const int nodeId) {
    auto weight =
        this->volume.getRepresentation<VolumeRAM>()
            ->dispatch<double, dispatching::filter::Scalars>([this, &nodeId](auto vrPrecision) {
                using T = util::PrecisionValueType<decltype(vrPrecision)>;
                const T* data = vrPrecision->getDataTyped();

                auto node = joinTree[nodeId];
                auto parentNode = joinTree[*node.parents.begin()];
                double weight = *(data+node.id) - *(data+parentNode.id);
                return weight;
    });
    return weight;
}

void mergeTree::simplifyMergeTree(const double threshold) {

    struct nodeIdAndWeight {
        int nodeId;
        double weight;
    };

    // use inQueueCount to keep track of how many times does each node been insert in to the priority queue,
    // so we can know if the popped item is valid.
    std::unordered_map<int, int> inQueueCount;
    inQueueCount.reserve(joinTree.size());
    for (const auto& node : joinTree) {
        inQueueCount.insert({node.first, 0});
    }

    // priority queue for prune the edge.
    auto comp = [](nodeIdAndWeight a, nodeIdAndWeight b) {
        return a.weight > b.weight;
    };
    std::priority_queue<nodeIdAndWeight, std::vector<nodeIdAndWeight>, decltype(comp)> pruneQueue(comp);

    // find all leaf nodes and put them into the prune queue
    for (auto p:joinTree) {
        if (p.second.isLeaf()) {
            nodeIdAndWeight newNIW{p.first, calculateWeight(p.first)};
            pruneQueue.push(newNIW);
            inQueueCount[p.first] ++;
        }
    }

    // main loop
    double currentWeight = pruneQueue.top().weight;
    while (pruneQueue.size() > 1 && currentWeight < threshold) {
//        std::cout << "currentWeight: " << currentWeight << ", threshold: " << threshold << std::endl;
        int id = pruneQueue.top().nodeId;
        pruneQueue.pop();
        currentWeight = pruneQueue.top().weight;
        inQueueCount[id]--;
        if (inQueueCount[id] == 0) { // if this item is valid
            int changedNodeId = prune(id);

            // if we have a collapsed node and the node is a leaf node
            if (changedNodeId >= 0 && joinTree[changedNodeId].isLeaf()) {
                nodeIdAndWeight newNIW{changedNodeId, calculateWeight(changedNodeId)};
                pruneQueue.push(newNIW);
                inQueueCount[changedNodeId]++;
            }
        }
    }
}

template <typename T>
void mergeTree::printOut(const std::vector<int>& orderOffset, T* start) {
    for (auto offset : orderOffset) {
        std::cout << (double)*(start + offset) << std::endl;
    }
}

size3_t mergeTree::idxToPos(int index, size3_t dims) {
    return {index % dims.x, (index / dims.x) % dims.y, index / (dims.x * dims.y)};
}

int mergeTree::posToIdx(size3_t position, size3_t dims) {
    return position.z * dims.x * dims.y + position.y * dims.x + position.x;
}
bool mergeTree::inBox(size3_t pos, size3_t min, size3_t max) {
    if (pos.x >= min.x && pos.x < max.x) {
        if (pos.y >= min.y && pos.y < max.y) {
            if (pos.z >= min.z && pos.z < max.z) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void mergeTree::printJoinTree() const {
    for (auto p:joinTree) {
        auto values = volume.getRepresentation<VolumeRAM>();
        auto dims = volume.getDimensions();
        std::cout << "---------------------------------" << std::endl;
        std::cout << "Node: " << values->getAsDouble(idxToPos(p.first,dims)) << std::endl;
        std::cout << "Parent: ";
        for (auto par:p.second.parents) {
            std::cout << values->getAsDouble(idxToPos(par,dims)) << " ";
        }
        std::cout << std::endl;
        std::cout << "Children: ";
        for (auto chi:p.second.children) {
            std::cout << values->getAsDouble(idxToPos(chi,dims)) << " ";
        }
        std::cout << std::endl;
    }
}

std::string mergeTree::Info() const {
    std::cout << "Merge Tree:" << std::endl;
    std::cout << "Number of tree node: " << this->joinTree.size() << std::endl;
    for (auto p : this->joinTree) {
        std::cout << "-------------------------------" << std::endl;
        std::cout << "ID: " << p.second.id << std::endl;

        std::cout << "Number of Parent: " << p.second.parents.size() << std::endl;
        for (auto par : p.second.parents) {
            std::cout << par << " ";
        }
        std::cout << std::endl;

        std::cout << "Number of Children: " << p.second.children.size() << std::endl;
        for (auto par : p.second.children) {
            std::cout << par << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    return "This is a merge tree.\n";
}

const std::unordered_map<int, mergeTree::node>& mergeTree::getJoinTree() const { return joinTree; }

const Volume& mergeTree::getVolume() const { return volume; }
const std::vector<size3_t>& mergeTree::getNeighbourOffset() const { return neighbourOffset; }
void mergeTree::setJoinTree(const std::unordered_map<int, node>& joinTree) {
    mergeTree::joinTree = joinTree;
}

}  // namespace inviwo
