//
//  objects2d.hpp
//  CompSci78
//
//  Created by Michael Schuff on 10/3/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef objects2d_hpp
#define objects2d_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class object2D {
public:
    Color c;
    
    object2D(Color _c) : c(_c) {}
    
    virtual void print() = 0;
};

class circle : public object2D {
public:
    double x, y, r;
    
    circle(double _x, double _y, double _r, Color _c) : object2D(_c), x(_x), y(_y), r(_r) {
        
    }
    
    void print() {
        std::cout << "<" << x << ", " << y << ">, r: " << r << std::endl;
    }
};

class rectangle : public object2D {
public:
    double x, y, width, height, theta;
    
    rectangle(double _x, double _y, double _width, double _height, double _theta, Color _c) : object2D(_c), x(_x), y(_y), width(_width), height(_height), theta(_theta) {
        
    }
    
    void print() {
        std::cout << "<" << x << ", " << y << ">\nwidth:" << width << "\nheight: " << height << "\ntheta: " << theta << std::endl;
    }
};

class convexshape : public object2D {
public:
    std::vector<std::vector<double>> points;
    
    convexshape(std::vector<std::vector<double>> _points, Color _c) : object2D(_c), points(_points) {
        
    }
    
    void print() {
        for (int i = 0; i < points.size(); i++) {
            std::cout << "<" << points[i][0] << ", " << points[i][1] << ">" << std::endl;
        }
    }
};

#endif /* objects2d_hpp */
