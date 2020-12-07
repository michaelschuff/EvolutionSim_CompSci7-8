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
};

void bubble::setV0(vector3 whaleVel, vector3 emissionVel) {
    velocity = (whaleVel + emissionVel).normalized(); //actually I think this is physically accurate

}

void bubble::updateBubble()
{
    ///deal with bubbles if they are out of bounds
    setNewVelocity();
    position += velocity;

    myShape.setPosition(position.x, position.z); ///debugging only
}

void bubble::setNewVelocity()
{
    //could have something more fancy, but this is fine for now
    velocity = speed * (velocity + vector3(0, 0, -0.0001)).normalized();
}

#endif // BUBBLES_HPP_INCLUDED
