//
//  agent.hpp
//  CompSci78
//
//  Created by Elaine Demetrion on 10/30/20.
//  Copyright © 2020 Elaine Demetrion. All rights reserved.
//

#ifndef agent_hpp
#define agent_hpp

#include "vector3.hpp"

class agent {
public:
    vector3 position;
    vector3 velocity;
    int framerate;

    static int nextID;
    int id; //individual agent id

protected:
    //Constructor should only get accessed by child classes. (An amorphous agent has no meaning; must be a fish or a whale.)
    agent(vector3 pos, vector3 vel, int fr) : position(pos), velocity(vel), id(nextID), framerate(fr) {
        //each time a new agent is made, it gets a unique id. So no two whales, two fish, or whale and fish have the same id.
        nextID++;
    } //agent constructor takes in two vector3s to create instance of agent
};


#endif /* agent_hpp */
