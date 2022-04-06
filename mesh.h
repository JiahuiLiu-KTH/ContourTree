//
// Created by jiahui on 6/4/22.
//

#ifndef CONTOURTREE_MESH_H
#define CONTOURTREE_MESH_H

#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>

class mesh {
public:
    mesh();
    mesh(std::string fileName);

    void printMesh();

    int getM() const;

    int getN() const;

    const std::vector<double> &getMeshVal() const;

private:
    int m,n;
    std::vector<double> meshVal;
};


#endif //CONTOURTREE_MESH_H
