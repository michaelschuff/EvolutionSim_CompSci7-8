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
    float fov;
    float clippingPlane;
    int width = 800, height = 800;
    
    camera(vector3 _position, vector3 _forward, vector3 _up, vector3 _right, float _fov = 3.14159/3, float _clippingPlane = 0.5) : position(_position), forward(_forward), up(_up), right(_right), fov(_fov), clippingPlane(_clippingPlane) { }
    camera(const camera &c) : position(c.position), forward(c.forward), right(c.right), up(c.up), target(c.target), fov(c.fov) { }
    
    std::vector<object2D*> get_view(std::vector<object*>);
    
    void rotate(const quaternion &q);
    void rotate(const vector3 &axis, float theta);
    void rotate_around(const vector3 &axis, float theta);
    
    void look_at(vector3 _position, vector3 _target, vector3 _up);
private:
    vector3 point_relative_to_camera(vector3);
    vector3 move_to_clipping_plane(vector3, vector3);
    void sort(std::vector<object*> &objects, std::vector<object2D*> &shapes);
    template <class T>
    void swap(vector<T> v, int x, int y);
    bool first_is_in_front(triangle &t1, triangle &t2);
    bool first_is_in_front(triangle &tri, line &l);
    bool first_is_in_front(line &l, triangle &tri);
    bool first_is_in_front(vector3 &v, triangle &tri);
    bool first_is_in_front(triangle &tri, vector3 &v);
    bool first_is_in_front(line &l1, line &l2) {return true;};
    bool first_is_in_front(vector3 &v1, vector3 &v2) {return true;}
    bool first_is_in_front(vector3 &v, line &l) {return true;};
    bool first_is_in_front(line &l, vector3 &v) {return true;};
};

