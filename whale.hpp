#ifndef whale_hpp
#define whale_hpp

#include<iostream>
#include<math.h>
#include<time.h>
#include "vector3.hpp"
#include "agent.hpp"

/*
This file sets up the whale class, which is a child of the agent class.
*/

class whale : public agent
{
public:
    whale(int, int);
    int fishCounter; //how many fish the whale has eaten, used for ranking
    sight (); //see the fish around them and determine which ones are in a distance of them
    whaleMove (); //given a location, move towards it frame by frame
    decision (); //using its traits, decide what to do on a turn
    eatFish (); //reports which fish need to be removed from the list and adds to fishCounter

private:
    int eatCloseFish; //trait that decides how much the whale is willing to move
    int eatDenseFish; //trait that determines how efficient the whale is
};

whale::whale (int givenTraitClose, int givenTraitDense)
{
    int randChangeClose, randChangeDense;

    fishCounter = 0;

    //set up traits with some randomness
    randChangeClose = (rand());
}

#endif /*whale_hpp*/
