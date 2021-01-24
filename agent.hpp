#ifndef agent_hpp
#define agent_hpp

#include "vector3.hpp"
#include <cmath>

class agent {
public:
    vector3 position;
    vector3 velocity;
    int framerate;

    static int nextID;
    int id; //individual agent id
    bool closeEnough(vector3, vector3, int); //given 2 positions and a radius, compares distance

protected:
    //Constructor should only get accessed by child classes. (An amorphous agent has no meaning; must be a fish or a whale.)
    agent(vector3 pos, vector3 vel, int fr) : position(pos), velocity(vel), id(nextID), framerate(fr) {
        //each time a new agent is made, it gets a unique id. So no two whales, two fish, or whale and fish have the same id.
        nextID++;
    } //agent constructor takes in two vector3s to create instance of agent
};

bool agent::closeEnough(vector3 otherPos, vector3 myPos, int maxDist)
{
    bool withinDist = false;
    vector3 difference;
    double actualRadius;

    //use 3D Pythagorean theorem to find the radius value
    difference = otherPos - myPos;
    actualRadius = sqrt(pow(difference.x, 2) + pow(difference.y, 2)+ pow(difference.z, 2));

    //if it's less than r the other agent is close enough
    if (actualRadius <= maxDist) {
        withinDist = true;
    }

    //return a bool
    return withinDist;
}

#endif /* agent_hpp */
