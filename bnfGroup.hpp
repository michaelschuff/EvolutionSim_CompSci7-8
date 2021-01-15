//
//  agent.hpp
//  CompSci78
//
//  Created by Bronte McKinnis and Elaine Demetrion on 11/12/20.
//  Copyright © 2020 Bronte McKinnis and Elaine Demetrion. All rights reserved.
//
#ifndef BNFGROUP_HPP_INCLUDED
#define BNFGROUP_HPP_INCLUDED

using namespace std;
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "vector3.hpp"
#include "fish.hpp"
#include "whale.hpp"
#include <cstdlib>

class bnfGroup {
public:
    bnfGroup(vector<whale *>);
    void addWhale(whale *); //to add a whale after the group has been made
    vector<whale *> pod; //all of the whales in the group
    vector3 center; //of the pod
    float radius; //how far the bnf goes
    void trapFish(vector<fish>&); //sees which fish swim into the radius
    void bnfUpdate();
    bool done; //end bnf

private:
    int fishCounter;
    void calculateRadius();
    void calculateCenter();
    void endSession();
};

bnfGroup::bnfGroup(vector<whale*> initialWhales)
{
    pod = initialWhales;
    calculateCenter();
    fishCounter = 0;
    done = false;
}

void bnfGroup::addWhale(whale* newWhale)
{
    pod.push_back(newWhale);
}

void bnfGroup::calculateCenter()
{
    //find the avg position of the whales for the center of the pod
    vector3 sum;

    for (int aa = 0; aa < pod.size(); aa++)
    {
        sum += (*pod)[aa].position;
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
        avgBNF += (*pod)[aa].bubbleNetFeed;
        avgDense += (*pod)[aa].eatDenseFish;
        avgClose += (*pod)[aa].eatCloseFish;
    }

    avgBNF = avgBNF / pod.size();
    avgDense = avgDense / pod.size();
    avgClose = avgClose / pod.size();

    radius = pod.size() + (avgBNF * avgDense)/avgClose;
}

void bnfGroup::trapFish(vector<fish> &fishList)
{
    fishCounter = 0;

    for (int ff = 0; ff < fishList.size(); ff ++)
    {
        //if fish is within radius, stop moving
        if (fishList[ff].closeEnough(fishList[ff].position, center, radiusbnf))
        {
            fishCounter ++;
            fishList[ff].velocity = vector3();
        }
    }
}

void bnfGroup::endSession()
{
    for (int ff = 0; ff < fishList.size(); ff ++)
    {
        //if fish is within radius, add to food list
        if (fishList[ff].closeEnough(fishList[ff].position, center, radiusbnf))
        {
            fishCounter ++;

            //add the next fish to a whale's foodList
            (*pod)[fishCounter % pod.size()].foodList.push_back(fishList[ff].id);
        }
    }

    //change bnfCurrently to false for all whales
    for (int ww = 0; ww < pod.size(); ww++)
    {
        (*pod)[ww].bnfCurrently = false;
    }

    done = true;
}

void bnfGroup::bnfUpdate()
{
    calculateCenter();
    calculateRadius();
    trapFish();

    //check if the bnf session is over
    for (int ww = 0; ww < pod.size(); ww++)
    {
        if (!(*pod)[ww].bnfCurrently)
        {
            endSession();
        }
    }
}

#endif // BNFGROUP_HPP_INCLUDED
