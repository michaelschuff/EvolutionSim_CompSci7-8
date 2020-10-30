//
//  fish.hpp
//  CompSci78
//
//  Created by Michael Schuff on 10/30/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
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

    static vector<fish>* allFishList; //pointer to vector in main. Every fish can see every other fish, so this is static

    double vision; ///could be private

    //constructor. Same as agent constructor for now.
    fish(vector3 pos, vector3 vel) : agent(pos, vel) {
//        srand(time(NULL));
        vision = 50;
    }

    
    void updateFish();

    void reportFishNumbers();

private:
    const double fishBoundary = 400;
    void fixOffScreen();
    ///these should only be used if there is more than one fish around
    //function to calculate separation velocity. Returns vector3
    vector3 vSeparation();
    //function to calculate alignment velocity. Returns vector3
    vector3 vAlignment();
    //function to calculate cohesion velocity. Returns vector3
    vector3 vCohesion();

    //void to update velocity
    void updateVelocity();
    //void to update position based on current velocity
    void updatePosition();

    //function to make vector of only other fish
    //std::vector <fish> otherFishList();

};

void fish::updateFish() {
    updateVelocity();
    updatePosition();
    fixOffScreen();
}

void fish::fixOffScreen()
{
    vector3 screenCenter = vector3(500, 500, 0);
    //if fish is off screen, move it to the opposite side and let it continue
    if(abs(position.x - screenCenter.x) >= fishBoundary || abs(position.y - screenCenter.y) >= fishBoundary || abs(position.z - screenCenter.z) >= fishBoundary) {
        velocity *= -1;
    }
}

void fish::updateVelocity() {
    velocity += (3*vSeparation() + 0.2*vAlignment() + 2*vCohesion())*0.1;
    if (velocity.magnitude() > 0) {
        velocity.normalize(); //might wanna change this
        velocity *= 1.5;
    }
}

void fish :: updatePosition() {
    position += velocity*0.5; //scale by something?
}
//should work
vector3 fish::vSeparation() {
    vector3 finalV = vector3(0, 0, 0); //will become the returned vector3

    float r = 20; ///How close fish can be to trigger this. Tweak this later!

    //Evaluate each fish in the array
    for(int ii = 0; ii < (*allFishList).size(); ii ++) {

        if((*allFishList)[ii].id == id) { continue; }

        double distAway = position.distance((*allFishList)[ii].position);

        //if dist btwn me and this fish is less than r...
        if(distAway < r && distAway > 0) {
            //adjust finalV by: vector3 of movement away from fish, but move less for farther away fish
            finalV += (position - (*allFishList)[ii].position).normalized()/distAway; //Possibly multiply by some value
        }
    }

    ///may want to scale finalV by some amount
    //finalV -= velocity;
    if (finalV.magnitude() == 0) {return vector3(0, 0, 0);}
    return finalV.normalized(); ///may want to scale differently
}
//should work
vector3 fish::vAlignment() {
    vector3 finalV = vector3(0, 0, 0);
    double fishConsidering;

    //sum all other fish velocities (if fish can be seen)
    for(int ii = 0; ii < (*allFishList).size(); ii ++) {
        if((*allFishList)[ii].id == id) {continue;}
        if(position.distance((*allFishList)[ii].position) < vision) {
            fishConsidering ++;
            finalV += (*allFishList)[ii].velocity;
        }
    }

    //divide to get average
    if(fishConsidering > 0 && finalV.magnitude() > 0) {
        finalV /= fishConsidering;
        return finalV.normalized();
    }
    return finalV;
}

//should work
vector3 fish :: vCohesion() {
    vector3 avgPos = vector3(0, 0, 0);
    double fishConsidering = 0;

    //add up all of the positions
    for(int ii = 0; ii < (*allFishList).size(); ii ++) {
        if((*allFishList)[ii].id == id) {continue;}
        if(position.distance((*allFishList)[ii].position) < vision) {
            fishConsidering ++;
            avgPos += (*allFishList)[ii].position;
        }
    }
    //make sure we don't divide by 0
    if(fishConsidering > 0) {
        avgPos /= fishConsidering;
        //find vector from current position to average position
        avgPos -= position;
    }
    //make sure we don't divide by 0
    if(avgPos.magnitude() > 0) {
        return avgPos.normalized();
    }

    return avgPos;
}

#endif /* fish_hpp */
