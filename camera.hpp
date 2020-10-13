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
    int width, height;
    
    camera(vector3 _position, vector3 _forward, vector3 _up, vector3 _right, float _fov = M_PI/3, float _clippingPlane = 0.5) : position(_position), forward(_forward), up(_up), right(_right), fov(_fov), clippingPlane(_clippingPlane) { }
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
//    shapes.reserve(objects.size());
    std::vector<object*> objs;
    
    vector3* _point = nullptr;
    line* _line = nullptr;
    plane* _plane = nullptr;
    triangle* _triangle = nullptr;
    mesh* _mesh = nullptr;
    for (int i = 0; i < objects.size(); i++) {
        
        if ((_point = dynamic_cast<vector3*>(objects[i]))) {
            vector3 P = point_relative_to_camera(*_point);
            if (P.y > clippingPlane) {
                objs.push_back(new vector3(P));
                if (fov != 0) {
                    P /= (P.y*tan(fov/2));
                }
                shapes.push_back(new circle(P.x, P.z, 1, sf::Color(_point->c.r, _point->c.g, _point->c.b)));
            }
            
        } else if ((_line = dynamic_cast<line*>(objects[i]))) {
            vector3 P = point_relative_to_camera(_line->head);
            vector3 Q = point_relative_to_camera(_line->tail);
            
            objs.push_back(new line(Q, P));
            
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
            
        } else if ((_plane = dynamic_cast<plane*>(objects[i]))) {
//            vector3 P(point_relative_to_camera(_plane->top_left)),
//                    Q(point_relative_to_camera(_plane->top_right)),
//                    R(point_relative_to_camera(_plane->bottom_right)),
//                    S(point_relative_to_camera(_plane->bottom_left));
//
//            if (fov != 0) {
//                P /= (abs(P.y)*tan(fov/2));
//                Q /= (abs(Q.y)*tan(fov/2));
//                R /= (abs(R.y)*tan(fov/2));
//                S /= (abs(S.y)*tan(fov/2));
//            }
//            shapes.push_back(new convexshape({P, Q, R, S}, sf::Color(_plane->c.r, _plane->c.g, _plane->c.b)));
        } else if ((_triangle = dynamic_cast<triangle*>(objects[i]))) {
            vector3 P(point_relative_to_camera(_triangle->v1)),
                    Q(point_relative_to_camera(_triangle->v2)),
                    R(point_relative_to_camera(_triangle->v3)),
                    S(0, 0, 0);
            
            
            if (P.y < clippingPlane) {
                if (Q.y < clippingPlane) {
                    if (R.y < clippingPlane) {
                        
                    } else {
                        objs.push_back(new triangle(P, Q, R));
                        P = move_to_clipping_plane(P, R);
                        Q = move_to_clipping_plane(Q, R);
                        if (fov != 0) {
                            P /= (P.y*tan(fov/2));
                            Q /= (Q.y*tan(fov/2));
                            R /= (R.y*tan(fov/2));
                        }
                        shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                    }
                    
                } else if (R.y < clippingPlane) {
                    objs.push_back(new triangle(P, Q, R));
                    P = move_to_clipping_plane(P, Q);
                    R = move_to_clipping_plane(R, Q);
                    if (fov != 0) {
                        P /= (P.y*tan(fov/2));
                        Q /= (Q.y*tan(fov/2));
                        R /= (R.y*tan(fov/2));
                    }
                    shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                } else {
                    objs.push_back(new triangle(P, Q, R));
                    S = move_to_clipping_plane(P, Q);
                    P = move_to_clipping_plane(P, R);
                    if (fov != 0) {
                        P /= (P.y*tan(fov/2));
                        S /= (S.y*tan(fov/2));
                        Q /= (Q.y*tan(fov/2));
                        R /= (R.y*tan(fov/2));
                    }
                    shapes.push_back(new convexshape({P, S, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                }
            } else if (R.y < clippingPlane) {
                if (Q.y < clippingPlane) {
                    objs.push_back(new triangle(P, Q, R));
                    R = move_to_clipping_plane(R, P);
                    Q = move_to_clipping_plane(Q, P);
                    if (fov != 0) {
                        P /= (P.y*tan(fov/2));
                        Q /= (Q.y*tan(fov/2));
                        R /= (R.y*tan(fov/2));
                    }
                    shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                } else {
                    objs.push_back(new triangle(P, Q, R));
                    S = move_to_clipping_plane(R, P);
                    R = move_to_clipping_plane(R, Q);
                    if (fov != 0) {
                        P /= (P.y*tan(fov/2));
                        Q /= (Q.y*tan(fov/2));
                        R /= (R.y*tan(fov/2));
                        S /= (S.y*tan(fov/2));
                    }
                    shapes.push_back(new convexshape({P, Q, R, S}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                }
            } else if (Q.y < clippingPlane) {
                objs.push_back(new triangle(P, Q, R));
                S = move_to_clipping_plane(Q, R);
                Q = move_to_clipping_plane(Q, P);
                if (fov != 0) {
                    P /= (P.y*tan(fov/2));
                    Q /= (Q.y*tan(fov/2));
                    S /= (S.y*tan(fov/2));
                    R /= (R.y*tan(fov/2));
                }
                shapes.push_back(new convexshape({P, Q, S, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
            } else {
                objs.push_back(new triangle(P, Q, R));
                if (fov != 0) {
                    P /= (P.y*tan(fov/2));
                    Q /= (Q.y*tan(fov/2));
                    R /= (R.y*tan(fov/2));
                }
                shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
            }
            
            
        } else if ((_mesh = dynamic_cast<mesh*>(objects[i]))) {
            for (int i = 0; i < _mesh->size(); i++) {
                vector3 P(point_relative_to_camera(((*_mesh)[i]).v1)),
                        Q(point_relative_to_camera(((*_mesh)[i]).v2)),
                        R(point_relative_to_camera(((*_mesh)[i]).v3)),
                        S(0, 0, 0);
                
                if (P.y < clippingPlane) {
                    if (Q.y < clippingPlane) {
                        if (R.y < clippingPlane) {
                            
                        } else {
                            objs.push_back(new triangle(P, Q, R));
                            P = move_to_clipping_plane(P, R);
                            Q = move_to_clipping_plane(Q, R);
                            if (fov != 0) {
                                P /= (P.y*tan(fov/2));
                                Q /= (Q.y*tan(fov/2));
                                R /= (R.y*tan(fov/2));
                            }
                            shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                        }
                    } else if (R.y < clippingPlane) {
                        objs.push_back(new triangle(P, Q, R));
                        P = move_to_clipping_plane(P, Q);
                        R = move_to_clipping_plane(R, Q);
                        if (fov != 0) {
                            P /= (P.y*tan(fov/2));
                            Q /= (Q.y*tan(fov/2));
                            R /= (R.y*tan(fov/2));
                        }
                        shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                    } else {
                        objs.push_back(new triangle(P, Q, R));
                        S = move_to_clipping_plane(P, Q);
                        P = move_to_clipping_plane(P, R);
                        if (fov != 0) {
                            P /= (P.y*tan(fov/2));
                            S /= (S.y*tan(fov/2));
                            Q /= (Q.y*tan(fov/2));
                            R /= (R.y*tan(fov/2));
                        }
                        shapes.push_back(new convexshape({P, S, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                    }
                } else if (R.y < clippingPlane) {
                    if (Q.y < clippingPlane) {
                        objs.push_back(new triangle(P, Q, R));
                        R = move_to_clipping_plane(R, P);
                        Q = move_to_clipping_plane(Q, P);
                        if (fov != 0) {
                            P /= (P.y*tan(fov/2));
                            Q /= (Q.y*tan(fov/2));
                            R /= (R.y*tan(fov/2));
                        }
                        shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                    } else {
                        objs.push_back(new triangle(P, Q, R));
                        S = move_to_clipping_plane(R, P);
                        R = move_to_clipping_plane(R, Q);
                        if (fov != 0) {
                            P /= (P.y*tan(fov/2));
                            Q /= (Q.y*tan(fov/2));
                            R /= (R.y*tan(fov/2));
                            S /= (S.y*tan(fov/2));
                        }
                        shapes.push_back(new convexshape({P, Q, R, S}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                    }
                } else if (Q.y < clippingPlane) {
                    objs.push_back(new triangle(P, Q, R));
                    S = move_to_clipping_plane(Q, R);
                    Q = move_to_clipping_plane(Q, P);
                    if (fov != 0) {
                        P /= (P.y*tan(fov/2));
                        Q /= (Q.y*tan(fov/2));
                        S /= (S.y*tan(fov/2));
                        R /= (R.y*tan(fov/2));
                    }
                    shapes.push_back(new convexshape({P, Q, S, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                } else {
                    objs.push_back(new triangle(P, Q, R));
                    if (fov != 0) {
                        P /= (P.y*tan(fov/2));
                        Q /= (Q.y*tan(fov/2));
                        R /= (R.y*tan(fov/2));
                    }
                    shapes.push_back(new convexshape({P, Q, R}, sf::Color(_triangle->c.r, _triangle->c.g, _triangle->c.b)));
                }
            }
        }
    }
    sort(objs, shapes);
    return shapes;
}

vector3 camera::point_relative_to_camera(vector3 _point) {
    vector3 P(_point - position);
    quaternion forward_to_y = get_quaternion(forward, vector3(0, 1, 0));
    quaternion up_to_z = get_quaternion(up.rotated(forward_to_y), vector3(0, 0, 1));
    quaternion right_to_x = get_quaternion(right.rotated(forward_to_y).rotated(up_to_z), vector3(1, 0, 0));
    return P.rotated(forward_to_y).rotated(up_to_z).rotated(right_to_x);
}

vector3 camera::move_to_clipping_plane(vector3 v1, vector3 v2) {
    return v1 + ((clippingPlane - v1.y) / (v2.y - v1.y)) * (v2-v1);
}

void camera::rotate(const quaternion &q) {
    position.rotate(q);
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



bool first_is_in_front(line &l1, line &l2) {return true;};
bool first_is_in_front(vector3 &v1, vector3 &v2) {return true;}
bool first_is_in_front(vector3 &v, line &l) {return true;};
bool first_is_in_front(line &l, vector3 &v) {return true;};

bool first_is_in_front(triangle &t1, triangle &t2) {
    vector<vector<vector3>> points = {
        {   (t1.v1 + t2.v1)/2,
            (t1.v2 + t2.v2)/2,
            (t1.v3 + t2.v3)/2   },
        
        {   (t1.v1 + t2.v1)/2,
            (t1.v2 + t2.v3)/2,
            (t1.v3 + t2.v2)/2   },
        
        {   (t1.v1 + t2.v2)/2,
            (t1.v2 + t2.v1)/2,
            (t1.v3 + t2.v3)/2   },
        
        {   (t1.v1 + t2.v2)/2,
            (t1.v2 + t2.v3)/2,
            (t1.v3 + t2.v1)/2   },
        
        {   (t1.v1 + t2.v3)/2,
            (t1.v2 + t2.v1)/2,
            (t1.v3 + t2.v2)/2   },
        
        {   (t1.v1 + t2.v3)/2,
            (t1.v2 + t2.v2)/2,
            (t1.v3 + t2.v1)/2   },

    };
    vector<float> mags = {
        (t1.v1 - t2.v1).magnitude() + (t1.v2 - t2.v2).magnitude() + (t1.v3 - t2.v3).magnitude(),
        (t1.v1 - t2.v1).magnitude() + (t1.v2 - t2.v3).magnitude() + (t1.v3 - t2.v2).magnitude(),
        
        (t1.v1 - t2.v2).magnitude() + (t1.v2 - t2.v1).magnitude() + (t1.v3 - t2.v3).magnitude(),
        (t1.v1 - t2.v2).magnitude() + (t1.v2 - t2.v3).magnitude() + (t1.v3 - t2.v1).magnitude(),
        
        (t1.v1 - t2.v3).magnitude() + (t1.v2 - t2.v1).magnitude() + (t1.v3 - t2.v2).magnitude(),
        (t1.v1 - t2.v3).magnitude() + (t1.v2 - t2.v2).magnitude() + (t1.v3 - t2.v1).magnitude(),
    };
    vector3 p1, p2, p3;
    float min = mags[0];
    p1 = points[0][0];
    p2 = points[0][1];
    p3 = points[0][2];
    for (int i = 1; i < 6; i++) {
        if (mags[i] < min) {
            min = mags[i];
            p1 = points[i][0];
            p2 = points[i][1];
            p3 = points[i][2];
        }
    }
    
    
    vector3 normal = cross_product(p2 - p1, p3 - p1);
    if (normal.y < 0) {
        normal = -normal;
    }
    float val1 = (dot_product(normal, (t1.v1 - p1).normalized()) + dot_product(normal, (t1.v2 - p1).normalized()) + dot_product(normal, (t1.v3 - p1).normalized())) / 3;
    float val2 = (dot_product(normal, (t2.v1 - p1).normalized()) + dot_product(normal, (t2.v2 - p1).normalized()) + dot_product(normal, (t2.v3 - p1).normalized())) / 3;
    return val1 > val2;
}

bool first_is_in_front(triangle &tri, line &l) {
    vector3 p1 = (tri.v1 + l.head) / 2,
            p2 = (tri.v2 + l.head) / 2,
            p3 = (tri.v3 + l.tail) / 2;
    vector3 normal = cross_product(p2 - p1, p3 - p1);
    if (normal.y < 0) {
        normal = -normal;
    }//average dot products of normal and all triangle points and find highest value
    float val1 = (dot_product(normal, (tri.v1 - p1).normalized()) + dot_product(normal, (tri.v2 - p1).normalized()) + dot_product(normal, tri.v3 - p1)) / 3;
    float val2 = (dot_product(normal, (l.tail - p1).normalized()) + dot_product(normal, (l.head - p1).normalized())) / 2;
    return val1 > val2;
}

bool first_is_in_front(line &l, triangle &tri) {
    return !first_is_in_front(tri, l);
}

bool first_is_in_front(vector3 &v, triangle &tri) {
    vector3 p1 = (tri.v1 + v) / 2,
            p2 = (tri.v2 + v) / 2,
            p3 = (tri.v3 + v) / 2;
    vector3 normal = cross_product(p2 - p1, p3 - p1);
    if (normal.y < 0) {
        normal = -normal;
    }
    float val1 = (dot_product(normal, (tri.v1 - p1).normalized()) + dot_product(normal, (tri.v2 - p1).normalized()) + dot_product(normal, (tri.v3 - p1).normalized())) / 3;
    float val2 = dot_product(normal, (v - p1).normalized());
    return val1 > val2;
}

bool first_is_in_front(triangle &tri, vector3 &v) {
    return !first_is_in_front(v, tri);
}
bool check(object* temp1, std::vector<object*> &objects, int i, int j) {
    vector3* p1 = nullptr;
    vector3* p2 = nullptr;
    line* l1 = nullptr;
    line* l2 = nullptr;
    triangle* t1 = nullptr;
    triangle* t2 = nullptr;
    if ((p1 = dynamic_cast<vector3*>(temp1))) {
        if ((p2 = dynamic_cast<vector3*>(objects[j-1]))) {
            if (!first_is_in_front(*p1, *p2)) {
                return true;
            }
        } else if ((l2 = dynamic_cast<line*>(objects[j-1]))) {
            if (!first_is_in_front(*p1, *l2)) {
                return true;
            }
        } else if ((t2 = dynamic_cast<triangle*>(objects[j-1]))) {
            if (!first_is_in_front(*p1, *t2)) {
                return true;
            }
        }
    } else if ((l1 = dynamic_cast<line*>(temp1))) {
        if ((p2 = dynamic_cast<vector3*>(objects[j-1]))) {
            if (!first_is_in_front(*l1, *p2)) {
                return true;
            }
        } else if ((l2 = dynamic_cast<line*>(objects[j-1]))) {
            if (!first_is_in_front(*l1, *l2)) {
                return true;
            }
        } else if ((t2 = dynamic_cast<triangle*>(objects[j-1]))) {
            if (!first_is_in_front(*l1, *t2)) {
                return true;
            }
        }
    } else if ((t1 = dynamic_cast<triangle*>(temp1))) {
        if ((p2 = dynamic_cast<vector3*>(objects[j-1]))) {
            if (!first_is_in_front(*t1, *p2)) {
                return true;
            }
        } else if ((l2 = dynamic_cast<line*>(objects[j-1]))) {
            if (!first_is_in_front(*t1, *l2)) {
                return true;
            }
        } else if ((t2 = dynamic_cast<triangle*>(objects[j-1]))) {
            if (!first_is_in_front(*t1, *t2)) {
                return true;
            }
        }
    }
    return false;
}
void camera::sort(std::vector<object*> &objects, std::vector<object2D*> &shapes) {
    int i, j;
    object *temp1;
    object2D *temp2;
    for (int i = 1; i < objects.size(); i++) {
        j = i;
        temp1 = objects[i];
        temp2 = shapes[i];
        while (j > 0) {
            if (check(temp1, objects, i, j)) {
                break;
            }
            objects[j] = objects[j-1];
            shapes[j] = shapes[j-1];
            j--;
            
        }
        objects[j] = temp1;
        shapes[j] = temp2;
    }
    
}

template <class T>
void swap(vector<T> v, int x, int y) {
    T temp = v[x];
    v[x] = v[y];
    v[y] = temp;

}

#endif /* camera_hpp */
