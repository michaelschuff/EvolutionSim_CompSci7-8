//
//  plane.hpp
//  CompSci78
//
//  Created by Michael Schuff on 10/3/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef plane_hpp
#define plane_hpp

#include "vector3.hpp"
#include <iostream>
class plane : object {
public:
    vector3 position, normal;
    double width, height, roll;
    
    plane() : object(false), position(vector3()), normal(vector3()), width(0), height(0), roll(0) {}
    plane(vector3 _position, vector3 _normal, double _width = 10, double _height = 10, double _roll = 0, bool visable = true) : object(visable), position(_position), normal(_normal), width(_width), height(_height), roll(_roll) {}
    plane(double px, double py, double pz, vector3 _normal, double _width = 10, double _height = 10, double _roll = 0, bool visable = true) : object(visable), position(vector3(px, py, pz)), normal(_normal), width(_width), height(_height), roll(_roll) {}
    plane(vector3 _position, double nx, double ny, double nz, double _width = 10, double _height = 10, double _roll = 0, bool visable = true) : object(visable), position(_position), normal(vector3(nx, ny, nz)), width(_width), height(_height), roll(_roll) {}
    plane(double px, double py, double pz, double nx, double ny, double nz, double _width = 10, double _height = 10, double _roll = 0, bool visable = true) : object(visable), position(vector3(px, py, pz)), normal(vector3(nx, ny, nz)), width(_width), height(_height), roll(_roll) {}
    plane(vector3 p1, vector3 p2, vector3 p3, double _width = 10, double _height = 10, double _roll = 0, bool visable = true) : object(visable), position(p1), normal(cross_product(p2-p1, p3-p1)), width(_width), height(_height), roll(_roll) {}
    
    plane &operator=(const plane &p);
    
    void flip();
    std::string to_string();
    void print();
};

plane &plane::operator=(const plane &p) {
    position = p.position;
    normal = p.normal;
    width = p.width;
    height = p.height;
    roll = p.roll;
    return *this;
}

void plane::flip() {
    normal = -normal;
}

std::string plane::to_string() {
    return "Position: " + position.to_string() + "    Normal: " + normal.to_string() + "\nWidth: " + std::to_string(width) + "    Height: " + std::to_string(height) + "    Roll: " + std::to_string(roll);
}

void plane::print() {
    std::cout << (*this).to_string() << std::endl;
}

#endif /* plane_hpp */
