//
// Created by jiahui on 7/4/22.
//

#ifndef CONTOURTREE_UNIONFIND_H
#define CONTOURTREE_UNIONFIND_H

#include <vector>
#include <numeric>

class unionFind {
public:
    unionFind(int size);
    void initialize();
    void newComp(int id);
    int find(int id);
    void pathComp(int startId, int id);
    void join(int cId, int pId);

private:
    std::vector<int> comp;

};


#endif //CONTOURTREE_UNIONFIND_H
