#ifndef agent_hpp
#define agent_hpp

#include<iostream>
#include "vector3.hpp"

class agent {
public:
    vector3 position;
    vector3 velocity;

    int id;

protected:
    agent(vector3 pos, vector3 vel, int ID) : position(pos), velocity(vel),  id(ID) {} //agent constructor takes in two vector3's to create instance of agent
};

#endif /* agent_hpp */
