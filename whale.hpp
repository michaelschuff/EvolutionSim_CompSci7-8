//
//  whale.hpp
//  CompSci78
//
//  Created by Bronte McKinnis
//  Copyright © 2020 Bronte McKinnis. All rights reserved.
//

#ifndef whale_hpp
#define whale_hpp

using namespace sf;
using namespace std;

#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
#include "vector3.hpp"
#include "agent.hpp"
#include "fish.hpp"
#include <cstdlib>
#include <cmath>


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
    int age;
    int speed;

    //both of these are on a scale of 1-10
    int eatCloseFish; //trait that decides how much the whale is willing to move
    int eatDenseFish; //trait that determines how efficient the whale is

private:
    int radius; //how far around the whale it can eat
    long int volume; //the volume of water the fish can eat from
    void decisionEat (int);
    void decisionMove(vector<fish>&);
    vector3 edges; //the bottom corner of the board
    void sight (vector<fish>&); //see the fish around them and determine which ones are in a distance of them
    int division; //the number of times the cube has been divided for movement
    vector<int> fishDiv1; //the number of fish per cube
    vector<int> fishDiv2;
    vector<int> fishDiv3;
    vector<int> fishDiv4;
    int chosenQuadrant;
    vector3 tempLimits; //for movement
    int direction; //the direction of division for movement
    int maxDiv; //the maximum amount of times the "ocean" can be divided
    vector3 destination; //where the whale wants to go
};

whale::whale(int givenTraitClose, int givenTraitDense, vector3 pos, vector3 vel, vector3 boundary, int _framerate) : agent(pos, vel), framerate(_framerate) {
    int randChangeClose, randChangeDense;

    fishCounter = 0;
    radius = 5;
    volume = (float) (4.0/3.0) * M_PI * pow(radius, 3);
    edges = boundary;
    age = 0;
    speed = 20;
    direction = 1;
    tempLimits = boundary;
    division = 0;
    maxDiv = 0;
    destination = (50.0f, 50.0f, 50.0f);
    int currentRadius;


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

    //find maxDiv using radius
    while (currentRadius > radius)
    {
        currentRadius = boundary.x / pow(2, maxDiv +1);
        maxDiv ++;
    }
}

void whale::updatePosition () {
    position += speed * velocity / framerate;

    //check if beyond boundaries
    position %= edges.x;
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

    //eat fish
    if (eat == true) {
        //don't move, in main go through foodList and remove those IDs
        fishCounter += foodList.size();
    }
    //find destination to move
    else {
        decisionMove(fishList);
    }
}

void whale::decisionEat(int numFish) {
    //numFish is the size of the fishList in main
    eat = false;

    //see if foodList is dense enough
    float density = ((float)foodList.size() / (float)volume) * 1000.0f;

    //see if fish are close enough considering all fish
    float percentTotal = ((float)foodList.size() / (float)numFish) * 1000.0f;

    if (density >= eatDenseFish || percentTotal >= eatCloseFish) {
        eat = true;
    }
}

void whale::decisionMove(vector<fish> &fishList) {
    /*
    //move randomly, rotating a little each time
    float drandom = 5;
    float randPhi = (rand() % ((int) drandom*2)) - drandom;
    float randTheta = (rand() % ((int) drandom*2)) - drandom;
    velocity.rotate(vector3(0, 1, 0), randTheta * 3.14159 / 180.0);
    vector3 temp(-velocity.z, 0, velocity.x);
    velocity.rotate(temp, randPhi * 3.14159 / 180.0);
    */

    //!assumption: the limit is the same in x, y, and z directions (ocean is a cube)
    //see if we are dividing to make cubes smaller or larger
    if (division == maxDiv)
    {
        direction = -1;
    }
    else if (division == 0)
    {
        direction = 1;
    }

    if (direction == 1)
    {
        tempLimits = tempLimits / 2.0f;
        division ++;
    }
    else
    {
        tempLimits = tempLimits * 2.0f;
        division --;
    }

    //go through all fish and see if they're in divisions
    for (int ff = 0; ff < fishList.size(); ff++)
    {
        //if(fishList[ff].position.x)
    }

}

#endif /* whale_hpp */
