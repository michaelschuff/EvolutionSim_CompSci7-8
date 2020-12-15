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
    void updatePosition(vector<fish>&); //update position every frame based on velocity and time
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
    void decisionEat (int); //sees if the fish around it match its traits to eat
    void decisionMove(vector<fish>&); //finds a new target fish
    vector3 edges; //the bottom corner of the board
    vector3 destination; //where the whale wants to go
    int fishTarget; //the fish whose location is the destination
    bool closeEnough(vector3, vector3, int); //given 2 positions and a radius, compares distance
    vector<int> sight(vector<fish>&, int); //sees which fish are within eating distance
    void updateDestination (vector<fish>&); //updates based on target fish
    int frameCounter;
    int reverseCloseFish; //used to determine if whale eats
};

whale::whale(int givenTraitClose, int givenTraitDense, vector3 pos, vector3 vel, vector3 boundary, int _framerate) : agent(pos, vel),framerate(_framerate) {
    int randChangeClose, randChangeDense;

    fishCounter = 0;
    radius = 5;
    volume = (float) (4.0/3.0) * M_PI * pow(radius, 3);
    edges = boundary;
    age = 0;
    speed = 20;
    vector3 destination (50, 50, 50);
    int currentRadius;
    frameCounter = 0;
    fishTarget = 0;

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

    reverseCloseFish = 10 - eatCloseFish;

    //set velocity and position
    velocity = vel;
    position = pos;
}

void whale::updatePosition (vector<fish> &fishList) {
    frameCounter ++;

    //every 5 frames, update destination and velocity
    if (frameCounter % 5 == 0)
    {
        updateDestination(fishList);
    }

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
    if (eat)
    {
        //don't move, in main go through foodList and remove those IDs
        fishCounter += foodList.size();
    }
    //find destination to move
    else
    {
        updatePosition(fishList);
    }

    //only change destination if it reached the old one or every 20 frames
    if (closeEnough(destination, position, 2))
    {
        decisionMove(fishList);
    }
}

void whale::decisionEat(int numFish) {
    //numFish is the size of the fishList in main
    eat = false;

    //see if foodList is dense enough
    float density = ((float)foodList.size() / (float)volume) * 1000.0f;

    if (density >= eatDenseFish) {

        eat = true;
        cout << "eat " << foodList.size() << endl;
    }
}

void whale::decisionMove(vector<fish> &fishList) {

    int fishInReach = 0;

    //go through all fish
    for (int ff = 0; ff < fishList.size(); ff++)
    {
        fishInReach = 0;

        //see if the fish is close enough (using eatCloseFish)
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
            if (((float)fishInReach / (float)volume) >= (eatDenseFish / 1000.0f))
            {
                fishTarget = fishList[ff].id;
                break;
            }
        }
    }
}

vector<int> whale::sight(vector<fish> &fishList, int maxDist)
{
    //see which fish are within a certain distance of a given position
    vector<int> fishWork;

    for (int f = 0; f < fishList.size(); f++) {

        if (closeEnough(fishList[f].position, position, maxDist)) {
            fishWork.push_back(fishList[f].id);
        }
    }

    //return a vector of fish IDs
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

    //return a bool
    return withinDist;
}

void whale:: updateDestination (vector<fish> &fishList)
{
    bool fishAlive = false;
    int vectorPos;

    for (int ff = 0; ff < fishList.size(); ff ++)
    {
        if (fishList[ff].id == fishTarget)
        {
            vectorPos = ff;
            fishAlive = true;
        }
    }

    //if the fish has been eaten, choose a new target
    if (fishAlive == false)
    {
        decisionMove(fishList);
    }
    else
    {
        //update destination using the fish's position
        destination = fishList[vectorPos].position;

        //find velocity to reach fish
        vector3 diff = fishList[vectorPos].position - position;
        velocity = diff;

        //normalize velocity
        velocity.normalize();

        //scale it to 2
        velocity *= 2;
    }
}

#endif /* whale_hpp */
