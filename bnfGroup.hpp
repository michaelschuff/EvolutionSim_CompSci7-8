//
//  agent.hpp
//  CompSci78
//
//  Created by Bronte McKinnis and Elaine Demetrion on 11/12/20.
//  Copyright � 2020 Bronte McKinnis and Elaine Demetrion. All rights reserved.
//
#ifndef BNFGROUP_HPP_INCLUDED
#define BNFGROUP_HPP_INCLUDED

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <cstdlib>
#include "vector3.hpp"
#include "fish.hpp"
#include "whale.hpp"
using namespace std;

class bnfGroup {
public:
    bnfGroup(vector<whale*>);
    void addWhale(whale*); //to add a whale after the group has been made
    vector<whale*> pod; //all of the whales in the group
    vector3 center; //of the pod
    float radius; //how far the bnf goes
    void trapFish(vector<fish>&); //sees which fish swim into the radius
    void bnfUpdate(vector<fish>&);
    bool done; //end bnf
    void redoPointers(vector<whale*>); //redos the address of the pointers for whales

private:
    int fishCounter;
    void calculateRadius();
    void calculateCenter();
    void endSession(vector<fish>&);
    vector<int> whaleIDs;
};

bnfGroup::bnfGroup(vector<whale*> initialWhales)
{
    pod = initialWhales;
    calculateCenter();
    fishCounter = 0;
    done = false;

    //make all whales aware that they are assigned to a pod
    for (int ww = 0; ww < pod.size(); ww++)
    {
        pod[ww]->isAssignedToPod = true;
        whaleIDs.push_back(pod[ww]->id);
    }
}

void bnfGroup::addWhale(whale* newWhale)
{
    pod.push_back(newWhale);
    newWhale->isAssignedToPod = true;
    whaleIDs.push_back(newWhale->id);
}

void bnfGroup::calculateCenter()
{
    //find the avg position of the whales for the center of the pod
    vector3 sum;

    for (int aa = 0; aa < pod.size(); aa++)
    {
        sum += pod[aa]->position;
    }

    center = sum / pod.size();
}

void bnfGroup::calculateRadius()
{
    //radius is determined by several factors of the pod

    float avgBNF = 0;
    float avgDense =0;
    float avgClose = 0;

    for (int aa = 0; aa < pod.size(); aa++)
    {
        avgBNF += pod[aa]->bubbleNetFeed;
        avgDense += pod[aa]->eatDenseFish;
        avgClose += pod[aa]->eatCloseFish;
    }

    avgBNF = avgBNF / pod.size();
    avgDense = avgDense / pod.size();
    avgClose = avgClose / pod.size();

    radius = (pod.size() + (avgBNF * avgDense)/avgClose);
}

void bnfGroup::trapFish(vector<fish> &fishList) {

    for (int ff = 0; ff < fishList.size(); ff ++)
    {
        //if fish is within radius, stop moving
        if (!fishList[ff].inBubbleNet && fishList[ff].closeEnough(fishList[ff].position, center, radius))
        {
            fishList[ff].inBubbleNet = true;
        }
    }
}

void bnfGroup::endSession(vector<fish> &fishList)
{
    fishCounter = 0;
    int whichWhale = 0;

    cout<< "Pod with: ";

    for (int w = 0; w < pod.size(); w ++)
    {
        cout << pod[w] -> id << ", ";
    }
    cout << "..... at location ("<<center.x<<","<<center.y<<","<<center.z<<") with radius "<<radius<<endl;

    for (int ff = 0; ff < fishList.size(); ff ++)
    {
        //if fish is within radius, add to food list
        if (fishList[ff].closeEnough(fishList[ff].position, center, radius))
        {
            fishCounter ++;

            if (whichWhale >= pod.size())
            {
                whichWhale = 0;
            }

            //add the next fish to a whale's foodList
            (*(pod[whichWhale])).foodList.push_back(fishList[ff].id);

            whichWhale ++;
        }
    }

    //change bnfCurrently to false for all whales
    for (int ww = 0; ww < pod.size(); ww++)
    {
        pod[ww]->bnfCurrently = false;
        pod[ww]->isAssignedToPod = false;
        pod[ww]->distributeFish = true;
    }

    done = true;
}

void bnfGroup::bnfUpdate(vector<fish> &fishList)
{
    //check if the pod still has whales. If not, set radius to 0 to ensure no fish remain trapped
    if(pod.size() == 0) {
        radius = 0;
        endSession(fishList);
    }

    else{
        calculateCenter();
        calculateRadius();
        trapFish(fishList);

        //check if the bnf session is over
        for (int ww = 0; ww < pod.size(); ww++)
        {
            if (!pod[ww]->bnfCurrently)
            {
                endSession(fishList);
            }
        }
    }

}

void bnfGroup::redoPointers(vector<whale*> whalePointers)
{
    //clear pod
    pod.clear();

    //pass bnf group a vector of all whales in pointer form
    for (int pp = 0; pp < whalePointers.size(); pp++)
    {
        for (int ii = 0; ii < whaleIDs.size(); ii ++)
        {
            //add whales w/ correct ids (unless dead) to pod
            if (whalePointers[pp]->id == whaleIDs[ii])
            {
                pod.push_back(whalePointers[pp]);
            }
        }
    }
}

#endif // BNFGROUP_HPP_INCLUDED
