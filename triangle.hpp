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
#include <iostream>

class triangle : public object {
public:
    vector3 v1, v2, v3;
    
    triangle(vector3 _v1=vector3(), vector3 _v2=vector3(), vector3 _v3=vector3(), bool visable = true) : object(visable), v1(_v1), v2(_v2), v3(_v3) {}
    triangle(const triangle &tri, bool visable = true) : object(visable), v1(tri.v1), v2(tri.v2), v3(tri.v3) {}//copy constructor
    
    triangle &operator=(const triangle &tri);
    vector3 &operator[](int i);
    double area();
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

double triangle::area() {
    return 0.5 * cross_product(v2-v1, v3-v1).magnitude();
}

std::string triangle::to_string() {
    return v1.to_string() + ", " + v2.to_string() + ", " + v3.to_string();
}

void triangle::print() {
    std::cout << (*this).to_string() << std::endl;
}

#endif /* triangle_hpp */
