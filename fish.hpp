#ifndef FISH_HPP_INCLUDED
#define FISH_HPP_INCLUDED

#include <iostream>
#include <math.h>
#include <vector>

#include "vector3.hpp"
#include "agent.hpp"
using namespace std;

class fish : public agent{
public:
    //constructor--right now, it's just the same as the agent
    fish(vector3 pos = vector3(), vector3 vel = vector3()): agent(pos, vel)
    {

    }
    /*Function that should be run repeatedly in update area (while loop in main).
    Does everything that needs to be done repeatedly (updating velocity, position, physical appearance, etc)*/
    //void updateFish();


private:
    //function to calculate separation velocity. Returns vector3
    //function to calculate alignment velocity. Returns vector3
    //function to calculate cohesion velocity. Returns vector3

    //void to update velocity
    //void to update position

    //function to make vector of only other fish
    std::vector <fish> otherFishList(std::vector <fish>);
};

//allAgents is the list of all agent types. Returns a vector with only fish and not the agent itself. Yes, this works.
std :: vector <fish> fish :: otherFishList(std::vector<fish> allAgents)
{
    std::vector<fish> test = allAgents;

    for(int ii = test.size()-1; ii >= 0; ii --)
    {
        if(test[ii].id == id) { test.erase(test.begin() + ii); }
    }
    return test;
}

#endif // FISH_HPP_INCLUDED
