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

class camera {
public:
    vector3 position, forward, right, up, target;
    matrix world_to_cam = matrix(4, 4);
    
    camera(vector3 _position, vector3 _target, vector3 _up) : position(_position), target(_target), up(_up) { look_at(_position, _target, _up); }
    camera(const camera &c) : position(c.position), forward(c.forward), right(c.right), up(c.up), target(c.target), world_to_cam(c.world_to_cam) {}
    
    std::vector<object2D*> get_view(std::vector<object*>);
    
    void rotate(const quaternion &q);
    void rotate(const vector3 &axis, float theta);
    
    void look_at(vector3 _position, vector3 _target, vector3 _up);
    
private:
    void update_world_to_cam_matrix();
};

std::vector<object2D*> camera::get_view(std::vector<object*> objects) {
    std::vector<object2D*> shapes;
    shapes.reserve(objects.size());
    
    vector3* _point = nullptr;
    line* _line = nullptr;
    plane* _plane = nullptr;
    triangle* _triangle = nullptr;
    mesh* _mesh = nullptr;
    for (int i = 0; i < objects.size(); i++) {
        if ((_point = dynamic_cast<vector3*>(objects[i]))) {
//            vector3 P(*_point);
            vector3 P = (world_to_cam * to_1x4_matrix(*_point)).row_vector(0, 0);
            P.print();
            shapes.push_back(new circle(400+200*P.z, 400+200*P.y, 2, sf::Color(_point->c.r, _point->c.g, _point->c.b)));
            
        } else if ((_line = dynamic_cast<line*>(objects[i]))) {
//            vector3 P = (world_to_cam * to_1x4_matrix((_line->tail - (*this).position))).row_vector(0, 0);
            
//            vector3 Q = (world_to_cam * to_1x4_matrix((_line->head - (*this).position))).row_vector(0, 0);
            
            
//            shapes.push_back(new rectangle(400+200*Q.x, 400+200*Q.z, sqrt(pow(200*(P.x-Q.x), 2) + pow(200*(P.z-Q.z), 2)), 1, atan2(P.z-Q.z, P.x-Q.x), sf::Color(_line->c.r, _line->c.g, _line->c.b)));
            
        } else if ((_plane = dynamic_cast<plane*>(objects[i]))) {
            
        } else if ((_triangle = dynamic_cast<triangle*>(objects[i]))) {
            
        } else if ((_mesh = dynamic_cast<mesh*>(objects[i]))) {
            
        }
    }


    return shapes;
}

void camera::rotate(const quaternion &q) {
    forward.rotate(q);
    right.rotate(q);
    position.rotate(q);
    up.rotate(q);
    update_world_to_cam_matrix();
}

void camera::rotate(const vector3 &axis, float theta) {
    forward.rotate(axis, theta);
    right.rotate(axis, theta);
    position.rotate(axis, theta);
    up.rotate(axis, theta);
    update_world_to_cam_matrix();
}

void camera::look_at(vector3 _position, vector3 _target, vector3 _right) {
    _right.normalize();
    target = _target;
    position = _position;
    forward = (_target - _position).normalized();
    right = (_right - dot_product(_right, forward) * forward).normalized();
    up = cross_product(forward, right);
    update_world_to_cam_matrix();
}

void camera::update_world_to_cam_matrix() {
//        world_to_cam.data = {
//            {right.x, right.y, right.z, 0},
//            {up.x, up.y, up.z, 0},
//            {forward.x, forward.y, forward.z, 0},
//            {position.x, position.y, position.z, 1},
//        };
    world_to_cam.data = {
            {right.x, up.x, forward.x, 0},
            {right.y, up.y, forward.y, 0},
            {right.z, up.z, forward.z, 0},
            {-dot_product(right, position), -dot_product(up, position), -dot_product(forward, position), 1}};
}
#endif /* camera_hpp */
