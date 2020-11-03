//
//  matrix.hpp
//  CompSci78
//
//  Created by Michael Schuff on 10/4/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef matrix_hpp
#define matrix_hpp

#include <vector>
#include <iostream>
#include <assert.h>
#include "vector3.hpp"

using namespace std;

class matrix {
public:
    vector<vector<float>> data;
    int width, length;
    
    matrix(vector<vector<float>> _data) : data(_data), length(data.size()), width(data[0].size()) {}
    matrix(int _width, int _length) {
        width = _width;
        length = _length;
        for (int i = 0; i < _length; i++) {
            data.push_back(vector<float>());
            for (int j = 0; j < _width; j++) {
                data[i].push_back(0);
            }
        }
    }
    matrix(const matrix &m) : data(m.data), width(m.width), length(m.length) {}
    
    
    matrix operator*(const matrix &m);
    matrix operator*(const vector3 &v) const;
    matrix operator*(const float &k) const;
    matrix operator/(const float &k) const;
    matrix operator+(const matrix &m) const;
    matrix operator-(const matrix &m) const;
    matrix &operator*=(const matrix &m);
    matrix &operator*=(const vector3 &v);
    matrix &operator*=(const float &k);
    matrix &operator/=(const float &k);
    matrix &operator+=(const matrix &m);
    matrix &operator-=(const matrix &m);
    vector<float> &operator[](int i) const;
    
    vector3 row_vector(int, int);
    vector3 col_vector(int, int);
    float get(int, int) const;
    void set(int, int, float);
    
    string to_string() const;
    void print() const;
};

matrix matrix::operator*(const matrix &m) {
    assert(width==m.length);
    matrix mult(length, m.width);
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < m.width; ++j) {
            for (int k = 0; k < m.length; ++k) {
                mult.set(j, i, mult.get(j, i) + this->get(i, k) * m.get(k, j));
            }
        }
    }
    return mult;
}

matrix matrix::operator*(const vector3 &v) const {
    assert(width==3);
    matrix mult(vector<vector<float>>(length, vector<float>(1, 0)));
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < 3; j++) {
            mult[i][0] += (*this)[i][j] * v[j];
        }
    }
    return mult;
}

matrix matrix::operator*(const float &k) const {
    matrix mult(vector<vector<float>>(length, vector<float>(width, 0)));
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            mult[i][j] = data[i][j] * k;
        }
    }
    return mult;
}

matrix operator*(const float &k, const matrix &m) {
    matrix mult(vector<vector<float>>(m.length, vector<float>(m.width, 0)));
    for (int i = 0; i < m.length; i++) {
        for (int j = 0; j < m.width; j++) {
            mult[i][j] = m.data[i][j] * k;
        }
    }
    return mult;
}

matrix matrix::operator/(const float &k) const {
    return (1/k) * (*this);
}

matrix matrix::operator+(const matrix &m) const {
    assert(length==m.length && width==m.width);
    matrix mult(vector<vector<float>>(length, vector<float>(m.width, 0)));
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            mult[i][j] = data[i][j] + m[i][j];
        }
    }
    return mult;
}

matrix matrix::operator-(const matrix &m) const {
    assert(length==m.length && width==m.width);
    matrix mult(vector<vector<float>>(length, vector<float>(m.width, 0)));
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            mult[i][j] = data[i][j] - m[i][j];
        }
    }
    return mult;
}

matrix &matrix::operator*=(const matrix &m) {
    assert(width==m.length);
    data = (*this * m).data;
    return *this;
}

matrix &matrix::operator*=(const vector3 &v) {
    assert(width==3);
    data = (*this * v).data;
    return *this;
}

matrix &matrix::operator*=(const float &k) {
    data = (*this * k).data;
    return *this;
}

matrix &matrix::operator/=(const float &k) {
    data = (*this * (1/k)).data;
    return *this;
}

matrix &matrix::operator+=(const matrix &m) {
    assert(length==m.length && width==m.width);
    data = (*this + m).data;
    return *this;
}

matrix &matrix::operator-=(const matrix &m) {
    assert(length==m.length && width==m.width);
    data = (*this - m).data;
    return *this;
}

vector<float> &matrix::operator[](int i) const {
    assert(i>=0 && i<length);
    static vector<float> a = data[i];
    return a;
}

matrix to_1x4_matrix(const vector3 &v) {
    return matrix({{v.x}, {v.y}, {v.z}, {0}});
}

matrix to_4x1_matrix(const vector3 &v) {
    return matrix(vector<vector<float>> {{v.x, v.y, v.z, 0}});
}

float matrix::get(int i, int j) const {
    assert(i>=0 && i<this->length && j>=0 && j<this->width);
    return data[i][j];
}

void matrix::set(int i, int j, float val) {
    assert(i>=0 && i<length && j>=0 && j<width);
    data[i][j] = val;
}

vector3 matrix::row_vector(int i, int j) {
    assert(i>=0 && i<length);
    assert(j>=0 && j + 3<width);
    return vector3(data[i][j], data[i][j + 1], data[i][j + 2]);
}

vector3 matrix::col_vector(int i, int j) {
    assert(i>=0 && i + 3<length);
    assert(j>=0 && j<width);
    return vector3(data[i][j], data[i + 1][j], data[i + 2][j]);
}

string matrix::to_string() const {
    string s = "";
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            s += std::to_string(data[i][j]) + ", ";
        }
        s += "\n";
    }
    return s;
}

void matrix::print() const {
    cout << to_string() << endl;
}

#endif /* matrix_hpp */
