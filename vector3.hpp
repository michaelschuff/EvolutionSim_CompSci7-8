//
//  vector3.hpp
//  CompSci78
//
//  Created by Michael Schuff on 9/30/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef vector3_hpp
#define vector3_hpp

#include <stdio.h>

#include <iostream>
#include <math.h>
#include <assert.h>

class vector3 {
public:
    double x, y, z;

    vector3(double x1=0, double y1=0, double z1=0) : x(x1), y(y1), z(z1) {}
    vector3(const vector3 &v) : x(v.x), y(v.y), z(v.z) {}//copy constructor
    
    vector3 operator+(const vector3 &v);
    vector3 &operator+=(const vector3 &v);
    vector3 operator-(const vector3 &v);
    vector3 &operator-=(const vector3 &v);
    vector3 operator*(double k);
    vector3 &operator*=(double k);
    vector3 operator/(double k);
    vector3 &operator/=(double k);
    vector3 &operator=(const vector3 &v);
    double &operator[](int i);
    
    double dot_product(const vector3 &v);
    vector3 cross_product(const vector3 &v);            //cross_product
    
    void rotate(vector3 axis, double theta);  //rotates vector, clockwise by right-hand-rule
    vector3 rotated(vector3 axis, double theta);    //returns rotated vector
    
    vector3 normalize();                                //sets magnitude to 1
    double magnitude();
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

vector3 operator*(double k, vector3 v) {    //for double * vector
    return v * k;
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

double vector3::dot_product(const vector3 &v) {
    return x*v.x + y*v.y + z*v.z;
}

vector3 vector3::cross_product(const vector3 &v) {
    double i = y*v.z - z*v.y;
    double j = z*v.x - x*v.z;
    double k = x*v.y - y*v.x;
    return vector3(i, j, k);
}

vector3 vector3::rotated(vector3 axis, double theta) {
    vector3 copy(*this);
    axis.normalize();
    double sin_theta = sin(theta);
    double cos_theta = cos(theta);
    return *this*cos_theta + sin_theta*(axis.cross_product(*this)) + dot_product(axis)*(1-cos_theta); //dont question the stack overflow gods
}

void vector3::rotate(vector3 axis, double theta) {
    vector3 v = rotated(axis, theta);
    x = v.x;
    y = v.y;
    z = v.z;
}

double vector3::magnitude() {
    return sqrt(x*x + y*y + z*z);
}

vector3 vector3::normalize() {
    assert(magnitude()!=0);
    *this /= magnitude();
    return *this;
}

double vector3::distance(const vector3 &v) {
    return (*this-v).magnitude();
}

std::string vector3::to_string() {
    return ("<" + std::to_string((int) x) + ", " + std::to_string((int) y) + ", " + std::to_string((int) z) + ">");
}

void vector3::print() {
    std::cout << (*this).to_string() << std::endl;
}
#endif /* vector3_hpp */
