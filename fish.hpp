#ifndef FISH_HPP_INCLUDED
#define FISH_HPP_INCLUDED

#include <iostream>
#include <math.h>
#include <vector>

#include "vector3.hpp"
#include "agent.hpp"

class fish : public agent{
public:
    fish(vector3, vector3, int);

    //the id, but that was in the parent class

    /*Function that should be run repeatedly in update area (while loop in main).
    Does everything that needs to be done repeatedly (updating velocity, position, physical appearance, etc)*/
    //void updateFish();


private:
    //function to calculate separation velocity. Returns vector3
    //function to calculate alignment velocity. Returns vector3
    //function to calculate cohesion velocity. Returns vector3

    //void to update velocity
    //void to update position

    //function to make void of only other fish
    std::vector <agent> otherFishList(std::vector <agent>);
};

//constructor. Right now it's the same as the agent
fish :: fish(vector3 pos = vector3(), vector3 vel = vector3(), int id = 0): agent(pos, vel, id)
{

}

//allAgents is the list of all agent types. Returns a vector with only fish and not the agent itself
std :: vector <agent> fish :: otherFishList(std::vector<agent> allAgents)
{
    std::vector<agent> test = allAgents;
    //not working right now
    /*for(int ii = test.size()-1; ii >= 0; ii --)
    {
        //if (test.id == my id) { remove test}
        if(test[ii].id == id) { test.erase(ii); }
        //if (test is a whale) { remove test}

    }*/

    return test;
}

#endif // FISH_HPP_INCLUDED
