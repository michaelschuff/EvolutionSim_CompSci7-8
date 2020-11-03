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
class plane : public object {
public:
    vector3 position, normal, up, right;
    float width, height;
    color c;
    
    vector3 top_left, top_right, bottom_left, bottom_right;
    
    plane() : object(false), position(vector3()), normal(vector3()), up(vector3()), width(0), height(0), c(color()) {set_corners();}
    plane(vector3 _position, vector3 _normal, vector3 _up, float _width = 10, float _height = 10, color _c = color(), bool visable = true) : object(visable), position(_position), normal(_normal.normalized()), up(_up.normalized()), width(_width), height(_height), c(_c) {set_corners();}
    plane(vector3 p1, vector3 p2, vector3 p3, vector3 _up, float _width = 10, float _height = 10, color _c = color(), bool visable = true) : object(visable), position(p1), normal(cross_product(p2-p1, p3-p1).normalized()), up(_up.normalized()), width(_width), height(_height), c(_c) {set_corners();}
    
    plane &operator=(const plane &p);
    
    void flip();
    std::string to_string();
    void print();
private:
    void set_corners();
};

plane &plane::operator=(const plane &p) {
    position = p.position;
    normal = p.normal;
    width = p.width;
    height = p.height;
    return *this;
}

void plane::set_corners() {
    right = cross_product(normal, up).normalized();
    top_left = position + up * height / 2 - right * width / 2;
    top_right = position + up * height / 2 + right * width / 2;
    bottom_left = position - up * height / 2 - right * width / 2;
    bottom_right = position - up * height / 2 + right * width / 2;
}

void plane::flip() {
    normal = -normal;
    right = cross_product(normal, up).normalized();
}

std::string plane::to_string() {
    return "Position: " + position.to_string() + "    Normal: " + normal.to_string() + "    Up: " + up.to_string() + "\nWidth: " + std::to_string(width) + "    Height: " + std::to_string(height);
}

void plane::print() {
    std::cout << (*this).to_string() << std::endl;
}

#endif /* plane_hpp */
