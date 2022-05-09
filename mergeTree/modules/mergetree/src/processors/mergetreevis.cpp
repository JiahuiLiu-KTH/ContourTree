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

#include <inviwo/mergetree/processors/mergetreevis.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo mergeTreeVis::processorInfo_{
    "org.inviwo.mergeTreeVis",      // Class identifier
    "merge Tree Vis",                // Display name
    "Merge Tree",              // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};
const ProcessorInfo mergeTreeVis::getProcessorInfo() const { return processorInfo_; }

mergeTreeVis::mergeTreeVis()
    : Processor()
    , inport_("inport")
    , outport_("outport")
    , scale_("scale", "Scale", vec3(1.0f), vec3(0.0f), vec3(10.0f)) {

    addPort(inport_);
    addPort(outport_);
    addProperty(scale_);
}

void drawLineSegment(const vec3& v1, const vec3& v2, IndexBufferRAM* indexBuffer, Vec3BufferRAM* vertices) {
    indexBuffer->add(static_cast<std::uint32_t>(vertices->getSize()));
    vertices->add(v1);
    indexBuffer->add(static_cast<std::uint32_t>(vertices->getSize()));
    vertices->add(v2);
}

void mergeTreeVis::process() {
    auto mt = inport_.getData().get();

    // create a mesh
    auto newMesh = std::make_shared<Mesh>(DrawType::NumberOfDrawTypes, ConnectivityType::NumberOfConnectivityTypes);

    // vertex
    auto vertexBuffer = std::make_shared<Buffer<vec3>>();
    auto vertices = vertexBuffer->getEditableRAMRepresentation();

    // line
    auto lineIndexBuffer = std::make_shared<IndexBuffer>();
    auto lineIndices = lineIndexBuffer->getEditableRAMRepresentation();

    // add vertex to mesh
    newMesh->addBuffer(BufferType::PositionAttrib, vertexBuffer);

    // add lin to mesh
    newMesh->addIndices(Mesh::MeshInfo(DrawType::Lines, ConnectivityType::None), lineIndexBuffer);

    auto volume = mt->getVolume();
    auto tree = mt->getJoinTree();
    auto coord = volume.getRepresentation<VolumeRAM>()->dispatch<std::unordered_map<int, vec3>, dispatching::filter::Scalars>([&tree, this](auto vrPrecision) {
        int rtn = 0;
        using T = util::PrecisionValueType<decltype(vrPrecision)>;
        const T* data = vrPrecision->getDataTyped();

        // figure out who is the root and how many leaf nodes
        T max = *(data);
        T min = max;
        int root;
        int numLeafNode = 0;
        for (auto p:tree) {
            auto value = *(data+p.first);

            if (value > max) max = value;
            if (value < min) min = value;

            if (p.second.isRoot()) root = p.first;

            if(p.second.isLeaf()) numLeafNode++;
        }

        // create coordinate for nodes
        std::unordered_map<int, vec3> coordinates;
        coordinates.reserve(tree.size());

//        // space
//        std::unordered_map<int, vec2> space;
//        space.reserve(tree.size());

        // try to build a dfs traversal so that we can build the tree
        std::queue<int> dfsQueue;
        std::stack<int> dfsStack;

        dfsQueue.push(root);

        // build dfs traversal order
        while (!dfsQueue.empty()) {
            int newNode = dfsQueue.front();
            dfsQueue.pop();
            dfsStack.push(newNode);

            for (auto cNode:tree[newNode].children) {
                dfsQueue.push(cNode);
            }
        }

        // traversal all node in dfs order
        int xMax = 0;
        if (numLeafNode <= 1) numLeafNode++;

        while (!dfsStack.empty()) {
            int nodeIdx = dfsStack.top();
            dfsStack.pop();
            float x,y;
            if (tree[nodeIdx].isLeaf()) { // leaf node will use a space
                x = (xMax*1.0)/(numLeafNode-1);
                xMax++;
//                space.insert({nodeIdx, vec2(x, x)});
            } else { // non-leaf node will be in the middle of all children node
                double sum = 0.0;
//                float left = 1.0, right = 0.0;
                for (auto chi:tree[nodeIdx].children) {
                    sum += coordinates[chi].x;
//                    left = std::min(left, space[chi].x);
//                    right = std::max(right, space[chi].y);
                }
                x = sum/(double)(tree[nodeIdx].children.size());
//                space.insert({nodeIdx, vec2(left, right)});
//                x = (left + right)/2;
            }

            y = (*(data + nodeIdx)-min)*1.0/(max-min);

            coordinates.insert({nodeIdx, vec3(x, y, 0)});
        }

        return coordinates;
    });

    // scale
    for (auto &c:coord) {
        c.second.x *= scale_.get().x;
        c.second.y *= scale_.get().y;
    }

    for (auto p:tree) {
        for (auto chi:p.second.children) {
            drawLineSegment(coord[p.first], coord[chi],lineIndices, vertices);
        }
    }

     outport_.setData(newMesh);
}

}  // namespace inviwo
