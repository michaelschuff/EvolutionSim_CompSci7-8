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

private:
    int fishCounter;
    void calculateRadius();
    void calculateCenter();
    void endSession(vector<fish>&);
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
    }

    cout << "pod: ";
    for (int uu = 0; uu < pod.size(); uu ++)
    {
        cout << (* (pod[uu])).id << ", ";
    }
    cout << endl << endl;
}

void bnfGroup::addWhale(whale* newWhale)
{
    pod.push_back(newWhale);
    /**cout<<"     Whales in this pod: ";
    for(int i = 0; i < pod.size(); i ++) {
        cout<<pod[i]->id<<", ";
    }
    cout<<endl;*/
    newWhale->isAssignedToPod = true;
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
    //cout << "   center: " << center.x << ", " << center.y << ", " << center.z << endl;
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

    radius = 5*(pod.size() + (avgBNF * avgDense)/avgClose);
    //cout << "   radius: " << radius << endl;
}

void bnfGroup::trapFish(vector<fish> &fishList) {
    fishCounter = 0;

    //cout << "   fish in bnf: ";
    for (int ff = 0; ff < fishList.size(); ff ++)
    {
        //if fish is within radius, stop moving
        if (fishList[ff].closeEnough(fishList[ff].position, center, radius))
        {
            //cout << fishList[ff].id << ", ";
            fishCounter ++;
            fishList[ff].velocity = vector3();
        }
    }
    //cout << endl << "      " << fishCounter << endl;
}

void bnfGroup::endSession(vector<fish> &fishList)
{
    fishCounter = 0;
    int whichWhale = 0;

    cout << "   end session!!!" << endl;

    for (int ff = 0; ff < fishList.size(); ff ++)
    {
        //if fish is within radius, add to food list
        if (fishList[ff].closeEnough(fishList[ff].position, center, radius))
        {
            //cout << endl << "   " << fishList[ff].id << endl;
            fishCounter ++;

            if (whichWhale >= pod.size())
            {
                whichWhale = 0;
            }
            //cout << "   fish counter: " << fishCounter << ", pod size: " << pod.size() << endl;

            //add the next fish to a whale's foodList
            (*(pod[whichWhale])).foodList.push_back(fishList[ff].id);
            //cout << "   correct? " << pod.size() % fishCounter << endl;
            //cout << "   whale " << whichWhale << ": food list size: " << (*(pod[whichWhale])).foodList.size() << endl;

            whichWhale ++;
        }
    }
    //cout << "   !ENDING fish counter: " << fishCounter << endl;

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
    cout<< "Pod with: ";

    for (int w = 0; w < pod.size(); w ++)
    {
        cout << pod[w] -> id << ", ";
    }
    cout << endl;
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

#endif // BNFGROUP_HPP_INCLUDED
