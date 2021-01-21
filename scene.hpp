//
//  scene.hpp
//  CompSci78
//
//  Created by Michael Schuff on 1/11/21.
//  Copyright © 2021 Michael Schuff. All rights reserved.
//

#ifndef scene_h
#define scene_h

#include <vector>
#include "triangle.hpp"
#include "mesh.hpp"
#include "vector3.hpp"
#include "camera.hpp"
#include "line.hpp"

class scene {
public:
    std::vector<vector3> points;
    std::vector<line> lines;
    std::vector<triangle> triangles;
    std::vector<mesh> meshes;
    camera main_camera;
    
    scene();
    scene(const std::vector<vector3>& t_points = std::vector<vector3>(0),
          const std::vector<line>& t_lines = std::vector<line>(0),
          const std::vector<triangle>& t_triangles = std::vector<triangle>(0),
          const std::vector<mesh>& t_meshes = std::vector<mesh>(0),
          const camera& main_cam = camera());
    
};


scene::scene(const std::vector<vector3>& t_points,
             const std::vector<line>& t_lines,
             const std::vector<triangle>& t_triangles,
             const std::vector<mesh>& t_meshes,
             const camera& main_cam) :
                points(t_points),
                lines(t_lines),
                triangles(t_triangles),
                meshes(t_meshes),
                main_camera(main_cam) {
    
}

