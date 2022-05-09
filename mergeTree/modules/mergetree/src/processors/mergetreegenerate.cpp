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

#include <inviwo/mergetree/processors/mergetreegenerate.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo mergeTreeGenerate::processorInfo_{
    "org.inviwo.mergeTreeGenerate",      // Class identifier
    "merge Tree Generate",                // Display name
    "Merge Tree",              // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};
const ProcessorInfo mergeTreeGenerate::getProcessorInfo() const { return processorInfo_; }

mergeTreeGenerate::mergeTreeGenerate()
    : Processor()
    , inport_("inport")
    , outport_("outport")
    , propNeighbourOffset("neighbourOffset", "Neighbour Offset",
                          {{"2D_4", "2D_4", 1},
                           {"2D_6", "2D_6", 2},
                           {"3D_6", "3D_6", 3}}, 1) {

    addPort(inport_);
    addPort(outport_);
    addProperty(propNeighbourOffset);
}

void mergeTreeGenerate::process() {
    auto vol = inport_.getData();
    auto dims = vol->getDimensions();
//    auto a = vol->getRepresentation<VolumeRAM>();

    std::shared_ptr<mergeTree> newMergeTree;
    int no = propNeighbourOffset.get();
    switch (no) {
        case 1:
        {
            newMergeTree = std::make_shared<mergeTree>(neighbourOffset_2D_4, vol);
            break ;
        }
        case 2:
        {
            newMergeTree = std::make_shared<mergeTree>(vol);
            break ;
        }
        case 3:
        {
            newMergeTree = std::make_shared<mergeTree>(neighbourOffset_3D_6, vol);
            break ;
        }
    }

    newMergeTree->computeJoinTree();

    outport_.setData(newMergeTree);
}

}  // namespace inviwo
