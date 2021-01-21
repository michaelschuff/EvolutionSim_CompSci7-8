//
//  camera.hpp
//  CompSci78
//
//  Created by Michael Schuff on 10/1/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <SFML/Graphics.hpp>
#include <vector>
#include <typeinfo>
#include <math.h>
#include <iostream>
#include "object.hpp"
#include "color.hpp"
#include "vector3.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "mesh.hpp"
#include "objects2d.hpp"
#include "quaternion.hpp"
#include "matrix.hpp"

class scene;
class camera {
public:
    vector3 position, forward, up, right, target;
    float fov;
    float clippingPlane;
    int width = 800, height = 800;
    
    camera() : position(vector3(0, 0, -5)), forward(vector3(0, 0, 1)), up(vector3(0, 1, 0)), right(vector3(1, 0, 0)), fov(3.14159/3), clippingPlane(0.3) { }
    camera(vector3 _position, vector3 _forward, vector3 _up, vector3 _right, float _fov = 3.14159/3, float _clippingPlane = 0.5) : position(_position), forward(_forward), up(_up), right(_right), fov(_fov), clippingPlane(_clippingPlane) { }
    camera(const camera &c) : position(c.position), forward(c.forward), right(c.right), up(c.up), target(c.target), fov(c.fov) { }
    
    std::vector<triangle> get_view(const scene&);
    
    void rotate(const quaternion &q);
    void rotate(const vector3 &axis, float theta);
    void rotate_around(const vector3 &axis, float theta);
    
    void look_at(vector3 _position, vector3 _target, vector3 _up);
private:
    vector3 point_relative_to_camera(vector3);
    vector3 move_to_clipping_plane(vector3, vector3);
    template <class T>
    void swap(vector<T> v, int x, int y);
};

vector3 camera::point_relative_to_camera(vector3 _point) {
    vector3 P(_point - position);
    quaternion forward_to_y = get_quaternion(forward, vector3(0, 1, 0));
    quaternion up_to_z = get_quaternion(up.rotated(forward_to_y), vector3(0, 0, 1));
//    quaternion right_to_x = get_quaternion(right.rotated(forward_to_y).rotated(up_to_z), vector3(1, 0, 0));
    
    return P.rotated(forward_to_y).rotated(up_to_z);//.rotated(right_to_x);
}

vector3 camera::move_to_clipping_plane(vector3 v1, vector3 v2) {
    return v1 + ((clippingPlane - v1.y) / (v2.y - v1.y)) * (v2-v1);
}

void camera::rotate(const quaternion &q) {
    up.rotate(q);
    right.rotate(q);
    forward.rotate(q);
}

void camera::rotate(const vector3 &axis, float theta) {
    if (theta == 0) {
        return;
    }
    up.rotate(axis, theta);
    right.rotate(axis, theta);
    forward.rotate(axis, theta);
}

void camera::rotate_around(const vector3 &axis, float theta) {
    if (theta == 0) {
        return;
    }
    position.rotate(axis, theta);
    up.rotate(axis, theta);
    right.rotate(axis, theta);
    forward.rotate(axis, theta);
}

void camera::look_at(vector3 _position, vector3 _target, vector3 _right) {
    
}

template <class T>
void swap(vector<T> v, int x, int y) {
    T temp = v[x];
    v[x] = v[y];
    v[y] = temp;

}

#endif /* camera_hpp */
