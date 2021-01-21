//
//  vector3.hpp
//  Graphics3D
//
//  Created by Michael Schuff on 10/26/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef vector3_hpp
#define vector3_hpp

#include <stdio.h>
#include <iostream>

class quaternion;
class vector3 {
public:
    float x, y, z;
    vector3();
    vector3(const float&, const float&, const float&);
    
    void normalize();
    void projected_on(const vector3&);
    void rotate(const quaternion&);
    void rotate(const vector3&, float);
    void rotate(const vector3&, float, const vector3&);
    
    float magnitude() const;
    float square() const;
    float distance(const vector3&) const;
    vector3 crossed(const vector3&) const;
    vector3 normalized() const;
    vector3 rotated(const quaternion&) const;
    vector3 rotated(const vector3&, float) const;
    vector3 rotated(const vector3&, float, const vector3&) const;
    
    std::string to_string() const;
    void print() const;
    
    vector3& operator+=(const vector3&);
    vector3& operator-=(const vector3&);
    vector3& operator*=(const float&);
    vector3& operator*=(const quaternion&);
    vector3& operator/=(const float&);
    vector3& operator%=(const float&);
    vector3& operator%=(const vector3&);
    float operator[](int) const;
};


vector3::vector3() : x(0), y(0), z(0) {
    
}

vector3::vector3(const float& t_x, const float& t_y, const float& t_z) : x(t_x), y(t_y), z(t_z) {
    
}

vector3 operator+(const vector3& v1, const vector3& v2) {
    return vector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

vector3 operator-(const vector3& v1, const vector3& v2) {
    return vector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

vector3 operator-(const vector3& v) {
    return vector3(-v.x, -v.y, -v.z);
}

vector3 operator*(const float& k, const vector3& v) {
    return vector3(k*v.x, k*v.y, k*v.z);
}

vector3 operator*(const vector3& v, const float& k) {
    return vector3(k*v.x, k*v.y, k*v.z);
}

vector3 operator*(const vector3& v, const quaternion& q) {
    return v.rotated(q);
}

vector3 operator*(const quaternion& q, const vector3& v) {
    return v.rotated(q);
}

vector3 operator/(const vector3& v, const float& k) {
    if (k != 0) {
        return vector3(v.x/k, v.y/k, v.z/k);
    } else {
        return v;
    }
}

vector3 operator%(const vector3& v, const float& k) {
    vector3 v1(v);
    while (v1.x >= k) { v1.x -= k; }
    while (v1.x < 0) { v1.x += k; }
    while (v1.y >= k) { v1.y -= k; }
    while (v1.y < 0) { v1.y += k; }
    while (v1.z >= k) { v1.z -= k; }
    while (v1.z < 0) { v1.z += k; }
    return v1;
}

vector3 operator%(const vector3& v1, const vector3& v2) {
    vector3 v3(v1);
    while (v3.x >= v2.x) { v3.x -= v2.x; }
    while (v3.x < 0) { v3.x += v2.x; }
    while (v3.y >= v2.y) { v3.y -= v2.y; }
    while (v3.y < 0) { v3.y += v2.y; }
    while (v3.z >= v2.z) { v3.z -= v2.z; }
    while (v3.z < 0) { v3.z += v2.z; }
    return v3;
}

bool operator!=(const vector3& v1, const vector3& v2) {
    return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z);
}

float vector3::operator[](int i) const {
    float a;
    if (i >= 0 && i < 3) {
        switch (i % 3) {
            case 0:
                a = x;
            case 1:
                a = y;
            case 2:
                a = z;
        }
    }
    if (a) {
        return a;
    }
    throw std::out_of_range("Expected value between 0 and 3, got " + std::to_string(i));
}

vector3& vector3::operator+=(const vector3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

vector3& vector3::operator-=(const vector3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

vector3& vector3::operator*=(const float& k) {
    x *= k;
    y *= k;
    z *= k;
    return *this;
}

vector3& vector3::operator*=(const quaternion& q) {
    rotate(q);
    return *this;
}

vector3& vector3::operator/=(const float& k) {
    if (k != 0) {
        x /= k;
        y /= k;
        z /= k;
    }
    return *this;
}

vector3& vector3::operator%=(const float& k) {
    while (x > k) { x -= k; }
    while (x < 0) { x += k; }
    while (y > k) { y -= k; }
    while (y < 0) { y += k; }
    while (z > k) { z -= k; }
    while (z < 0) { z += k; }
    return *this;
}

vector3& vector3::operator%=(const vector3& v) {
    while (x > v.x) { x -= v.x; }
    while (x < 0) { x += v.x; }
    while (y > v.y) { y -= v.y; }
    while (y < 0) { y += v.y; }
    while (z > v.z) { z -= v.z; }
    while (z < 0) { z += v.z; }
    return *this;
}

float dot_product(const vector3& v1, const vector3& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

vector3 cross_product(const vector3& v1, const vector3& v2) {
    return vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

vector3 projection(const vector3& v1, const vector3& v2) {
    return v2 * dot_product(v1, v2) / v2.square();
}

void vector3::normalize() {
    float mag = magnitude();
    if (mag != 0) {
        *this /= mag;
    }
}

void vector3::projected_on(const vector3& v) {
    vector3 v1 = v * dot_product(*this, v) / v.square();
    x = v1.x;
    y = v1.y;
    z = v1.z;
}

float vector3::magnitude() const {
    return sqrt(x*x + y*y + z*z);
}

float vector3::square() const {
    return x*x + y*y + z*z;
}

float vector3::distance(const vector3& v) const {
    return (*this-v).magnitude();
}

vector3 vector3::crossed(const vector3& v) const {
    return vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

vector3 vector3::normalized() const {
    float k = magnitude();
    if (k == 0) {
        return *this;
    }
    return vector3(x/k, y/k, z/k);
}

static std::string remove_terminating_zeros(std::string str) {
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    str.erase(str.find_last_not_of('.') + 1, std::string::npos);
    return str;
}

std::string vector3::to_string() const {
    return ("<" + remove_terminating_zeros(std::to_string(x)) + ", " + remove_terminating_zeros(std::to_string(y)) + ", " + remove_terminating_zeros(std::to_string(z)) + ">");
}

void vector3::print() const {
    std::cout << (*this).to_string() << std::endl;
}

#endif /* vector3_hpp */
