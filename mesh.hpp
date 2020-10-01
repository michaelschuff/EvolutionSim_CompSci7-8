//
//  mesh.hpp
//  CompSci78
//
//  Created by Michael Schuff on 9/30/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef mesh_h
#define mesh_h

#include <vector>
#include <assert.h>
#include "triangle.hpp"

class mesh {
public:
    std::vector<triangle> triangles;
    
    mesh(std::vector<triangle> _triangles=std::vector<triangle>()) : triangles(_triangles) {}
    mesh(const mesh &m) : triangles(m.triangles) {}//copy constructor
    
    
    int size();
    triangle &operator[](int i);
};

int mesh::size() {
    return triangles.size();
}

triangle &mesh::operator[](int i) {
    assert(i>=0 && i<(*this).size());
    return triangles[i];
}

#endif /* mesh_h */
