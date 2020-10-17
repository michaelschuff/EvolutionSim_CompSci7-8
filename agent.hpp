#ifndef agent_hpp
#define agent_hpp

#include<iostream>
#include "vector3.hpp"

class agent {
public:
    vector3 position;
    vector3 velocity;

    static int nextID;
    int id; //individual agent id

protected:
    agent(vector3 pos, vector3 vel) : position(pos), velocity(vel), id(nextID)
    {
        nextID ++;
    } //agent constructor takes in two vector3s to create instance of agent
};


#endif /* agent_hpp */
