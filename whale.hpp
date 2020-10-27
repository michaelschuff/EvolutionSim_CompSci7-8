#ifndef whale_hpp
#define _USE_MATH_DEFINES
using namespace sf;
using namespace std;

#include<iostream>
#include<math.h>
#include<time.h>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include "vector3.hpp"
#include "agent.hpp"
#include "fish.hpp"
#include <cstdlib>
#include <unistd.h>
#include<ctime>
#include<cmath>


/*
This file sets up the whale class, which is a child of the agent class.
*/

class whale : public agent
{
public:
    whale(int, int, vector3, vector3);
    int fishCounter; //how many fish the whale has eaten, used for ranking
    void updatePosition(double); //update position every frame based on velocity and time
    void sight (vector<fish>&); //see the fish around them and determine which ones are in a distance of them
    whaleMove (); //given a location, move towards it frame by frame
    vector<fish> decision (int); //using its traits, decide what to do on a turn
    vector<int> foodList; //reports which fish the whale might eat


private:
    //both of these are on a scale of 1-10
    int eatCloseFish; //trait that decides how much the whale is willing to move
    int eatDenseFish; //trait that determines how efficient the whale is
    int radius; //how far around the whale it can eat
    long int volume; //the volume of water the fish can eat from
    bool eat; //whether or not to eat the fish
    decisionEat (int);
    decisionMove();
};

whale::whale (int givenTraitClose, int givenTraitDense, vector3 pos, vector3 vel) : agent(pos, vel)
{
    int randChangeClose, randChangeDense, addOrSubtract;

    fishCounter = 0;
    radius = 200; //200 cm
    volume = (4.0f/3.0f) * M_PI * (float)pow(radius, 3);

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

vector<fish> whale::decision (int numFish)
{
    decisionEat(numFish);

    //eat fish
    if (eat == true)
    {
        return foodList();
    }

    //find destination to move
    else
    {
        decisionMove();
    }
}

void decisionEat(int numFish)
{
    //numFish is the size of the fishList in main
    float density;
    float percentTotal;
    eat = false;

    //see if foodList is dense enough
    density = foodList.size() / volume * 100.0f;

    if (density >= eatDenseFish)
    {
        eat = true;
    }

    //otherwise, see if the whale is fine with eating far fish
    else
    {
        percentTotal = foodList.size() / numFish * 100.0f;
        if (percentTotal >= eatCloseFish)
        {
            eat = true;
        }
    }
}

void decisionMove()
{
    //divide ocean into cubes

    //check if in densest cube

    //if yes, divide again

    //if no, move to center of densest cube
}

#endif /*whale_hpp*/
