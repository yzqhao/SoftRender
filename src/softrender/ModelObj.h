#pragma once

#include <vector>
#include <string>
#include "Image.h"
#include "Vector.h"

class Model {
private:
    std::vector<Vector3f> verts_;
    std::vector<std::vector<int> > faces_;
public:
    Model(const char* filename);
    ~Model();
    int nverts();
    int nfaces();
    Vector3f vert(int i);
    std::vector<int> face(int idx);
};