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
    vector3 destination; //where the whale wants to go
    bool closeEnough(vector3, vector3, int);
    vector<int> sight(vector<fish>&, int);

};

whale::whale(int givenTraitClose, int givenTraitDense, vector3 pos, vector3 vel, vector3 boundary, int _framerate) : agent(pos, vel), framerate(_framerate) {
    int randChangeClose, randChangeDense;

    fishCounter = 0;
    radius = 5;
    volume = (float) (4.0/3.0) * M_PI * pow(radius, 3);
    edges = boundary;
    age = 0;
    speed = 20;
    vector3 destination (50, 50, 50);
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
}

void whale::updatePosition () {
    position += speed * velocity / framerate;

    //check if beyond boundaries
    position %= edges.x;
}

void whale::decision(vector<fish> &fishList) {
    foodList.clear();
    foodList = sight(fishList, radius);
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

    int fishInReach = 0;

    //go through all fish
    for (int ff = 0; ff < fishList.size(); ff++)
    {
        fishInReach = 0;

        //see if the fish is close enough
        if (closeEnough(fishList[ff].position, position, eatCloseFish * radius))
        {
            //fish sees density of nearby fish (within a mouthful)
            for (int of = 0; of < fishList.size(); of ++)
            {
                if (closeEnough(fishList[ff].position, fishList[of].position, radius))
                {
                    fishInReach ++;
                }
            }

            //if the fish works, set destination to that fish and break
            if (((float)fishInReach / (float)volume) >= (eatDenseFish * 1000.0f))
            {
                destination = fishList[ff].position;

                break;
            }

        }
    }

}

vector<int> whale::sight(vector<fish> &fishList, int maxDist) {

    vector<int> fishWork;

    for (int f = 0; f < fishList.size(); f++) {

        if (closeEnough(fishList[f].position, position, maxDist)) {
            fishWork.push_back(fishList[f].id);
        }
    }

    return fishWork;
}

bool whale::closeEnough(vector3 otherPos, vector3 myPos, int maxDist)
{
    bool withinDist = false;

    //use 3D Pythagorean theorem to find the radius value
    float actualRadius = (otherPos - myPos).magnitude();

    //if it's less than r add that fish's id to the list
    if (actualRadius <= maxDist) {
        withinDist = true;
    }

    return withinDist;
}

#endif /* whale_hpp */
