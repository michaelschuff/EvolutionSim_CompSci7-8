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

class camera {
public:
    const int width, height;
    double roll, theta, phi;
    vector3 position, normal;
    
    camera(const camera &c);
    camera(vector3, int, int, double, double, double);
    
    void set_position(vector3);
    void look_at(vector3);
    void inc_phi(double);
    void dec_phi(double);
    void inc_theta(double);
    void dec_theta(double);
    void inc_roll(double);
    void dec_roll(double);
    
    std::vector<object2D*> get_screen(std::vector<object*>);
};

camera::camera(const camera &c) : position(c.position), width(c.width), height(c.height), theta(c.theta), phi(c.phi), roll(c.roll) {
    
}

camera::camera(vector3 pos, int _width, int _height, double _theta = 0, double _phi = 0, double _roll = 0) : position(pos), width(_width), height(_height), theta(_theta), phi(_phi), roll(_roll) {
    normal.x = sin(_phi)*cos(_theta);
    normal.y = sin(_phi)*sin(_theta);
    normal.z = cos(_phi);
//    screen = plane(pos, normal, _width, _height, _roll, false);
}

std::vector<object2D*> camera::get_screen(std::vector<object*> objects) {
    std::vector<object2D*> shapes;
    shapes.reserve(objects.size());
    
    vector3* _point = nullptr;
    line* _line = nullptr;
    plane* _plane = nullptr;
    triangle* _triangle = nullptr;
    mesh* _mesh = nullptr;
    
    for (int i = 0; i < objects.size(); i++) {
        if ((_point = dynamic_cast<vector3*>(objects[i]))) {
            vector3 P((*_point) - (*this).position);
            P -= projection(P, normal);
            P.rotate(vector3(0, 0, 1), -theta);
            P.rotate(vector3(0, 1, 0), -phi);
            P.rotate(vector3(0, 0, 1), roll);
            shapes.push_back(new circle(400+200*P.x, 400+200*P.y, 2, sf::Color(_point->c.r, _point->c.g, _point->c.b)));
            
        } else if ((_line = dynamic_cast<line*>(objects[i]))) {
            vector3 P(_line->tail - (*this).position);
            P -= projection(P, normal);
            P.rotate(vector3(0, 0, 1), -theta);
            P.rotate(vector3(0, 1, 0), -phi);
            P.rotate(vector3(0, 0, 1), roll);
            
            vector3 Q(_line->head - (*this).position);
            Q -= projection(Q, normal);
            Q.rotate(vector3(0, 0, 1), -theta);
            Q.rotate(vector3(0, 1, 0), -phi);
            Q.rotate(vector3(0, 0, 1), roll);
            shapes.push_back(new rectangle(400+200*Q.x, 400+200*Q.y, sqrt(pow(200*(P.x-Q.x), 2) + pow(200*(P.y-Q.y), 2)), 1, atan2(P.y-Q.y, P.x-Q.x), sf::Color(_line->c.r, _line->c.g, _line->c.b)));
            
        } else if ((_plane = dynamic_cast<plane*>(objects[i]))) {
            double th = normal.theta(), ph = normal.phi();
            vector3 P(_plane->width / 2, _plane->height / 2, 0);
            vector3 Q(-_plane->width / 2, _plane->height / 2, 0);
            vector3 R(-_plane->width / 2, -_plane->height / 2, 0);
            vector3 S(_plane->width / 2, -_plane->height / 2, 0);
            
            
            P.rotate(vector3(0, 0, 1), th);
            P.rotate(vector3(0, 1, 0), ph);
            Q.rotate(vector3(0, 0, 1), th);
            Q.rotate(vector3(0, 1, 0), ph);
            R.rotate(vector3(0, 0, 1), th);
            R.rotate(vector3(0, 1, 0), ph);
            S.rotate(vector3(0, 0, 1), th);
            S.rotate(vector3(0, 1, 0), ph);
            
            P -= (*this).position;
            P -= projection(P, normal);
            P.rotate(vector3(0, 0, 1), -theta);
            P.rotate(vector3(0, 1, 0), -phi);
            P.rotate(vector3(0, 0, 1), roll);
            
            Q -= (*this).position;
            Q -= projection(Q, normal);
            Q.rotate(vector3(0, 0, 1), -theta);
            Q.rotate(vector3(0, 1, 0), -phi);
            Q.rotate(vector3(0, 0, 1), roll);
            
            R -= (*this).position;
            R -= projection(R, normal);
            R.rotate(vector3(0, 0, 1), -theta);
            R.rotate(vector3(0, 1, 0), -phi);
            R.rotate(vector3(0, 0, 1), roll);
            
            S -= (*this).position;
            S -= projection(S, normal);
            S.rotate(vector3(0, 0, 1), -theta);
            S.rotate(vector3(0, 1, 0), -phi);
            S.rotate(vector3(0, 0, 1), roll);
            
//            shapes.push_back(new convexshape(std::vector<std::vector<double>> {{P.x, P.y}, {Q.x, Q.y}, {R.x, R.y}, {S.x, S.y}}, sf::Color(255, 255, 255)));
        } else if ((_triangle = dynamic_cast<triangle*>(objects[i]))) {
            
        } else if ((_mesh = dynamic_cast<mesh*>(objects[i]))) {
            
        }
    }


    return shapes;
}

void camera::set_position(vector3 pos) {
    position = pos;
//    screen.position = pos;
}

void camera::look_at(vector3 point) {
    normal = (point - position);
    theta = normal.theta();
    phi = normal.phi();
}
#endif /* camera_hpp */
