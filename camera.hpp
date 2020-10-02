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
#include <math.h>
#include <iostream>
#include "vector3.hpp"
#include "color.hpp"

class camera {
public:
    const int width, height;
    double roll, theta, phi;
    vector3 position; //direction vector is also the normal vector of the screen
    
    camera(const camera &c);
    camera(vector3, int, int, double, double, double);
    
    void set_position(vector3);
    void look_at(vector3);
    void inc_phi(double);
    void dec_phi(double);
    void inc_theta(double);
    void dec_theta(double);
//    void int_roll(double);
    
    std::vector<sf::CircleShape> get_screen(std::vector<vector3>) const;

    vector3 normal;
};

camera::camera(const camera &c) : position(c.position), width(c.width), height(c.height), theta(c.theta), phi(c.phi), roll(c.roll) {
}

camera::camera(vector3 pos, int w, int h, double _theta, double _phi, double _roll) : position(pos), width(w), height(h), theta(_theta), phi(_phi), roll(_roll) {
    normal.x = sin(_phi)*cos(_theta);
    normal.y = sin(_phi)*sin(_theta);
    normal.z = cos(_phi);
}

std::vector<sf::CircleShape> camera::get_screen(std::vector<vector3> points) const {
    std::vector<sf::CircleShape> circles;
    for (int i = 0; i < points.size(); i++) {
        vector3 P(points[i] - (*this).position);
        P -= projection(P, normal);
        P.rotate(vector3(0, 0, 1), -theta);
        P.rotate(vector3(0, 1, 0), -phi);
        
        sf::CircleShape c(2);
        c.setOrigin(2, 2);
        c.setPosition(400+200*P.x, 400+200*P.y);
        
//        if (i == 0) {
//            c.setFillColor(sf::Color(255, 0, 0));
//            P.print();
//        }
        circles.push_back(c);
        if (dot_product(P, normal) > 0) {
            
        }
    }
    return circles;
}

void camera::set_position(vector3 pos) {
    position = pos;
}

void camera::look_at(vector3 point) {
    normal = (point - position).normalized();
    theta = atan2(normal.y, normal.x);
    phi = atan2(sqrt(pow(normal.x, 2) + pow(normal.y, 2)), normal.z);
}

void camera::inc_phi(double k = 1.0 * M_PI / 180.0) {
    phi += k;
    while (phi >= 2 * M_PI) {
        phi -= 2 * M_PI;
    }
    normal.x = sin(phi)*cos(theta);
    normal.y = sin(phi)*sin(theta);
    normal.z = cos(phi);
}

void camera::dec_phi(double k = 1.0 * M_PI / 180.0) {
    phi -= k;
    while (phi < 0) {
        phi += 2 * M_PI;
    }
    normal.x = sin(phi)*cos(theta);
    normal.y = sin(phi)*sin(theta);
    normal.z = cos(phi);
}

void camera::inc_theta(double k = 1.0 * M_PI / 180.0) {
    theta += k;
    while (theta >= 2 * M_PI) {
        theta -= 2 * M_PI;
    }
    normal.x = sin(phi)*cos(theta);
    normal.y = sin(phi)*sin(theta);
    normal.z = cos(phi);
}

void camera::dec_theta(double k = 1.0 * M_PI / 180.0) {
    theta -= k;
    while (theta < 0) {
        theta += 2 * M_PI;
    }
    normal.x = sin(phi)*cos(theta);
    normal.y = sin(phi)*sin(theta);
    normal.z = cos(phi);
}
#endif /* camera_hpp */
