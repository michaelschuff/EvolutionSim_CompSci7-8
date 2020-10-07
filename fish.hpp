#ifndef FISH_HPP_INCLUDED
#define FISH_HPP_INCLUDED

#include <iostream>
#include <math.h>

#include "vector3.hpp"
#include "agent.hpp"

class fish : public agent{
public:
    ///ERROR. Must somehow specify that I'm referring to position/velocity from agent
    fish(vector3 _pos=vector3(), vector3 _vel=vector3()) : position(_pos), velocity(_vel) {} //constructor (I think)
    //fish(const fish &Fish) : position(Fish.position), velocity(Fish.velocity) {}

    //the id, but that was in the parent class

    /*Function that should be run repeatedly in update area (while loop in main).
    Does everything that needs to be done repeatedly (updating velocity, position, physical appearance, etc)*/
    void updateFish();
private:
    //function to calculate separation velocity. Returns vector3
    //function to calculate alignment velocity. Returns vector3
    //function to calculate cohesion velocity. Returns vector3

    //void to update velocity
    //void to update position

};
#endif // FISH_HPP_INCLUDED
