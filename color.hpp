//
//  color.hpp
//  CompSci78
//
//  Created by Michael Schuff on 10/1/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef color_hpp
#define color_hpp

#include <iostream>

class color {
public:
    int r, g, b, a;
    
    color() : r(255), g(255), b(255), a(255) {}
    color(int _r, int _g, int _b, int _a = 255) : r(_r), g(_g), b(_b), a(_a) {}
    color(float _r, float _g, float _b, float _a = 1) : r(255*_r), g(255*_g), b(255*_b), a(255*_a) {}
    color(double _r, double _g, double _b, double _a = 1) : r(255*_r), g(255*_g), b(255*_b), a(255*_a) {}
    color(const color &c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
    
    int &operator[](int i);
    std::string to_string();
    void print();
};

int &color::operator[](int i) {
    assert(i>=0 && i<4);
    switch (i) {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
            return a;
    }
}

std::string color::to_string() {
    return "<" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ">, " + std::to_string(a);
}

void color::print() {
    std::cout << (*this).to_string() << std::endl;
}

#endif /* color_hpp */
