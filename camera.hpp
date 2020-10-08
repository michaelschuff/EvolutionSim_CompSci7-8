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
    vector3 position, forward, up, right, target;
    
    camera(vector3 _position, vector3 _forward, vector3 _up, vector3 _right) : position(_position), forward(_forward), up(_up), right(_right) { }
    camera(const camera &c) : position(c.position), forward(c.forward), right(c.right), up(c.up), target(c.target) { }
    
    std::vector<object2D*> get_view(std::vector<object*>);
    
    void rotate(const quaternion &q);
    void rotate(const vector3 &axis, float theta);
    
    void look_at(vector3 _position, vector3 _target, vector3 _up);
private:
    vector3 point_relative_to_camera(vector3);
    
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
            vector3 P = point_relative_to_camera(*_point);
            shapes.push_back(new circle(400+200*P.x, 400+200*P.z, 2, sf::Color(_point->c.r, _point->c.g, _point->c.b)));
            
        } else if ((_line = dynamic_cast<line*>(objects[i]))) {
            vector3 P = point_relative_to_camera(_line->head);
            vector3 Q = point_relative_to_camera(_line->tail);
            
            shapes.push_back(new rectangle(400+200*Q.x, 400+200*Q.z, sqrt(pow(200*(P.x-Q.x), 2) + pow(200*(P.z-Q.z), 2)), 1, atan2(P.z-Q.z, P.x-Q.x), sf::Color(_line->c.r, _line->c.g, _line->c.b)));
            
        } else if ((_plane = dynamic_cast<plane*>(objects[i]))) {
            
        } else if ((_triangle = dynamic_cast<triangle*>(objects[i]))) {
            
        } else if ((_mesh = dynamic_cast<mesh*>(objects[i]))) {
            
        }
    }


    return shapes;
}

vector3 camera::point_relative_to_camera(vector3 _point) {
    quaternion forward_to_y = get_quaternion(forward, vector3(0, 1, 0));
    return (_point - position).rotated(forward_to_y).rotated(get_quaternion(up.rotated(forward_to_y), vector3(0, 0, 1)));
}

void camera::rotate(const quaternion &q) {
    position.rotate(q);
    target.rotate(q);
    up.rotate(q);
    right.rotate(q);
    forward.rotate(q);
}

void camera::rotate(const vector3 &axis, float theta) {
    position.rotate(axis, theta);
    target.rotate(axis, theta);
    up.rotate(axis, theta);
    right.rotate(axis, theta);
    forward.rotate(axis, theta);
}

void camera::look_at(vector3 _position, vector3 _target, vector3 _right) {
    
}

#endif /* camera_hpp */
