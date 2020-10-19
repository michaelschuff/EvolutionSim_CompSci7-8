//
//  triangle.hpp
//  CompSci78
//
//  Created by Michael Schuff on 9/30/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef triangle_hpp
#define triangle_hpp

#include "vector3.hpp"
#include "object.hpp"
#include "line.hpp"
#include <iostream>

class triangle : public object {
public:
    vector3 v1, v2, v3, normal;
    color c;
    
    triangle(vector3 _v1=vector3(), vector3 _v2=vector3(), vector3 _v3=vector3(), color _c = color(), bool visable = true) : object(visable), v1(_v1), v2(_v2), v3(_v3), c(_c), normal(cross_product(_v2 - _v1, _v3 - _v1)) {}
    triangle(const triangle &tri, bool visable = true) : object(visable), v1(tri.v1), v2(tri.v2), v3(tri.v3), c(tri.c), normal(tri.normal) {}//copy constructor
    
    triangle &operator=(const triangle &tri);
    vector3 &operator[](int i);
//    void setTexture(sf::Image &image, int x1, int y1, int x2, int y2, int x3, int y3);
    float area();
    std::string to_string();
    void print();
};

triangle &triangle::operator=(const triangle &tri) {
    v1 = tri.v1;
    v2 = tri.v2;
    v3 = tri.v3;
    return *this;
}

vector3 &triangle::operator[](int i) {
    assert(i>=0 && i<3);
    switch (i) {
        case 0:
            return v1;
        case 1:
            return v2;
        case 2:
            return v3;
    }
}

//void triangle::setTexture(sf::Image &_image, int x1, int y1, int x2, int y2, int x3, int y3) {
//    image = _image;
//    p1 = vector3(x1, 0, y1);
//    p2 = vector3(x2, 0, y2);
//    p3 = vector3(x3, 0, y3);
//}

float triangle::area() {
    return 0.5 * cross_product(v2-v1, v3-v1).magnitude();
}

std::string triangle::to_string() {
    return v1.to_string() + ", " + v2.to_string() + ", " + v3.to_string();
}

void triangle::print() {
    std::cout << (*this).to_string() << std::endl;
}

#endif /* triangle_hpp */
