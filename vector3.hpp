//
//  vector3.hpp
//  CompSci78
//
//  Created by Michael Schuff on 9/30/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef vector3_hpp
#define vector3_hpp

#include <iostream>
#include <math.h>
#include <assert.h>
#include "object.hpp"

class vector3 : public object {
public:
    double x, y, z;

    vector3() : object(), x(0), y(0), z(0) {}
    vector3(double x1, double y1, double z1, bool visable = true) : object(visable), x(x1), y(y1), z(z1) {}
    vector3(const vector3 &v, bool visable = true) : object(visable), x(v.x), y(v.y), z(v.z) {}//copy constructor
    
    vector3 operator+(const vector3 &v);
    vector3 &operator+=(const vector3 &v);
    vector3 operator-(const vector3 &v);
    vector3 &operator-=(const vector3 &v);
    vector3 operator*(double k);
    vector3 &operator*=(double k);
    vector3 operator/(double k);
    vector3 &operator/=(double k);
    vector3 &operator=(const vector3 &v);
    vector3 operator-();
    double &operator[](int i);
    
    void cross(const vector3 &v);                       //vector becomes the cross product of the vector and the parameter
    void project_onto(vector3 v);
    
    void rotate(vector3 axis, double theta);            //rotates vector, clockwise by right-hand-rule
    vector3 rotated(vector3 axis, double theta);        //returns rotated vector
    
    void normalize();                                   //sets magnitude to 1
    vector3 normalized();                               //returns vector with magnitude 1
    double magnitude() const;
    double distance(const vector3 &v);                  //distance between two vectors
    std::string to_string();
    void print();
};

vector3 vector3::operator+(const vector3 &v) {
    return vector3(x+v.x, y+v.y, z+v.z);
}

vector3 &vector3::operator+=(const vector3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

vector3 vector3::operator-(const vector3 &v) {
    return vector3(x-v.x, y-v.y, z-v.z);
}

vector3 &vector3::operator-=(const vector3 &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

vector3 vector3::operator*(double k) {      //for vector * double
    return vector3(k*x, k*y, k*z);
}

vector3 operator*(double k, const vector3 v) {    //for double * vector
    return vector3(k*v.x, k*v.y, k*v.z);
}

vector3 &vector3::operator*=(double k) {
    x *= k;
    y *= k;
    z *= k;
    return *this;
}

vector3 vector3::operator/(double k){
    assert(k!=0);
    return vector3(x/k, y/k, z/k);
}

vector3 &vector3::operator/=(double k) {
    assert(k!=0);
    x /= k;
    y /= k;
    z /= k;
    return *this;
}

vector3 &vector3::operator=(const vector3 &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

vector3 vector3::operator-() {
    return vector3(-x, -y, -z);
}

double &vector3::operator[](int i) {
    assert(i>=0 && i<3);
    switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
    }
}

double vector3::magnitude() const {
    return sqrt(x*x + y*y + z*z);
}

void vector3::normalize() {
    assert(magnitude()!=0);
    *this /= magnitude();
}

vector3 vector3::normalized() {
    double m = magnitude();
    assert(m!=0);
    return vector3(x/m, y/m, z/m);
}

double vector3::distance(const vector3 &v) {
    return (*this-v).magnitude();
}

double dot_product(const vector3 &v1, const vector3 &v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

void vector3::cross(const vector3 &v) {
    double i = y*v.z - z*v.y;
    double j = z*v.x - x*v.z;
    double k = x*v.y - y*v.x;
    x = i;
    y = j;
    z = k;
}

vector3 cross_product(const vector3 &v1, const vector3 &v2) {
    double i = v1.y*v2.z - v1.z*v2.y;
    double j = v1.z*v2.x - v1.x*v2.z;
    double k = v1.x*v2.y - v1.y*v2.x;
    return vector3(i, j, k);
}

vector3 projection(vector3 v1, vector3 v2) { //project v1 onto v2
    return v2 * (dot_product(v1, v2)/pow(v2.magnitude(), 2));
}

void vector3::project_onto(vector3 v) { //project v1 onto v2
    vector3 temp = v * dot_product(*this, v)/pow(v.magnitude(), 2);
    x = temp.x;
    y = temp.y;
    z = temp.z;
}

vector3 vector3::rotated(vector3 axis, double theta) {
    axis.normalize();
    double _sin = sin(theta);
    double _cos = cos(theta);
    return *this*_cos + _sin*cross_product(axis, *this) + axis*dot_product(*this, axis)*(1-_cos); //dont question the stack overflow gods
}

void vector3::rotate(vector3 axis, double theta) {
    vector3 v = rotated(axis, theta);
    x = v.x;
    y = v.y;
    z = v.z;
}


std::string vector3::to_string() {
    return ("<" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ">");
}

void vector3::print() {
    std::cout << (*this).to_string() << std::endl;
}
#endif /* vector3_hpp */
