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

#include <inviwo/mergetree/mergetreemodule.h>
#include <inviwo/mergetree/processors/mergetreegenerate.h>
#include <inviwo/mergetree/processors/mergetreesimplification.h>
#include <inviwo/mergetree/processors/mergetreevis.h>

namespace inviwo {

mergeTreeModule::mergeTreeModule(InviwoApplication* app) : InviwoModule(app, "mergeTree") {
    // Add a directory to the search path of the Shadermanager
    // ShaderManager::getPtr()->addShaderSearchPath(getPath(ModulePath::GLSL));

    // Register objects that can be shared with the rest of inviwo here:

    // Processors
    registerProcessor<mergeTreeGenerate>();
    // registerProcessor<mergeTreeProcessor>();
    registerProcessor<mergeTreeSimplification>();
    registerProcessor<mergeTreeVis>();

    // Properties
    // registerProperty<mergeTreeProperty>();

    // Readers and writes
    // registerDataReader(std::make_unique<mergeTreeReader>());
    // registerDataWriter(std::make_unique<mergeTreeWriter>());

    // Data converters
    // registerRepresentationConverter(std::make_unique<mergeTreeDisk2RAMConverter>());

    // Ports
     registerPort<MergeTreeInport>();
     registerPort<MergeTreeInport>();

    // PropertyWidgets
    // registerPropertyWidget<mergeTreePropertyWidget, mergeTreeProperty>("Default");

    // Dialogs
    // registerDialog<mergeTreeDialog>(mergeTreeOutport);

    // Other things
    // registerCapabilities(std::make_unique<mergeTreeCapabilities>());
    // registerSettings(std::make_unique<mergeTreeSettings>());
    // registerMetaData(std::make_unique<mergeTreeMetaData>());
    // registerPortInspector("mergeTreeOutport", "path/workspace.inv");
    // registerProcessorWidget(std::string processorClassName, std::unique_ptr<ProcessorWidget> processorWidget); 
    // registerDrawer(util::make_unique_ptr<mergeTreeDrawer>());
}

}  // namespace inviwo
