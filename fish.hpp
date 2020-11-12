//
//  fish.hpp
//  CompSci78
//
//  Created by Elaine Demetrion on 10/30/20.
//  Copyright © 2020 Elaine Demetrion. All rights reserved.
//

#ifndef fish_hpp
#define fish_hpp

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "vector3.hpp"
#include "agent.hpp"

using namespace std;

class fish : public agent {
public:
    float cohesion=0.1, separation=0.1, alignment=0.2;
    float vision; ///could be private
    int framerate;
    float speed = 20;
    //constructor. Same as agent constructor for now.
    fish(vector3 pos, vector3 vel, int _framerate, int _vision = 10) : agent(pos, vel), framerate(_framerate), vision(_vision) {
//        srand(time(NULL));
    }


    void updateFish(vector<fish> &allFish);

    void reportFishNumbers();
    fish &operator=(const fish &f);

private:
    const float fishboundary = 100;
    void fixOffScreen();
    ///these should only be used if there is more than one fish around
    //function to calculate separation velocity. Returns vector3
    vector3 vSeparation(vector<fish> &allFish);
    //function to calculate alignment velocity. Returns vector3
    vector3 vAlignment(vector<fish> &allFish);
    //function to calculate cohesion velocity. Returns vector3
    vector3 vCohesion(vector<fish> &allFish);

    //void to update velocity
    void updateVelocity(vector<fish> &allFish);
    //void to update position based on current velocity
    void updatePosition();

    //function to make vector of only other fish
    //std::vector <fish> otherFishList();

};

fish &fish::operator=(const fish &f) {
    cohesion=f.cohesion;
    separation=f.separation;
    alignment=f.alignment;
    vision = f.vision;
    framerate = f.framerate;
    speed = f.speed;
    return *this;
}

void fish::updateFish(vector<fish> &allFish) {
    updateVelocity(allFish);
    fixOffScreen();
    updatePosition();
}

void fish::fixOffScreen() {
//    vector3 screenCenter = vector3(500, 500, 0);
//    //if fish is off screen, move it to the opposite side and let it continue
    position %= fishboundary;
//    if(position.x > fishboundary || position.x < 0 || position.y > fishboundary || position.y < 0 || position.z > fishboundary || position.z < 0) {
//        velocity = -speed * (position - 0.5 * vector3(fishboundary, fishboundary, fishboundary)).normalized();
//    }
}

void fish::updateVelocity(vector<fish> &allFish) {
    velocity = speed * (separation*vSeparation(allFish) + alignment*vAlignment(allFish) + cohesion*vCohesion(allFish)).normalized();
//    float x = ((float) rand() / RAND_MAX);
//    float y = ((float) rand() / RAND_MAX);
//    float z = ((float) rand() / RAND_MAX);
//    velocity += 0.1*vector3(x, y, z);
}

void fish::updatePosition() {
    position += velocity / framerate;
}
//should work
vector3 fish::vSeparation(vector<fish> &allFish) {
    vector3 finalV; //will become the returned vector3
    //Evaluate each fish in the array
    float r = vision;
    for (int ii = 0; ii < allFish.size(); ii ++) {
        if(allFish[ii].id != id) {
            float dist = position.distance(allFish[ii].position);

            //if dist btwn me and this fish is less than r...
            if (dist < r) {
                //adjust finalV by: vector3 of movement away from fish, but move less for farther away fish
                finalV += (position - allFish[ii].position) / (dist*dist);
            }
        }
    }
    return finalV;
}
//should work
vector3 fish::vAlignment(vector<fish> &allFish) {
    vector3 finalV;

    //sum all other fish velocities (if fish can be seen)
    for(int ii = 0; ii < allFish.size(); ii++) {
        if(allFish[ii].id != id) {
            double dist = position.distance(allFish[ii].position);
            if (dist < vision) {
                finalV += allFish[ii].velocity / (dist*dist);
            }
        }
    }

    return finalV;
}

//should work
vector3 fish::vCohesion(vector<fish> &allFish) {
    vector3 avgPos;
    int fishConsidering = 0;

    for (int ii = 0; ii < allFish.size(); ii++) {
        if(allFish[ii].id != id) {
            float dist = position.distance(allFish[ii].position);
            if (dist < vision) {
                fishConsidering++;
                avgPos += allFish[ii].position;
            }
        }
    }

    if(fishConsidering > 0) {
        avgPos /= fishConsidering;
        //find vector from current position to average position
        avgPos -= position;
    }

    return avgPos;
}

#endif /* fish_hpp */