std::vector<triangle> camera::get_view(const scene& world) {
    quaternion f2y = get_quaternion(forward, vector3(0, 1, 0));
    quaternion u2z = get_quaternion(up.rotated(f2y), vector3(0, 0, 1));
    
    std::vector<triangle> triangles;
    float t = tan(fov/2);
    float at = tan(fov/2 * (float) height/width);
#define tri triangles[triangles.size()-1]
#define v1 tri.v1
#define v2 tri.v2
#define v3 tri.v3
    for (int i = 0; i < world.lines.size(); i++) {
        triangles.push_back(triangle((world.lines[i].head - position).rotated(f2y).rotated(u2z),
                                     (world.lines[i].tail - position).rotated(f2y).rotated(u2z),
                                     vector3(),
                                     world.lines[i].c));
        
        if (v1.y >= clippingPlane || v2.y >= clippingPlane) {
            if (v1.y < clippingPlane) {
                v1 = move_to_clipping_plane(v1, v2);
            }
            
            if (v2.y < clippingPlane) {
                v2 = move_to_clipping_plane(v2, v1);
            }
            
            if (fov != 0) {
                v1.x = width/2 * (1 + v1.x/(v1.y * t));
                v1.z = height/2 * (1 + v1.z/(v1.y * at));
                v2.x = width/2 * (1 + v2.x/(v2.y * t));
                v2.z = height/2 * (1 + v2.z/(v2.y * at));
            }
        }
        v3 = v2;
    }

    for (int i = 0; i < world.triangles.size(); i++) {
        triangles.push_back((world.triangles[i] - position).rotated(f2y).rotated(u2z));
        tri.c = world.triangles[i].c;
        vector3 normal = cross_product(v2 - v1, v3 - v1);
        if (v1.y < v2.y) std::swap(v1, v2);
        if (v3.y < v1.y) std::swap(v3, v1);
        if (v3.y < v2.y) std::swap(v3, v2);
        
        if (normal.z < 0) {
            if (v1.y < clippingPlane) {
                if (v2.y < clippingPlane) {
                    if (v3.y < clippingPlane) { } else {
                        v1 = move_to_clipping_plane(v1, v3);
                        v2 = move_to_clipping_plane(v2, v3);
                        if (fov != 0) {
                            v1.x = width/2 * (1 + v1.x/(v1.y * t));
                            v1.z = height/2 * (1 + v1.z/(v1.y * at));
                            v2.x = width/2 * (1 + v2.x/(v2.y * t));
                            v2.z = height/2 * (1 + v2.z/(v2.y * at));
                            v3.x = width/2 * (1 + v3.x/(v3.y * t));
                            v3.z = height/2 * (1 + v3.z/(v3.y * at));
                        }
                        
                    }
                } else {
                    vector3 P = move_to_clipping_plane(v1, v3);
                    vector3 Q = v2;
                    vector3 R = v3;
                    vector3 S = move_to_clipping_plane(v1, v2);
                    if (fov != 0) {
                        P.x = width/2 * (1 + P.x/(P.y * t));
                        P.z = height/2 * (1 + P.z/(P.y * at));
                        Q.x = width/2 * (1 + Q.x/(Q.y * t));
                        Q.z = height/2 * (1 + Q.z/(Q.y * at));
                        R.x = width/2 * (1 + R.x/(R.y * t));
                        R.z = height/2 * (1 + R.z/(R.y * at));
                        S.x = width/2 * (1 + S.x/(S.y * t));
                        S.z = height/2 * (1 + S.z/(S.y * at));
                    }
                    color c = tri.c;
                    tri = triangle(Q, P, S, c);
                    triangles.push_back(triangle(Q, R, P, c));
                }
            } else {
                if (fov != 0) {
                    v1.x = width/2 * (1 + v1.x/(v1.y * t));
                    v1.z = height/2 * (1 + v1.z/(v1.y * at));
                    v2.x = width/2 * (1 + v2.x/(v2.y * t));
                    v2.z = height/2 * (1 + v2.z/(v2.y * at));
                    v3.x = width/2 * (1 + v3.x/(v3.y * t));
                    v3.z = height/2 * (1 + v3.z/(v3.y * at));
                }
            }
        }
    }
    
    for (int j = 0; j < world.meshes.size(); j++) {
        for (int i = 0; i < world.meshes[j].triangles.size(); i++) {
            triangles.push_back((world.meshes[j].triangles[i] - position).rotated(f2y).rotated(u2z));
            tri.c = world.meshes[j].triangles[i].c;
            vector3 normal = cross_product(v2 - v1, v3 - v1);
            if (v1.y < v2.y) std::swap(v1, v2);
            if (v3.y < v1.y) std::swap(v3, v1);
            if (v3.y < v2.y) std::swap(v3, v2);
            
            if (normal.z < 0) {
                if (v1.y < clippingPlane) {
                    if (v2.y < clippingPlane) {
                        if (v3.y < clippingPlane) { } else {
                            v1 = move_to_clipping_plane(v1, v3);
                            v2 = move_to_clipping_plane(v2, v3);
                            if (fov != 0) {
                                v1.x = width/2 * (1 + v1.x/(v1.y * t));
                                v1.z = height/2 * (1 + v1.z/(v1.y * at));
                                v2.x = width/2 * (1 + v2.x/(v2.y * t));
                                v2.z = height/2 * (1 + v2.z/(v2.y * at));
                                v3.x = width/2 * (1 + v3.x/(v3.y * t));
                                v3.z = height/2 * (1 + v3.z/(v3.y * at));
                            }
                            
                        }
                    } else {
                        vector3 P = move_to_clipping_plane(v1, v3);
                        vector3 Q = v2;
                        vector3 R = v3;
                        vector3 S = move_to_clipping_plane(v1, v2);
                        if (fov != 0) {
                            P.x = width/2 * (1 + P.x/(P.y * t));
                            P.z = height/2 * (1 + P.z/(P.y * at));
                            Q.x = width/2 * (1 + Q.x/(Q.y * t));
                            Q.z = height/2 * (1 + Q.z/(Q.y * at));
                            R.x = width/2 * (1 + R.x/(R.y * t));
                            R.z = height/2 * (1 + R.z/(R.y * at));
                            S.x = width/2 * (1 + S.x/(S.y * t));
                            S.z = height/2 * (1 + S.z/(S.y * at));
                        }
                        color c = tri.c;
                        tri = triangle(Q, P, S, c);
                        triangles.push_back(triangle(Q, R, P, c));
                    }
                } else {
                    if (fov != 0) {
                        v1.x = width/2 * (1 + v1.x/(v1.y * t));
                        v1.z = height/2 * (1 + v1.z/(v1.y * at));
                        v2.x = width/2 * (1 + v2.x/(v2.y * t));
                        v2.z = height/2 * (1 + v2.z/(v2.y * at));
                        v3.x = width/2 * (1 + v3.x/(v3.y * t));
                        v3.z = height/2 * (1 + v3.z/(v3.y * at));
                    }
                }
            }
        }
    }
#undef v1
#undef v2
#undef v3
#undef tri
    return triangles;
}
#endif /* scene_h */
