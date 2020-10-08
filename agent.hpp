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
    agent(vector3 pos, vector3 vel) : position(pos), velocity(vel){} //for agent constructor
};

#endif /* agent_hpp */
