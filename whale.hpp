//
//  whale.hpp
//  CompSci78
//
//  Created by Michael Schuff on 10/30/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef whale_hpp
#define whale_hpp

using namespace sf;
using namespace std;

#include<iostream>
#include<math.h>
#include<time.h>
#include<vector>
#include "vector3.hpp"
#include "agent.hpp"
#include "fish.hpp"
#include <cstdlib>
#include<cmath>


/*
This file sets up the whale class, which is a child of the agent class.
*/

class whale : public agent {
public:
    whale(int, int, vector3, vector3, vector3, int);
    int framerate;
    int fishCounter; //how many fish the whale has eaten, used for ranking
    void updatePosition(); //update position every frame based on velocity and time
    void whaleMove(); //given a location, move towards it frame by frame
    void decision (vector<fish>&); //using its traits, decide what to do on a turn
    vector<int> foodList; //reports which fish the whale might eat
    bool eat; //whether or not to eat the fish
    //both of these are on a scale of 1-10
    int eatCloseFish; //trait that decides how much the whale is willing to move
    int eatDenseFish; //trait that determines how efficient the whale is
    int age;
    int speed = 20;

private:
    int radius; //how far around the whale it can eat
    long int volume; //the volume of water the fish can eat from
    void decisionEat (int);
    void decisionMove();
    vector3 edges; //the bottom corner of the board
    void sight (vector<fish>&); //see the fish around them and determine which ones are in a distance of them
};

whale::whale(int givenTraitClose, int givenTraitDense, vector3 pos, vector3 vel, vector3 boundary, int _framerate) : agent(pos, vel), framerate(_framerate) {
    int randChangeClose, randChangeDense, addOrSubtract;

    fishCounter = 0;
    radius = 3;
    volume = (float) (4.0/3.0) * M_PI * pow(radius, 3);
    edges = boundary;
    age = 0;

    //set up traits with some randomness, based on a given initial value
    randChangeClose = (rand() % 5) - 2;
    randChangeDense = (rand() % 5) - 2;

    eatCloseFish = givenTraitClose + randChangeClose;
    eatDenseFish = givenTraitDense + randChangeDense;

    if (eatCloseFish > 10) {
        eatCloseFish = 10;
    } else if (eatCloseFish < 1) {
        eatCloseFish = 1;
    }
    
    if (eatDenseFish > 10) {
        eatDenseFish = 10;
    } else if (eatDenseFish < 1) {
        eatDenseFish = 1;
    }

    //set velocity and position
    velocity = vel;
    position = pos;
}

void whale::updatePosition () {
    position += speed * velocity / framerate;

    //check if beyond boundaries
    position %= edges.x;
//    if (position.x > edges.x or position.x < 0) {
//        position.x = abs(position.x - edges.x);
//    }
//
//    if (position.y > edges.y or position.y < 0) {
//        position.y = abs(position.y - edges.y);
//    }
//
//    if (position.z > edges.z or position.z < 0) {
//        position.z = abs(position.z - edges.z);
//    }
}

void whale::sight(vector<fish> &fishList) {
    foodList.clear();
    for (int f = 0; f < fishList.size(); f++) {
        //use 3D Pythagorean theorem to find the radius value
        float actualRadius = (fishList[f].position - position).magnitude();

        //if it's less than r add that fish's id to the list
        if (actualRadius <= radius) {
            foodList.push_back(fishList[f].id);
        }
    }
}

void whale::decision(vector<fish> &fishList) {
    sight(fishList);
    decisionEat(fishList.size());

    age++;
    
    if (eat == true) {//eat fish
        //don't move, in main go through foodList and remove those IDs
        fishCounter += foodList.size();
    } else { //find destination to move
        decisionMove();
    }
}

void whale::decisionEat(int numFish) {
    //numFish is the size of the fishList in main
    eat = false;

    //see if foodList is dense enoughs
    float density = ((float)foodList.size() / (float)volume) * 1000000.0f;
    float percentTotal = ((float)foodList.size() / (float)numFish) * 100.0f;

    if (density >= eatDenseFish || percentTotal >= eatCloseFish) {
        eat = true;
    }
}

void whale::decisionMove() {
    
    float drandom = 10;
    float randPhi = (rand() % ((int) drandom*2)) - drandom;
    float randTheta = (rand() % ((int) drandom*2)) - drandom;
    velocity.rotate(vector3(0, 1, 0), randTheta * 3.14159 / 180.0);
    vector3 temp(-velocity.z, 0, velocity.x);
    velocity.rotate(temp, randPhi * 3.14159 / 180.0);
}

#endif /* whale_hpp */