std::vector<object2D*> camera::get_view(std::vector<object*> objects) {
    std::vector<object2D*> shapes;
    
    vector3* _point = nullptr;
    line* _line = nullptr;
    triangle* _triangle = nullptr;
    mesh* _mesh = nullptr;
    for (int i = 0; i < objects.size(); i++) {  
        if ((_point = dynamic_cast<vector3*>(objects[i]))) {
            vector3 P = point_relative_to_camera(*_point);
            if (P.y > clippingPlane) {
                if (fov != 0) {
                    P /= (P.y*tan(fov/2));
                }
                shapes.push_back(new circle(P.x, P.z, 1, sf::Color(_point->c.r, _point->c.g, _point->c.b)));
            }
            
        } else if ((_line = dynamic_cast<line*>(objects[i]))) {
            vector3 P = point_relative_to_camera(_line->head);
            vector3 Q = point_relative_to_camera(_line->tail);
            
            if (P.y < clippingPlane) {
                P = move_to_clipping_plane(P, Q);
            }
            
            if (Q.y < clippingPlane) {
                Q = move_to_clipping_plane(Q, P);
            }
            if (fov != 0) {
                if (P.y != 0) {
                    P /= (P.y*tan(fov/2));
                }
                if (Q.y != 0) {
                    Q /= (Q.y*tan(fov/2));
                }
            }
            
            shapes.push_back(new rectangle(Q.x, Q.z, sqrt(pow((P.x-Q.x), 2) + pow((P.z-Q.z), 2)), 1, atan2(P.z-Q.z, P.x-Q.x), sf::Color(_line->c.r, _line->c.g, _line->c.b)));
            
        } else if ((_triangle = dynamic_cast<triangle*>(objects[i]))) {
            vector3 P(point_relative_to_camera(_triangle->v1)),
                    Q(point_relative_to_camera(_triangle->v2)),
                    R(point_relative_to_camera(_triangle->v3)),
                    S(0, 0, 0);
            vector3 normal = cross_product(Q - P, R - P);
            if (P.y < Q.y) std::swap(P, Q);
            if (R.y < P.y) std::swap(R, P);
            if (R.y < Q.y) std::swap(R, Q);
            
            if (normal.z < 0) {
                if (P.y < clippingPlane) {
                    if (Q.y < clippingPlane) {
                        if (R.y < clippingPlane) { } else {
                            P = move_to_clipping_plane(P, R);
                            Q = move_to_clipping_plane(Q, R);
                            if (fov != 0) {
                                P.x /= P.y * tan(fov/2);
                                P.z /= P.y * tan(fov/2 * (float) height/width);
                                Q.x /= Q.y * tan(fov/2);
                                Q.z /= Q.y * tan(fov/2 * (float) height/width);
                                R.x /= R.y * tan(fov/2);
                                R.z /= R.y * tan(fov/2 * (float) height/width);
                            }
                            shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                        }
                    } else {
                        S = move_to_clipping_plane(P, Q);
                        P = move_to_clipping_plane(P, R);
                        if (fov != 0) {
                            P.x /= P.y * tan(fov/2);
                            P.z /= P.y * tan(fov/2 * (float) height/width);
                            Q.x /= Q.y * tan(fov/2);
                            Q.z /= Q.y * tan(fov/2 * (float) height/width);
                            R.x /= R.y * tan(fov/2);
                            R.z /= R.y * tan(fov/2 * (float) height/width);
                            S.x /= S.y * tan(fov/2);
                            S.z /= S.y * tan(fov/2 * (float) height/width);
                        }
                        shapes.push_back(new convexshape({P, S, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                    }
                } else {
                    if (fov != 0) {
                        P.x /= P.y * tan(fov/2);
                        P.z /= P.y * tan(fov/2 * (float) height/width);
                        Q.x /= Q.y * tan(fov/2);
                        Q.z /= Q.y * tan(fov/2 * (float) height/width);
                        R.x /= R.y * tan(fov/2);
                        R.z /= R.y * tan(fov/2 * (float) height/width);
                    }
                    shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                }
            }
        } else if ((_mesh = dynamic_cast<mesh*>(objects[i]))) {
            for (int i = 0; i < _mesh->size(); i++) {
                vector3 P(point_relative_to_camera(_triangle->v1)),
                        Q(point_relative_to_camera(_triangle->v2)),
                        R(point_relative_to_camera(_triangle->v3)),
                        S(0, 0, 0);
                vector3 normal = cross_product(Q - P, R - P);
                if (P.y < Q.y) std::swap(P, Q);
                if (R.y < P.y) std::swap(R, P);
                if (R.y < Q.y) std::swap(R, Q);
                
                if (normal.z < 0) {
                    if (P.y < clippingPlane) {
                        if (Q.y < clippingPlane) {
                            if (R.y < clippingPlane) { } else {
                                P = move_to_clipping_plane(P, R);
                                Q = move_to_clipping_plane(Q, R);
                                if (fov != 0) {
                                    P.x /= P.y * tan(fov/2);
                                    P.z /= P.y * tan(fov/2 * (float) height/width);
                                    Q.x /= Q.y * tan(fov/2);
                                    Q.z /= Q.y * tan(fov/2 * (float) height/width);
                                    R.x /= R.y * tan(fov/2);
                                    R.z /= R.y * tan(fov/2 * (float) height/width);
                                }
                                shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                            }
                        } else {
                            S = move_to_clipping_plane(P, Q);
                            P = move_to_clipping_plane(P, R);
                            if (fov != 0) {
                                P.x /= P.y * tan(fov/2);
                                P.z /= P.y * tan(fov/2 * (float) height/width);
                                Q.x /= Q.y * tan(fov/2);
                                Q.z /= Q.y * tan(fov/2 * (float) height/width);
                                R.x /= R.y * tan(fov/2);
                                R.z /= R.y * tan(fov/2 * (float) height/width);
                                S.x /= S.y * tan(fov/2);
                                S.z /= S.y * tan(fov/2 * (float) height/width);
                            }
                            shapes.push_back(new convexshape({P, S, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                        }
                    } else {
                        if (fov != 0) {
                            P.x /= P.y * tan(fov/2);
                            P.z /= P.y * tan(fov/2 * (float) height/width);
                            Q.x /= Q.y * tan(fov/2);
                            Q.z /= Q.y * tan(fov/2 * (float) height/width);
                            R.x /= R.y * tan(fov/2);
                            R.z /= R.y * tan(fov/2 * (float) height/width);
                        }
                        shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                    }
                }
            }
        }
    }
    return shapes;
}

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
