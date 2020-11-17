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
    //variables for boid movement
    float cohesion=0.1, separation=0.1, alignment=0.2, avoidance=0.1;
    int framerate;

    //how fast the fish actually move
    float speed = 20;

    //constructor. Assigns vision, framerate, etc.
    fish(vector3 pos, vector3 vel, int _framerate, int _vision = 10) : agent(pos, vel), framerate(_framerate), vision(_vision) {
//        srand(time(NULL));
    }


    void updateFish(vector<fish> &allFish, vector<vector3> &allWhalePos);

    fish &operator=(const fish &f); ///what does this even do

private:
    const float fishboundary = 100;

    float vision;

    void fixOffScreen();

    //The functions to calculate each of the velocities that contribute to boid movement. Require knowledge of where all the fish are
    vector3 vSeparation(vector<fish> &allFish);
    vector3 vAlignment(vector<fish> &allFish);
    vector3 vCohesion(vector<fish> &allFish);

    vector3 vAvoidWhales(vector<vector3> &allWhalePos);

    //function to update velocity
    void updateVelocity(vector<fish> &allFish, vector<vector3> &allWhalePos);
    //function to update position based on current velocity
    void updatePosition();

};

///Way to set one fish's movement values to those of another fish (I think)
fish &fish::operator=(const fish &f) {
    cohesion=f.cohesion;
    separation=f.separation;
    alignment=f.alignment;
    vision = f.vision;
    framerate = f.framerate;
    speed = f.speed;
    return *this;
}

//Calls all functions necessary to this one fish
void fish::updateFish(vector<fish> &allFish, vector<vector3> &allWhalePos) {
    updateVelocity(allFish, allWhalePos);
    fixOffScreen();
    updatePosition();
}

//If fish are offscreen, this function fixes it. You can uncomment the portion with the behavior you want
void fish::fixOffScreen() {
    //if fish is off screen, move it to the opposite side and let it continue. Defines boundary to be cube starting at (0, 0, 0) and ending at (fishBoundary, fishBoundary, fishBoundary)
    position %= fishboundary;

/*  //Makes fish "bounce" of of edges. Admittedly is subject to a little error. Defines boundary as cube starting at (0, 0, 0) and ending at (fishBoundary, fishBoundary, fishBoundary)
    if(position.x > fishboundary || position.x < 0 || position.y > fishboundary || position.y < 0 || position.z > fishboundary || position.z < 0) {
        velocity = -speed * (position - 0.5 * vector3(fishboundary, fishboundary, fishboundary)).normalized();
    }
    */
}

//updates the velocity variable
void fish::updateVelocity(vector<fish> &allFish, vector<vector3> &allWhalePos) {
    velocity = speed * (avoidance * vAvoidWhales(allWhalePos) + separation*vSeparation(allFish) + alignment*vAlignment(allFish) + cohesion*vCohesion(allFish)).normalized();
}

//updates the position variable. Call after updateVelocity
void fish::updatePosition() {
    position += velocity / framerate;
}


/* ************BOID MOVEMENT FUNCTIONS***********************/
vector3 fish::vAvoidWhales(vector<vector3> &allWhalePos) {
    vector3 finalV;
    float r = vision * 2;

    for(int ii = 0; ii < allWhalePos.size(); ii ++) {

        float dist = position.distance(allWhalePos[ii]);

        if(dist < r) {
            finalV += (position - allWhalePos[ii]) / (dist*dist);
        }
    }

    return finalV;
}

//Velocity for RULE: fish should move away from close fish
vector3 fish::vSeparation(vector<fish> &allFish) {

    vector3 finalV; //will become the returned vector3

    //radius of personal "space bubble"
    float r = vision*0.99;
    //Evaluate each fish in the array
    for (int ii = 0; ii < allFish.size(); ii ++) {
        //check that the fish isn't me
        if(allFish[ii].id != id) {

            float dist = position.distance(allFish[ii].position);

            if (dist < r) {
                //adjust finalV by: vector3 of movement away from fish, but move less for farther away fish
                finalV += (position - allFish[ii].position) / (dist*dist);
            }
        }
    }
    return finalV;
}
//Velocity for RULE: fish should move in the same direction as nearby fish
vector3 fish::vAlignment(vector<fish> &allFish) {

    vector3 finalV;//vector3 that will get returned

    //sum all other fish velocities (if fish can be seen), but add less for farther away velocities
    for(int ii = 0; ii < allFish.size(); ii++) {
        //make sure that this fish isn't me (and has a different id)
        if(allFish[ii].id != id) {

            double dist = position.distance(allFish[ii].position);
            if (dist < vision) {
                finalV += allFish[ii].velocity / (dist*dist);
            }
        }
    }

    return finalV;
}

//Velocity for RULE: fish should swim to the center of their local flock
vector3 fish::vCohesion(vector<fish> &allFish) {
    vector3 avgPos;
    int fishConsidering = 0; //how many fish are within my range of vision

    //goes through each fish and adds the positions of all local fish to avgPos
    for (int ii = 0; ii < allFish.size(); ii++) {
        //make sure this fish isn't me and has a different id
        if(allFish[ii].id != id) {

            float dist = position.distance(allFish[ii].position);
            if (dist < vision) {

                fishConsidering++;
                avgPos += allFish[ii].position;
            }
        }
    }
    //check that fishConsidering isn't 0, b/c we're dividing avgPos by it
    if(fishConsidering > 0) {
        avgPos /= fishConsidering; //turns avgPos to the actual average position

        avgPos -= position;//find vector from current position to average position
    }

    return avgPos; //returns vector from current position to average position
}

#endif /* fish_hpp */
