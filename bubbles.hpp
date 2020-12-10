#ifndef BUBBLES_HPP_INCLUDED
#define BUBBLES_HPP_INCLUDED
#include<SFML/Graphics.hpp>
#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include "vector3.hpp"

class bubble {
public:
    sf::CircleShape myShape; ///debugging only
    bool deleteMe = false; //must be able to be accessed by evolutionSim

    bubble(vector3 pos, vector3 whaleVel, vector3 emissionVel) {
        setV0(whaleVel, emissionVel);
        position = pos;

        ///debugging only, remove later
        myShape.setRadius(20);
        myShape.setFillColor(sf::Color(200, 200, 255));
    }

    void updateBubble();
private:
    vector3 position, velocity;
    float speed = 0.1;
    void setV0(vector3, vector3); //calculate the bubble's first velocity
    void setNewVelocity();
    float yBound = 0;
};

void bubble::setV0(vector3 whaleVel, vector3 emissionVel) {
    velocity = speed*(whaleVel + emissionVel).normalized(); //actually I think this is physically accurate

}

void bubble::updateBubble()
{
    setNewVelocity();

    //deal with bubbles if they are out of bounds.
    if(position.z < yBound) {
        deleteMe = true; //tells simulation evolutionSim to delete the bubble. If this isn't tied to evoSim, the bubble will just stay put
        velocity = vector3(0, 0, 0);
    }

    position += velocity;

    myShape.setPosition(position.x, position.z); ///debugging only
}

void bubble::setNewVelocity()
{
    //This is roughly accurate according to Archimedes Principle
    vector3 aFromFb(0, 0, -0.0005);
    velocity += aFromFb;
}

#endif // BUBBLES_HPP_INCLUDED
