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
    bnfGroup(vector<whale *>);
    void addWhale(whale *);
    vector<whale *> pod;
    vector3 center;
    float radius;
    void trapFish(vector<fish>&);
    void bnfUpdate();
    bool done;

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
    vector3 sum;

    for (int aa = 0; aa < pod.size(); aa++)
    {
        sum += pod[aa]->position;
    }

    center = sum / pod.size();
}

void bnfGroup::calculateRadius()
{
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
            *pod[fishCounter % pod.size()].foodList.push_back(fishList[ff].id);
        }
    }

    for (int ww = 0; ww < pod.size(); ww++)
    {
        *pod[ww].bnfCurrently = false;
    }

    done = true;
}

void bnfGroup::bnfUpdate()
{
    trapFish();

    for (int ww = 0; ww < pod.size(); ww++)
    {
        if (!*pod[ww].bnfCurrently)
        {
            endSession();
        }
    }

    endSession();
}

#endif // BNFGROUP_HPP_INCLUDED
