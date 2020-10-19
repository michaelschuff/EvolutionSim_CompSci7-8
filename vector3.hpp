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
#include "color.hpp"
#include "object.hpp"


/**
 * Rounds floating point number to a number of decimal places.
 *
 * @param n number
 * @param decimals number of decimal places. 0 to cast to int, 1 to round to tenths, -1 to round to nearest 10
 * @return rounded floating point number.
 */
float round(float n, int decimals) {
    return round(pow(10, decimals) * n) / pow(10, decimals);
}

class quaternion;
class vector3 : public object {
public:
    float x, y, z;
    color c;

    vector3() : object(), x(0), y(0), z(0) {}
    vector3(float x1, float y1, float z1, color _c = color(), bool visable = true) : object(visable), x(x1), y(y1), z(z1), c(_c) {}
    vector3(const vector3 &v, color _c, bool visable = true) : object(visable), x(v.x), y(v.y), z(v.z), c(_c) {}//copy constructor

    vector3 operator+(const vector3 &v);
    vector3 &operator+=(const vector3 &v);
    vector3 operator-(const vector3 &v);
    vector3 operator-() const;
    vector3 &operator-=(const vector3 &v);
    vector3 operator*(float k);
    vector3 &operator*=(float k);
    vector3 operator/(float k);
    vector3 &operator/=(float k);
    vector3 &operator=(const vector3 &v);
    vector3 operator-();
    float &operator[](int i) const;

    void cross(const vector3 &v);                       //vector becomes the cross product of the vector and the parameter
    void project_onto(vector3 v);

    void rotate(vector3 axis, float theta);
    void rotate(const quaternion &q);
    vector3 rotated(vector3 axis, float theta);        //returns rotated vector
    vector3 rotated(const quaternion &q);

    float phi();
    float theta();
    void normalize();                                   //sets magnitude to 1
    vector3 normalized();                               //returns vector with magnitude 1
    float magnitude() const;
    float square() const;
    float distance(const vector3 &v);                  //distance between two vectors
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

vector3 vector3::operator-() {
    return vector3(-x, -y, -z);
}

vector3 &vector3::operator-=(const vector3 &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

vector3 vector3::operator*(float k) {                  //for vector * float
    return vector3(k*x, k*y, k*z);
}

vector3 operator*(float k, const vector3 v) {          //for float * vector
    return vector3(k*v.x, k*v.y, k*v.z);
}

vector3 &vector3::operator*=(float k) {
    x *= k;
    y *= k;
    z *= k;
    return *this;
}

vector3 vector3::operator/(float k){
    assert(k!=0);
    return vector3(x/k, y/k, z/k);
}

vector3 &vector3::operator/=(float k) {
    if (k == 0) {
        return *this;
    }
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

vector3 vector3::operator-() const {
    return vector3(-x, -y, -z);
}

float &vector3::operator[](int i) const {
    assert(i>=0 && i<3);
    static float a;
    switch (i) {
        case 0:
            a = x;
            break;
        case 1:
            a = x;
            break;
        case 2:
            a = x;
            break;
    }
    return a;
}

float vector3::phi() {
    return atan2(sqrt(x*x + z*z), y);
}

float vector3::theta() {
    return atan2(round(z, 8), round(x, 8));
}

float vector3::magnitude() const {
    return sqrt(x*x + y*y + z*z);
}

float vector3::square() const {
    return x*x + y*y + z*z;
}

void vector3::normalize() {
    assert(magnitude()!=0);
    *this /= magnitude();
}

vector3 vector3::normalized() {
    float m = magnitude();
    if (m == 0) {
        return vector3();
    }
    return vector3(x/m, y/m, z/m);
}

float vector3::distance(const vector3 &v) {
    return (*this-v).magnitude();
}

float dot_product(const vector3 &v1, const vector3 &v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

void vector3::cross(const vector3 &v) {
    float i = y*v.z - z*v.y;
    float j = z*v.x - x*v.z;
    float k = x*v.y - y*v.x;
    x = i;
    y = j;
    z = k;
}

vector3 cross_product(const vector3 &v1, const vector3 &v2) {
    return vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
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

std::string remove_terminating_zeros(std::string str) {
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    str.erase(str.find_last_not_of('.') + 1, std::string::npos);
    return str;
}

std::string vector3::to_string() {
    return ("<" + remove_terminating_zeros(std::to_string(x)) + ", " + remove_terminating_zeros(std::to_string(y)) + ", " + remove_terminating_zeros(std::to_string(z)) + ">");
}

void vector3::print() {
    std::cout << (*this).to_string() << std::endl;
}


#endif /* vector3_hpp */
