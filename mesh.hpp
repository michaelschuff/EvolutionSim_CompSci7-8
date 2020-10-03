//
//  mesh.hpp
//  CompSci78
//
//  Created by Michael Schuff on 9/30/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef mesh_hpp
#define mesh_hpp

#include <vector>
#include <assert.h>
#include <iostream>
#include "triangle.hpp"
#include "object.hpp"

class mesh : public object {
public:
    std::vector<triangle> triangles;
    
    mesh(std::vector<triangle> _triangles=std::vector<triangle>(), bool visable = true) : object(visable), triangles(_triangles) {}
    mesh(const mesh &m, bool visable = true) : object(visable), triangles(m.triangles) {}//copy constructor
    
    triangle &operator[](int);
    
    int size();
    void pop();
    void remove(int, int);
    void push_back(const triangle &tri);
    void insert(int i, const triangle &tri);
    
    std::string to_string();
    void print();
};

triangle &mesh::operator[](int i) {
    assert(i>=0 && i<(*this).size());
    return triangles[i];
}

int mesh::size() {
    return triangles.size();
}

void mesh::pop() {
    assert(size()>=0);
    triangles.pop_back();
}

void mesh::remove(int i, int j = -1) {
    assert(i>=0 && i<size());
    if (j == -1) {
        triangles.erase(triangles.begin() + i);
    } else {
        assert(j>i && j<size());
        triangles.erase(triangles.begin() + i, triangles.begin() + j);
    }
}

void mesh::push_back(const triangle &tri) {
    triangles.push_back(tri);
}

void mesh::insert(int i, const triangle &tri) {
    triangles.insert(triangles.begin() + i, tri);
}

std::string mesh::to_string() {
    std::string s = "";
    for (int i = 0; i < triangles.size(); i++) {
        s += "Triangle 1: " + triangles[i].to_string() + "\n";
    }
    return s;
}

void mesh::print() {
    std::cout << (*this).to_string() << std::endl;
}

#endif /* mesh_hpp */
