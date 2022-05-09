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

#include <inviwo/mergetree/utils/unionfind.h>

namespace inviwo {

unionFind::unionFind() {}

unionFind::unionFind(int size) {
    this->parent.resize(size);
    std::iota(this->parent.begin(), this->parent.end(), 0);
}

int unionFind::find(int id) {
    int idx = id;

    // while the set id is not itself, meas it is not the root
    while (parent[idx] != idx) {
        idx = parent[idx];
    }

    // when find the root, compress this search path
    pathCompression(id, idx);
    return idx;
}

void unionFind::pathCompression(int startId, int id) {
    int currentId = startId;
    int nextId;

    // go through the path
    while (currentId != id) {
        nextId = parent[currentId];
        parent[currentId] = id; // all item in this path, set the parent to root
        currentId = nextId;
    }
}

void unionFind::join(int cId, int pId) {
    // find the root of two set
    int cIdRoot = find(cId);
    int pIdRoot = find(pId);

    // union the two set.
    parent[cIdRoot] = pIdRoot;
}

}  // namespace inviwo
