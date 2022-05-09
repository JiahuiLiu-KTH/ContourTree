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
#pragma once

#include <inviwo/mergetree/mergetreemoduledefine.h>
#include <inviwo/core/datastructures/datatraits.h>
#include <unordered_set>
#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <utility>
#include <queue>
#include <limits>
#include "inviwo/core/datastructures/volume/volumeram.h"
#include "inviwo/mergetree/utils/unionfind.h"

namespace inviwo {

// preset neighbour offset
const std::vector<size3_t> neighbourOffset_2D_4 = {{-1, 0, 0},  // left
                                                   {0, -1, 0},  // up
                                                   {1, 0, 0},   // right
                                                   {0, 1, 0}};  // down

const std::vector<size3_t> neighbourOffset_2D_6 = {{-1, -1, 0},  // top-left
                                                   {-1, 0, 0},   // left
                                                   {0, -1, 0},   // down
                                                   {0, 1, 0},    // up
                                                   {1, 0, 0},    // right
                                                   {1, 1, 0}};   // bottom-right

const std::vector<size3_t> neighbourOffset_3D_6 = {{0, 1, 0},   // front
                                                   {0, -1, 0},  // back
                                                   {0, 0, 1},   // up
                                                   {0, 0, -1},  // down
                                                   {-1, 0, 0},  // left
                                                   {1, 0, 0}};  // right

/**
 * \brief Describes a merge tree.
 * \author Jiahui Liu
 */
class IVW_MODULE_MERGETREE_API mergeTree {

    // constructor and destructor
public:
    /// Default constructor
    mergeTree(const std::shared_ptr<const Volume>& inputVolume);

    /// Initialize the merge tree with dimension, number of neighbours, neighbour offset, and input
    /// volume
    mergeTree(std::vector<size3_t> neighbourOffset, const std::shared_ptr<const Volume>& inputVolume);

    virtual ~mergeTree() = default;

    // members
public:
    /// Single node in the merge tree
    struct node {
        int id;
        std::unordered_set<int> parents;
        std::unordered_set<int> children;

    public:
        void addParent(int parentNode) {
            if (parentNode == this->id) return;
            parents.insert(parentNode);
        }

        void addChildren(int childNode) {
            if (childNode == this->id) return;
            children.insert(childNode);
        }

        bool isLeaf() const {
            if (children.empty()) {
                return true;
            } else {
                return false;
            }
        }

        bool isRoot() const {
            if (parents.empty()) {
                return true ;
            } else {
                return false ;
            }
        }
    };  // end struct node

private:
    /// Augmented Join Tree
    std::vector<node> augmentedJoinTree;

    /// Join Tree
    std::unordered_map<int, node> joinTree;

    /// Offset of neighbours
    const std::vector<size3_t> neighbourOffset;

    /// Input volume
    Volume volume;

//getter and setter
public:
    const std::vector<size3_t>& getNeighbourOffset() const;
    const std::unordered_map<int, node>& getJoinTree() const;
    const Volume& getVolume() const;

    void setJoinTree(const std::unordered_map<int, node>& joinTree);

// Functions
public:

    /// Compute the join tree
    void computeJoinTree();

    void simplifyMergeTree(const double threshold = 0.0);

    /// Convert 1D index to 3D position
    static size3_t idxToPos(int index, size3_t dims);

    /// Convert 3D position to 1D index
    static int posToIdx(size3_t position, size3_t dims);

    /// Check if the position is in the boundingBox
    static bool inBox(size3_t pos, size3_t min = {0,0,0}, size3_t max = {1,1,1});

    /// Print out the join tree
    void printJoinTree() const;

    /// Test function
    void test();

    /// Print out the value according to offset and pointer in certain order
    template <typename T>
    void printOut(const std::vector<int>& orderOffset, T* start);

    /// Print out some information
    std::string Info() const;

private:

    /// Delete node from merge tree according to the ID
    int prune(int nodeId);

    /// calculate weight of the node
    double calculateWeight(const int nodeId);
};

template <>
struct DataTraits<mergeTree> {
    static const std::string& classIdentifier() {
        static const std::string id{"org.inviwo.mergeTree"};
        return id;
    }
    static const std::string& dataName() {
        static const std::string name{"mergeTree"};
        return name;
    }
    static uvec3 colorCode() { return uvec3{0, 255, 0}; }
    static Document info(const mergeTree& data) {
        Document doc;
        doc.append("p", data.Info());
        return doc;
    }
};

}  // namespace inviwo
