#ifndef FISH_HPP_INCLUDED
#define FISH_HPP_INCLUDED

#include <iostream>
#include <math.h>

#include "vector3.hpp"
#include "agent.hpp"

class fish : public agent{
public:
    fish(vector3, vector3);


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

};

//constructor. Right now it's the same as the agent
fish :: fish(vector3 pos = vector3(), vector3 vel = vector3()): agent(pos, vel)
{

}
#endif // FISH_HPP_INCLUDED
