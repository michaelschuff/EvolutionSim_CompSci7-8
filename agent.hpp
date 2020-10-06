#ifndef agent_hpp
#define AGENT_HPP_INCLUDED

#include<iostream>
#include "vector3.hpp"

class agent {
public:
    vector3 position;
    vector3 velocity;

    int id;
};

#endif // AGENT_HPP_INCLUDED
