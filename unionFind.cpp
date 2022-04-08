//
// Created by jiahui on 7/4/22.
//

#include "unionFind.h"

unionFind::unionFind(int size) {
    this->comp.resize(size);
    std::fill(comp.begin(), comp.end(), -1);
}

int unionFind::find(int id) {
    int idx = id;
    while (comp[idx] != idx) {
        idx = comp[idx];
    }
    pathComp(id, idx);
    return idx;
}

void unionFind::pathComp(int startId, int id) {
    int currentId = startId;
    int nextId;
    while (currentId != id) {
        nextId = comp[currentId];
        comp[currentId] = id;
        currentId = nextId;
    }
}

void unionFind::join(int cId, int pId) {
    int originalId1 = find(cId);
    int originalId2 = find(pId);

    comp[originalId1] = originalId2;
}

void unionFind::initialize() {
    std::iota(comp.begin(), comp.end(), 0);
}

void unionFind::newComp(int id) {
    comp[id] = id;
}
