//
// Created by jiahui on 6/4/22.
//

#include "mesh.h"

mesh::mesh() {}

mesh::mesh(std::string fileName) {
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        std::cout << "File Not Open!" << std::endl;
    } else {
        inputFile >> m >> n;
        meshVal.reserve(m*n+1);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                double tempVal;
                inputFile >> tempVal;
                meshVal.push_back(tempVal);
            }
        }
    }
//    std::cout << "m: " << m << ", " << "n: " << n << std::endl;
//    std::cout << this->meshVal.size() << std::endl;
//    printMesh();
}

void mesh::printMesh() {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%3.0f ", this->meshVal[i*n+j]);
        }
        printf("\n");
    }
}

const std::vector<double> &mesh::getMeshVal() const {
    return meshVal;
}

int mesh::getM() const {
    return m;
}

int mesh::getN() const {
    return n;
}
