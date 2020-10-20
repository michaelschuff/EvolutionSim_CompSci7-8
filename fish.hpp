#ifndef FISH_HPP_INCLUDED
#define FISH_HPP_INCLUDED

#include <iostream>
#include <math.h>
#include <vector>

#include "vector3.hpp"
#include "agent.hpp"
using namespace std;

class fish : public agent{
public:

    static vector<fish> *allFishList; //pointer to vector in main. Every fish can see every other fish, so this is static

    //constructor. Same as agent constructor for now.
    fish(vector3 pos = vector3(), vector3 vel = vector3()): agent(pos, vel)
    {
    }

    /*Function that should be run repeatedly in update area (while loop in main).
    Does everything that needs to be done repeatedly (updating velocity, position, physical appearance, etc)*/
    //void updateFish();

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

//not tested
vector3 fish :: vSeparation()
{
    vector<fish> fish = otherFishList(); //list of other fish
    vector3 finalV = vector3(0, 0, 0); //will become the returned vector3
    float r = 5; ///How close fish can be to trigger this. Tweak this later!
    for(int ii = 0; ii < fish.size(); ii ++) {
        //if dist btwn me and this fish is less than r...
        if(position.distance(fish[ii].position) < r) {
            fish[ii].position.print();
            finalV += position - fish[ii].position; //Possibly multiply by some value
        }
    }

    ///may want to scale finalV by some amount
    finalV -= velocity;
    return finalV.normalized();
}
//should work
vector3 fish :: vAlignment()
{
    cout<<"Determining alignment velocity of the fish (should point in same direction)"<<endl;
    vector<fish> fish = otherFishList();
    cout<<"I'm going to consider the positions of "<<fish.size()<<" other fish"<<endl;

    vector3 finalV = vector3(0, 0, 0);
    for(int ii = 0; ii < fish.size(); ii ++) {
        cout<<" * fish "<<fish[ii].id<<" has velocity ";
        fish[ii].velocity.print();

        finalV += fish[ii].velocity;
    }
    finalV /= fish.size();
    finalV -= velocity;

    if(finalV.x == 0 && finalV.y == 0 && finalV.z == 0) {return vector3(0, 0, 0);}
    return finalV.normalized(); ///may want to scale differently
}

//should work
vector3 fish :: vCohesion()
{
    cout<<"Determining cohesive velocity of fish "<<id<<endl;
    vector<fish> fish = otherFishList();
    cout<<"I'm going to consider the positions of "<<fish.size()<<" other fish"<<endl;

    vector3 avgPos = vector3(0, 0, 0);

    for(int ii = 0; ii < fish.size(); ii ++) {
        cout<<" * fish "<<fish[ii].id<<" has position ";
        fish[ii].position.print();
        avgPos += fish[ii].position;
    }
    avgPos /= fish.size();
    avgPos -= position;

    if(avgPos.x == 0 && avgPos.y == 0 && avgPos.z == 0) {return vector3(0, 0, 0);}
    return avgPos.normalized(); ///may want to scale differently
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
