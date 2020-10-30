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

class whale : public agent
{
public:
    whale(int, int, vector3, vector3, vector3);
    int fishCounter; //how many fish the whale has eaten, used for ranking
    void updatePosition(double); //update position every frame based on velocity and time
    whaleMove (); //given a location, move towards it frame by frame
    void decision (vector<fish>&); //using its traits, decide what to do on a turn
    vector<int> foodList; //reports which fish the whale might eat
    bool eat; //whether or not to eat the fish
    //both of these are on a scale of 1-10
    int eatCloseFish; //trait that decides how much the whale is willing to move
    int eatDenseFish; //trait that determines how efficient the whale is

private:

    int radius; //how far around the whale it can eat
    long int volume; //the volume of water the fish can eat from
    void decisionEat (int);
    void decisionMove();
    vector3 edges; //the bottom corner of the board
    void sight (vector<fish>&); //see the fish around them and determine which ones are in a distance of them
};

whale::whale (int givenTraitClose, int givenTraitDense, vector3 pos, vector3 vel, vector3 boundary) : agent(pos, vel)
{
    int randChangeClose, randChangeDense, addOrSubtract;

    fishCounter = 0;
    radius = 50; //200 cm
    volume = (4.0f/3.0f) * M_PI * (float)pow(radius, 3);
    edges = boundary;

//set up traits with some randomness, based on a given initial value
    randChangeClose = (rand() % 5) - 2;
    randChangeDense = (rand() % 5) - 2;

    eatCloseFish = givenTraitClose + randChangeClose;
    eatDenseFish = givenTraitDense + randChangeDense;

    if (eatCloseFish > 10)
    {
        eatCloseFish = 10;
    }
    else if (eatCloseFish < 1)
    {
        eatCloseFish = 1;
    }
    if (eatDenseFish > 10)
    {
        eatDenseFish = 10;
    }
    else if (eatDenseFish < 1)
    {
        eatDenseFish = 1;
    }

//set velocity and position
    velocity = vel;
    position = pos;
}

void whale::updatePosition (double timeDiff)
{
    position.x += velocity.x * timeDiff;
    position.y += velocity.y * timeDiff;
    position.z += velocity.z * timeDiff;

    //check if beyond boundaries
    if (position.x > edges.x or position.x < 0)
    {
        position.x = abs(position.x - edges.x);
    }

    if (position.y > edges.y or position.y < 0)
    {
        position.y = abs(position.y - edges.y);
    }

    if (position.z > edges.z or position.z < 0)
    {
        position.z = abs(position.z - edges.z);
    }
}

void whale::sight (vector<fish> &fishList)
{
    vector3 difference;
    double actualRadius;

    foodList.clear();

    for (int ff = 0; ff < fishList.size(); ff ++)
    {
        //find the differences in x, y, and z values
        difference.x = fishList[ff].position.x - position.x;
        difference.y = fishList[ff].position.y - position.y;
        difference.z = fishList[ff].position.z - position.z;

        //use 3D Pythagorean theorem to find the radius value
        actualRadius = sqrt(pow(difference.x, 2) + pow(difference.y, 2)+ pow(difference.z, 2));

        //if it's less than r add that fish's id to the list
        if (actualRadius <= radius)
        {
            foodList.push_back(fishList[ff].id);
        }
    }
}

void whale::decision (vector<fish> &fishList)
{
    sight(fishList);
    decisionEat(fishList.size());

    //eat fish
    if (eat == true)
    {
        //don't move, in main go through foodList and remove those IDs
        fishCounter += foodList.size();
    }

    //find destination to move
    else
    {
        decisionMove();
    }
}

void whale::decisionEat(int numFish)
{
    //numFish is the size of the fishList in main
    float density;
    float percentTotal;
    eat = false;

    //see if foodList is dense enoughs
    density = ((float)foodList.size() / (float)volume) * 1000000.0f;

    if (density >= eatDenseFish)
    {
        eat = true;
    }

    //otherwise, see if the whale is fine with eating far fish
    else
    {
        percentTotal = ((float)foodList.size() / (float)numFish) * 100.0f;

        if (percentTotal >= eatCloseFish)
        {
            eat = true;
        }
    }
}

void whale::decisionMove()
{
    vector3 tempVel;
    float randX, randY, randZ;

    //random movement if not eating
    while (true)
    {
        //generate random values for velocity
        randX = (rand() % 5) -2;
        randY = (rand() % 5) -2;
        randZ = (rand() % 5) -2;

        if (randX != 0 & randY != 0 & randZ != 0)
        {
            break;
        }
    }

    tempVel.x = randX;
    tempVel.y = randY;
    tempVel.z = randZ;

    //normalize velocity
    tempVel.normalize();

    //scale it to 2
    velocity.x = tempVel.x * 2;
    velocity.y = tempVel.y * 2;
    velocity.z = tempVel.z * 2;
}

#endif /* whale_hpp */
