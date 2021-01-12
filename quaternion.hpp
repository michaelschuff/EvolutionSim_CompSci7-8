//
//  quaternion.hpp
//  CompSci78
//
//  Created by Michael Schuff on 10/4/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef quaternion_hpp
#define quaternion_hpp

#include <assert.h>
#include <math.h>
#include "vector3.hpp"


class quaternion {
public:
    float r, i, j, k;
    vector3 v;
    
    quaternion(const float &_r, const float &_i, const float &_j, const float &_k) : r(_r), i(_i), j(_j), k(_k), v(vector3(_i, _j, _k)) {}
    quaternion(const float &_r, const vector3 &_v) : r(_r), i(_v.x), j(_v.y), k(_v.z), v(_v) {}
    
    quaternion operator+(const quaternion &q) const;
    quaternion operator-();
    quaternion operator-(const quaternion &q) const;
    quaternion operator*(const quaternion &q) const;
    quaternion operator*(const float &c) const;
    quaternion operator/(const float &c) const;
    quaternion &operator+=(const quaternion &q);
    quaternion &operator-=(const quaternion &q);
    quaternion &operator*=(const quaternion &q);
    quaternion &operator*=(const float &c);
    quaternion &operator/=(const float &c);
    quaternion &operator=(const quaternion &q);
    float &operator[](const int &c);
    
    void normalize();
    quaternion normalized() const;
    float norm() const;
    quaternion conjugate() const;
    quaternion inverse() const;
    quaternion reciprocal() const;
};

quaternion quaternion::operator+(const quaternion &q) const {
    return quaternion(r+q.r, i+q.i, j+q.j, k+q.k);
}

quaternion &quaternion::operator+=(const quaternion &q) {
    r += q.r;
    i += q.i;
    j += q.j;
    k += q.k;
    return *this;
}

quaternion quaternion::operator-(const quaternion &q) const {
    return quaternion(r-q.r, i-q.i, j-q.j, k-q.k);
}

quaternion &quaternion::operator-=(const quaternion &q) {
    r -= q.r;
    i -= q.i;
    j -= q.j;
    k -= q.k;
    return *this;
}

quaternion quaternion::operator*(const float &c) const {
    return quaternion(c*r, c*i, c*j, c*k);
}

quaternion operator*(const float &c, const quaternion &q) {
    return quaternion(c*q.r, c*q.i, c*q.j, c*q.k);
}

quaternion &quaternion::operator*=(const float &c) {
    r *= c;
    i *= c;
    j *= c;
    k *= c;
    return *this;
}

quaternion quaternion::operator/(const float &c) const {
//    assert(c!=0);
    return quaternion(r/c, i/c, j/c, k/c);
}

quaternion &quaternion::operator/=(const float &c) {
    assert(c!=0);
    r /= c;
    i /= c;
    j /= c;
    k /= c;
    return *this;
}

quaternion &quaternion::operator=(const quaternion &q) {
    r = q.r;
    i = q.i;
    j = q.j;
    k = q.k;
    return *this;
}

quaternion quaternion::operator-() {
    return quaternion(-r, -i, -j, -k);
}

quaternion get_quaternion(const vector3 &v1, const vector3 &v2) {
    vector3 a = cross_product(v1, v2);
    return quaternion(v1.magnitude() * v2.magnitude() + dot_product(v1, v2), a.x, a.y, a.z).normalized();
}

float &quaternion::operator[](const int &c) {
    assert(c>=0 && c<4);
    switch (c) {
        case 0:
            return r;
        case 1:
            return i;
        case 2:
            return j;
        case 3:
            return k;
    }
}

quaternion quaternion::operator*(const quaternion &q) const {
    return quaternion(r*q.r - i*q.i - j*q.j - k*q.k,
                      r*q.i + i*q.r + j*q.k - k*q.j,
                      r*q.j - i*q.k + j*q.r + k*q.i,
                      r*q.k + i*q.j - j*q.i + k*q.r);
}

float quaternion::norm() const {
    return sqrt(r*r + i*i + j*j + k*k);
}

quaternion quaternion::conjugate() const {
    return quaternion(r, -i, -j, -k);
}

quaternion quaternion::inverse() const {
    return conjugate() / (r*r + i*i + j*j + k*k);
}

void quaternion::normalize() {
    (*this) /= norm();
}

quaternion quaternion::normalized() const {
    return (*this) / norm();
}

quaternion quaternion::reciprocal() const {
    return conjugate() / pow(norm(), 2);
}

float distance(const quaternion &q1, const quaternion &q2) {
    return (q1-q2).norm();
}

quaternion operator-(const vector3 &v, const float &c) {
    return quaternion(-c, v);
}

quaternion operator-(const float &c, const vector3 &v) {
    return quaternion(c, -v);
}

quaternion operator+(const vector3 &v, const float &c) {
    return quaternion(c, v);
}

quaternion operator+(const float &c, const vector3 &v) {
    return quaternion(c, v);
}

quaternion operator*(const vector3 &v1, const vector3 &v2) {
    return cross_product(v1, v2) - dot_product(v1, v2);
}

vector3 vector3::rotated(const quaternion &q) const {
    return (q * (0 + *this) * q.conjugate()).v;
}

vector3 vector3::rotated(const vector3& axis, float theta) const {
    return rotated(cos(theta/2) + axis.normalized() * sin(theta)/2);
}

vector3 vector3::rotated(const vector3& axis, float theta, const vector3& origin) const {
    return ((*this - origin).rotated(cos(theta/2) + axis.normalized() * sin(theta)/2)) + origin;
}

void vector3::rotate(const vector3& axis, float theta) {
    rotate(cos(theta/2) + axis.normalized() * sin(theta)/2);
}

void vector3::rotate(const vector3& axis, float theta, const vector3& origin) {
    vector3 v = this->rotated(axis, theta, origin);
    x = v.x;
    y = v.y;
    z = v.z;
}

void vector3::rotate(const quaternion &q) {
    quaternion result = q * (0 + *this) * q.conjugate();
    x = result.i;
    y = result.j;
    z = result.k;
}


#endif /* quaternion_hpp */
