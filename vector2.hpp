//
//  vector2.hpp
//  Graphics3D
//
//  Created by Michael Schuff on 11/2/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef vector2_hpp
#define vector2_hpp

#include <stdio.h>
#include <iostream>

class vector2 {
public:
    float x, y;
    
    vector2();
    vector2(const float&, const float&);
    
    void normalize();
    void projected_on(const vector2&);
    void rotate(const float&);
    void rotate(const vector2&, const float&);
    
    float magnitude() const;
    float square() const;
    float distance(const vector2&) const;
    vector2 normalized() const;
    vector2 rotated(const float&) const;
    vector2 rotated(const vector2&, const float&) const;
    
    std::string to_string() const;
    void print() const;
    
    vector2 &operator+=(const vector2&);
    vector2 &operator-=(const vector2&);
    vector2 &operator*=(const float&);
    vector2 &operator/=(const float&);
    vector2 &operator%=(const float&);
    vector2 &operator%=(const vector2&);
    float& operator[](const int&);
};

//vector2 operator+(const vector2&, const vector2&);
//vector2 operator-(const vector2&, const vector2&);
//vector2 operator-(const vector2&);
//vector2 operator*(const float&, const vector2&);
//vector2 operator*(const vector2&, const float&);
//vector2 operator/(const vector2&, const float&);
//vector2 operator%(const vector2&, const float&);
//vector2 operator%(const vector2&, const vector2&);
//bool operator==(const vector2&, const vector2&);
//float dot_product(const vector2&, const vector2&);
//vector2 projection(const vector2&, const vector2&);


vector2::vector2() : x(0), y(0) {
    
}

vector2::vector2(const float& t_x, const float& t_y) : x(t_x), y(t_y) {
    
}

vector2 operator+(const vector2& v1, const vector2& v2) {
    return vector2(v1.x+v2.x, v1.y+v2.y);
}

vector2 operator-(const vector2& v1, const vector2& v2) {
    return vector2(v1.x-v2.x, v1.y-v2.y);
}

vector2 operator-(const vector2& v) {
    return vector2(-v.x, -v.y);
}

vector2 operator*(const float& k, const vector2& v) {
    return vector2(k*v.x, k*v.y);
}

vector2 operator*(const vector2& v, const float& k) {
    return vector2(k*v.x, k*v.y);
}

vector2 operator/(const vector2& v, const float& k) {
    if (k != 0) {
        return vector2(v.x/k, v.y/k);
    } else {
        return v;
    }
}

vector2 operator%(const vector2& v, const float& k) {
    vector2 v1(v);
    while (v1.x >= k) { v1.x -= k; }
    while (v1.x < 0) { v1.x += k; }
    while (v1.y >= k) { v1.y -= k; }
    while (v1.y < 0) { v1.y += k; }
    return v1;
}

vector2 operator%(const vector2& v1, const vector2& v2) {
    vector2 v3(v1);
    while (v3.x >= v2.x) { v3.x -= v2.x; }
    while (v3.x < 0) { v3.x += v2.x; }
    while (v3.y >= v2.y) { v3.y -= v2.y; }
    while (v3.y < 0) { v3.y += v2.y; }
    return v3;
}

float& vector2::operator[](const int& i) {
    if (i >= 0 && i < 2) {
        switch (i % 2) {
            case 0:
                return x;
            case 1:
                return y;
        }
    }
    throw std::out_of_range("Expected value between 0 and 2, got " + std::to_string(i));
}

bool operator==(const vector2& v1, const vector2& v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

vector2& vector2::operator+=(const vector2& v) {
    x += v.x;
    y += v.y;
    return *this;
}

vector2& vector2::operator-=(const vector2& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

vector2& vector2::operator*=(const float& k) {
    x *= k;
    y *= k;
    return *this;
}

vector2& vector2::operator/=(const float& k) {
    if (k != 0) {
        x /= k;
        y /= k;
    }
    return *this;
}

vector2& vector2::operator%=(const float& k) {
    while (x > k) { x -= k; }
    while (x < 0) { x += k; }
    while (y > k) { y -= k; }
    while (y < 0) { y += k; }
    return *this;
}

vector2& vector2::operator%=(const vector2& v) {
    while (x > v.x) { x -= v.x; }
    while (x < 0) { x += v.x; }
    while (y > v.y) { y -= v.y; }
    while (y < 0) { y += v.y; }
    return *this;
}

float dot_product(const vector2& v1, const vector2& v2) {
    return v1.x*v2.x + v1.y*v2.y;
}

vector2 projection(const vector2& v1, const vector2& v2) {
    return v2 * dot_product(v1, v2) / v2.square();
}

void vector2::normalize() {
    float mag = magnitude();
    if (mag != 0) {
        *this /= mag;
    }
}

void vector2::projected_on(const vector2& v) {
    vector2 v1 = v * dot_product(*this, v) / v.square();
    x = v1.x;
    y = v1.y;
}

float vector2::magnitude() const {
    return sqrt(x*x + y*y);
}

float vector2::square() const {
    return x*x + y*y;
}

float vector2::distance(const vector2& v) const {
    return (*this-v).magnitude();
}

vector2 vector2::normalized() const {
    float k = magnitude();
    if (k == 0) {
        return *this;
    }
    return vector2(x/k, y/k);
}
//
//static std::string remove_terminating_zeros(std::string str) {
//    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
//    str.erase(str.find_last_not_of('.') + 1, std::string::npos);
//    return str;
//}

std::string vector2::to_string() const {
    return ("<" + remove_terminating_zeros(std::to_string(x)) + ", " + remove_terminating_zeros(std::to_string(y)) + ">");
}

void vector2::print() const {
    std::cout << (*this).to_string() << std::endl;
}

#endif /* vector2_hpp */
