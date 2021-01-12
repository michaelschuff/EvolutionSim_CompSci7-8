//
//  loadDataFunctions.hpp
//  CompSci78
//
//  Created by Michael Schuff on 1/11/21.
//  Copyright © 2021 Michael Schuff. All rights reserved.
//

#ifndef loadDataFunctions_hpp
#define loadDataFunctions_hpp
#include <vector>
#include "line.hpp"
#include "color.hpp"
#include "vector3.hpp"
#include "triangle.hpp"
#include "mesh.hpp"
#include <fstream>
#include "whale.hpp"
#include "fish.hpp"

using namespace std;

vector<vector3> getPoints() {
    return {
        
    };
}

vector<line> getLines() {
    return {
        line(vector3(0, 0, 0), vector3(1.5, 0, 0), color(255, 0, 0)),
        line(vector3(0, 0, 0), vector3(0, 1.5, 0), color(0, 255, 0)),
        line(vector3(0, 0, 0), vector3(0, 0, 1.5), color(0, 0, 255)),
        line(vector3(0, 0, 0), vector3(100, 0, 0), color(0.5, 0.5, 0.5)),
        line(vector3(100, 0, 0), vector3(100, 0, 100), color(0.5, 0.5, 0.5)),
        line(vector3(100, 0, 100), vector3(0, 0, 100), color(0.5, 0.5, 0.5)),
        line(vector3(0, 0, 100), vector3(0, 0, 0), color(0.5, 0.5, 0.5)),
        line(vector3(0, 0, 0), vector3(0, 100, 0), color(0.5, 0.5, 0.5)),
        line(vector3(100, 0, 0), vector3(100, 100, 0), color(0.5, 0.5, 0.5)),
        line(vector3(0, 0, 100), vector3(0, 100, 100), color(0.5, 0.5, 0.5)),
        line(vector3(100, 0, 100), vector3(100, 100, 100), color(0.5, 0.5, 0.5)),
        line(vector3(0, 100, 0), vector3(100, 100, 0), color(0.5, 0.5, 0.5)),
        line(vector3(100, 100, 0), vector3(100, 100, 100), color(0.5, 0.5, 0.5)),
        line(vector3(100, 100, 100), vector3(0, 100, 100), color(0.5, 0.5, 0.5)),
        line(vector3(0, 100, 100), vector3(0, 100, 0), color(0.5, 0.5, 0.5)),
    };
}

vector<mesh> getBodies(const vector<fish>& fishList,
                       const vector<whale>& whaleList,
                       const mesh& fish_mesh,
                       const mesh& whale_mesh) {
    vector<mesh> bodies;
    for (int i = 0; i < fishList.size(); i++) {
        quaternion q = get_quaternion(vector3(1, 0, 0), fishList[i].velocity);
        bodies.push_back((fishList[i].position + fish_mesh));//.rotated(q));
    }

    for (int i = 0; i < whaleList.size(); i++) {
        quaternion q = get_quaternion(vector3(1, 0, 0), whaleList[i].velocity);
        bodies.push_back((whaleList[i].position + whale_mesh));//.rotated(q));
    }
    return bodies;
}

mesh getWhaleMesh() {
    mesh m;
    vector<vector3> whale_obj_points;
    string check;
    ifstream whale_file("/Users/michael/Downloads/whalemodel.obj");
    while (!whale_file.eof()) {
        float x, y, z;
        whale_file>>check;
        if (check == 'v') {
            whale_file>>x;
            whale_file>>y;
            whale_file>>z;
            whale_obj_points.push_back(vector3(x,y,z));
        } else if (check == 'f'){
            whale_file>>x;
            whale_file>>y;
            whale_file>>z;
            m.push_back(triangle(whale_obj_points[x-1], whale_obj_points[y-1], whale_obj_points[z-1],
                                 color((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX)));
        } else {
            string s;
            getline(whale_file, s);
        }
    }
    whale_file.close();
    return m;
}


mesh getFishMesh() {
    mesh m;
    vector<vector3> fish_obj_points;
    string check;
    ifstream fish_file("/Users/michael/Downloads/fishmodel.obj");
    while (!fish_file.eof()) {
        float x, y, z;
        fish_file>>check;
        if (check == 'v') {
            fish_file>>x;
            fish_file>>y;
            fish_file>>z;
            fish_obj_points.push_back(vector3(x,y,z));
        } else if (check == 'f'){
            fish_file>>x;
            fish_file>>y;
            fish_file>>z;
            m.push_back(triangle(fish_obj_points[x-1], fish_obj_points[y-1], fish_obj_points[z-1],
                                 color((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX)));
        } else {
            string s;
            getline(fish_file, s);
        }
    }
    fish_file.close();
    return m;
}

#endif /* loadDataFunctions_hpp */
