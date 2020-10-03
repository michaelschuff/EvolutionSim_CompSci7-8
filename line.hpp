//
//  line.hpp
//  CompSci78
//
//  Created by Michael Schuff on 10/3/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef line_hpp
#define line_hpp

#include "vector3.hpp"
#include <iostream>

class line : public object {
public:
    vector3 tail, head, body;
    
    line(double x1, double y1, double z1, double x2, double y2, double z2, bool visable = true) : object(visable), tail(vector3(x1, y1, z1)), head(vector3(x2, y2, z2)), body(vector3(x2-x1, y2-y1, z2-z1)) {}
    line(vector3 _tail, double x2, double y2, double z2, bool visable = true) : object(visable), tail(_tail), head(vector3(x2, y2, z2)), body(vector3(x2-_tail.x, y2-_tail.y, z2-_tail.z)) {}
    line(double x1, double y1, double z1, vector3 _head, bool visable = true) : object(visable), tail(vector3(x1, y1, z1)), head(_head), body(vector3(_head.x-x1, _head.y-y1, _head.z-z1)) {}
    line(vector3 _tail, vector3 _head, bool visable = true) : object(visable), tail(_tail), head(_head), body(_head-_tail) {}
    
    line rotated(vector3 axis, double theta);        //returns rotated vector
    void rotate(vector3 axis, double theta);            //rotates vector, clockwise by right-hand-rule
    line rotated(line axis, double theta);        //returns rotated vector
    void rotate(line axis, double theta);            //rotates vector, clockwise by right-hand-rule
    double magnitude() const;
    std::string to_string();
    void print();
};

double line::magnitude() const {
    return body.magnitude();
}

line line::rotated(vector3 axis, double theta) {
    return line(tail.rotated(axis, theta), head.rotated(axis, theta));
}

void line::rotate(vector3 axis, double theta) {
    line l = (*this).rotated(axis, theta);
    tail = l.tail;
    head = l.head;
    body = l.body;
}

line line::rotated(line axis, double theta) {
    return line((tail - axis.body).rotated(axis.body, theta), (head - axis.body).rotated(axis.body, theta));
}

void line::rotate(line axis, double theta) {
    line l = (*this).rotated(axis, theta);
    tail = l.tail;
    head = l.head;
    body = l.body;
}

std::string line::to_string() {
    return "Tail: " + (*this).tail.to_string() + "    Head: " + (*this).head.to_string();
}

void line::print() {
    std::cout << (*this).to_string() << std::endl;
}

#endif /* line_hpp */
