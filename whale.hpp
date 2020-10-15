#ifndef whale_hpp
#define whale_hpp

#include<iostream>
#include<math.h>
#include<time.h>
#include "vector3.hpp"
#include "agent.hpp"
#include "fish.hpp"

/*
This file sets up the whale class, which is a child of the agent class.
*/

class whale : public agent
{
public:
    whale(int, int, vector3, vector3);
    int fishCounter; //how many fish the whale has eaten, used for ranking
    void updatePosition(double); //update position every frame based on velocity and time
    //void sight (vector<fish>); //see the fish around them and determine which ones are in a distance of them
    whaleMove (); //given a location, move towards it frame by frame
    decision (); //using its traits, decide what to do on a turn
    eatFish (); //reports which fish need to be removed from the list and adds to fishCounter

private:
    //both of these are on a scale of 1-10
    int eatCloseFish; //trait that decides how much the whale is willing to move
    int eatDenseFish; //trait that determines how efficient the whale is
};

whale::whale (int givenTraitClose, int givenTraitDense, vector3 pos, vector3 vel) : agent(pos, vel)
{
    int randChangeClose, randChangeDense, addOrSubtract;

    fishCounter = 0;

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

void whale::sight (vector<fish> possibleFood)
{
    vector3 difference;
    int eatRadius = 5;
    int

    for (int ff = 0; ff < possibleFood.size(); ff ++)
    {
        //find the differences in x, y, and z values
        difference.x = possibleFood[ff].position.x - position.x;
        difference.y = possibleFood[ff].position.y - position.y;
        difference.z = possibleFood[ff].position.z - position.z;

        //use 3D Pythagorean theorem to find the radius value


        //if it's less than r add that fish's id to the list
    }
}

#endif /*whale_hpp*/
