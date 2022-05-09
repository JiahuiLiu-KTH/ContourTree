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

#include <inviwo/mergetree/processors/mergetreesimplification.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo mergeTreeSimplification::processorInfo_{
    "org.inviwo.mergeTreeSimplification",      // Class identifier
    "merge Tree Simplification",                // Display name
    "Merge Tree",              // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};
const ProcessorInfo mergeTreeSimplification::getProcessorInfo() const { return processorInfo_; }

mergeTreeSimplification::mergeTreeSimplification()
    : Processor()
    , inport_("inport")
    , outport_("outport")
    , propThreshold("threshold", "Threshold", 0.0, 0.0, 10.0) {

    addPort(inport_);
    addPort(outport_);
    addProperty(propThreshold);
}

void mergeTreeSimplification::process() {
    auto mt = inport_.getData().get();

    //copy input merge tree to a new merge tree
    std::shared_ptr simpMt = std::make_shared<mergeTree>(mt->getNeighbourOffset(), std::make_shared<Volume>(mt->getVolume()));
    auto tree = mt->getJoinTree();
    simpMt->setJoinTree(tree);

    simpMt->simplifyMergeTree(propThreshold.get());
    LogProcessorInfo("");
    LogProcessorInfo("Original tree size:   " << mt->getJoinTree().size());
    LogProcessorInfo("Simplified tree size: " << simpMt->getJoinTree().size());
    std::cout << "simplified tree size: " << simpMt->getJoinTree().size() << std::endl;

    outport_.setData(simpMt);
}

}  // namespace inviwo
