#ifndef BUBBLES_HPP_INCLUDED
#define BUBBLES_HPP_INCLUDED
#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include "vector3.hpp"

class bubble {
public:
    bubble(vector3 pos, vector3 whaleVel, vector3 emissionVel) {
        setV0(whaleVel, emissionVel);
        position = pos;
    }

    void updateBubble();
private:
    vector3 position, velocity;
    void setV0(vector3, vector3); //calculate the bubble's first velocity
};

void bubble::setV0(vector3 whaleVel, vector3 emissionVel) {
    velocity = whaleVel + emissionVel; //actually I think this is physically accurate

}


#endif // BUBBLES_HPP_INCLUDED
