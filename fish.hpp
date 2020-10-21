#ifndef FISH_HPP_INCLUDED
#define FISH_HPP_INCLUDED

#include <iostream>
#include <math.h>
#include <vector>
#include<SFML/Graphics.hpp>

#include "vector3.hpp"
#include "agent.hpp"
using namespace std;

class fish : public agent{
public:

    static vector<fish> *allFishList; //pointer to vector in main. Every fish can see every other fish, so this is static

    sf::CircleShape Shape2D; ///Delete later
    double vision; ///could be private

    //constructor. Same as agent constructor for now.
    fish(vector3 pos = vector3(), vector3 vel = vector3()): agent(pos, vel)
    {
        vision = 200;
        Shape2D.setRadius(10); ///Delete later
    }

    /*Function that should be run repeatedly in update area (while loop in main).
    Does everything that needs to be done repeatedly (updating velocity, position, physical appearance, etc)*/
    void updateFish();

    void reportFishNumbers();
    ///these should only be used if there is more than one fish around
    //function to calculate separation velocity. Returns vector3
    vector3 vSeparation();
    //function to calculate alignment velocity. Returns vector3
    vector3 vAlignment();
    //function to calculate cohesion velocity. Returns vector3
    vector3 vCohesion();



private:

    //void to update velocity
    void updateVelocity();
    //void to update position based on current velocity
    void updatePosition();

    //function to make vector of only other fish
    std::vector <fish> otherFishList();

};

void fish :: updateFish()
{
    updateVelocity();
    updatePosition();
    Shape2D.setPosition(position.x, position.y);
}

void fish :: updateVelocity()
{
    velocity += (3*vSeparation() + 0.2*vAlignment() + vCohesion())*0.25;
    velocity.normalize(); //might wanna change this
}

void fish :: updatePosition()
{
    position += velocity*0.5; //scale by something?
}
//should work
vector3 fish :: vSeparation()
{
    vector<fish> fish = otherFishList(); //list of other fish
    vector3 finalV = vector3(0, 0, 0); //will become the returned vector3

    float r = vision/4; ///How close fish can be to trigger this. Tweak this later!

    //Evaluate each fish in the array
    for(int ii = 0; ii < fish.size(); ii ++) {

        double distAway = position.distance(fish[ii].position);

        //if dist btwn me and this fish is less than r...
        if(distAway < r && distAway > 0) {
            //adjust finalV by: vector3 of movement away from fish, but move less for farther away fish
            finalV += (position - fish[ii].position).normalized()/distAway; //Possibly multiply by some value
        }
    }

    ///may want to scale finalV by some amount
    //finalV -= velocity;
    if(finalV.x == 0 && finalV.y == 0 && finalV.z == 0) {return vector3(0, 0, 0);}
    return finalV.normalized(); ///may want to scale differently
}
//should work
vector3 fish :: vAlignment()
{
    vector<fish> fish = otherFishList();
    vector3 finalV = vector3(0, 0, 0);
    double fishConsidering;

    //sum all other fish velocities (if fish can be seen)
    for(int ii = 0; ii < fish.size(); ii ++) {
        if(position.distance(fish[ii].position) < vision) {
            fishConsidering ++;
            finalV += fish[ii].velocity;
        }
    }

    //divide to get average
    if(fishConsidering > 0 && finalV.magnitude() > 0) {
        finalV /= fishConsidering;
        return finalV.normalized();
    }
    return finalV;
}

//should work
vector3 fish :: vCohesion()
{
    vector<fish> fish = otherFishList();

    vector3 avgPos = vector3(0, 0, 0);
    double fishConsidering = 0;

    //add up all of the positions
    for(int ii = 0; ii < fish.size(); ii ++) {
        if(position.distance(fish[ii].position) < vision) {
            fishConsidering ++;
            avgPos += fish[ii].position;
        }
    }

    //make sure we don't divide by 0
    if(fishConsidering > 0 && avgPos.magnitude() > 0) {
        avgPos /= fishConsidering;
        //find vector from current position to average position
        avgPos -= position;
        return avgPos.normalized();
    }
    return avgPos;
}
//allAgents is the list of all agent types. Returns a vector with only fish and not the agent itself. Yes, this works.
std :: vector <fish> fish :: otherFishList()
{
    std::vector<fish> test = *allFishList;

    for(int ii = test.size()-1; ii >= 0; ii --)
    {
        if(test[ii].id == id) { test.erase(test.begin() + ii); }
    }
    return test;
}

//For debugging purposes
void fish :: reportFishNumbers()
{
    std::cout<<"This fish, (id #"<<id<<") knows there are "<<allFishList -> size()<<" fish in the sea"<<endl;
    std::cout<<"    These fish are: ";
    for(int ii = 0; ii < allFishList -> size(); ii ++) {
        vector<fish> subFish = *allFishList;
        cout<<subFish[ii].id<<", ";
    }
    std::cout<<endl;
}
#endif // FISH_HPP_INCLUDED
