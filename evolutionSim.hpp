//
//  agent.hpp
//  CompSci78
//
//  Created by Bronte McKinnis and Elaine Demetrion on 11/12/20.
//  Copyright � 2020 Bronte McKinnis and Elaine Demetrion. All rights reserved.
//

#ifndef EVOLUTIONSIM_HPP_INCLUDED
#define EVOLUTIONSIM_HPP_INCLUDED

using namespace std;
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "vector3.hpp"
#include "agent.hpp"
#include "fish.hpp"
#include "whale.hpp"
#include "bnfGroup.hpp"
#include <cstdlib>


class evolutionSim {
public:
    evolutionSim(int, int, vector3, int);

    vector<whale> whaleList;
    vector<fish> fishList;
    vector<bnfGroup> bnfPods;

    void updateSim(float, float, float, float);

private:
    float numReproduce;
    float numDie;
    int frameCounter;
    int framerate;
    int fishNum;

    vector3 limits;

    void killFish(int);
    void whaleReproduction();
    void fishReproduction();

    void bnfBehavior();
    void updateBNFPods();
    vector<whale *> unassignedBNFWhales(); //creates a list of whales who aren't in a pod and who are bnf

};

evolutionSim::evolutionSim(int numWhales, int numFish, vector3 lim, int rate) {
    limits = lim;
    //numReproduce = floor(numWhales * 0.10);
    numDie = floor(numWhales * 0.10);
    numReproduce = numDie;
    frameCounter = 0;
    framerate = rate;
    fishNum = numFish;


    //make whales w/ random starting traits
    for (int w = 0; w < numWhales; w++) {
        whale newWhale((rand() % 10) + 1,(rand() % 10) + 1, (rand() % 10) + 1, vector3((rand() % (int) limits.x), (rand() % (int) limits.y), (rand() % (int) limits.z)), vector3(rand(), rand(), rand()).normalized(), limits, framerate);
        whaleList.push_back(newWhale);
    }

    //make fish
    for (int f = 0; f < numFish; f++) {
        fish newFish(vector3((rand() % (int) limits.x), (rand() % (int) limits.y), (rand() % (int) limits.z)), vector3(), framerate);
        fishList.push_back(newFish);
    }


}

void evolutionSim::updateSim(float coh, float sep, float ali, float avo)
{
    //puts all whale positions in a new vector to get passed to fish (couldn't use reference b/c can't include whale hpp file in fish hpp)
    vector<vector3> whalePositions;
    for(int ii = 0; ii < whaleList.size(); ii ++) {
        whalePositions.push_back(whaleList[ii].position);
    }

    // MARK: Update Agents
    for (int i = 0; i < fishList.size(); i++) {
        fishList[i].cohesion = coh;
        fishList[i].separation = sep;
        fishList[i].alignment = ali;
        fishList[i].avoidance = avo;
        fishList[i].updateFish(fishList, whalePositions);

        fishList[i].inBubbleNet = false;
    }

    for (int ww = 0; ww < whaleList.size(); ww++) {

        whaleList[ww].decision(fishList, whaleList);

        // when whales are able to eat
        if (whaleList[ww].eat == true) {
            killFish(ww);
        }
    }

    //go through bubble net feeding groups
    bnfBehavior();

    whaleReproduction ();

    fishReproduction ();

    frameCounter ++;
}

void evolutionSim::killFish(int positionInList)
{
    //go through the list of fish
    for (int ff = fishList.size() - 1; ff >= 0; ff--)
    {
        //see if the IDs match those of fish
        for (int fl = whaleList[positionInList].foodList.size() -1; fl >= 0; fl--)
        {
            if (fishList[ff].id == whaleList[positionInList].foodList[fl])
            {
                //eat the fish
                fishList.erase(fishList.begin() + ff);
                whaleList[positionInList].foodList.erase(whaleList[positionInList].foodList.begin() + fl);
                break;
            }
        }
    }
}

void evolutionSim::whaleReproduction()
{
    vector<whale*> pointerList;

    //every 500 frames whales reproduce/die
    if ((frameCounter % 500) == 0)
    {
        cout << endl;

        //sort the whales based on number of fish they've eaten
        for (int i = 0; i < whaleList.size(); i++) {
            int j = i;
            while (j > 0 and ((float) whaleList[j].fishCounter / whaleList[j].age) < ((float) whaleList[j-1].fishCounter / whaleList[j-1].age)) {
                swap(whaleList[j], whaleList[j - 1]);
                j = j - 1;
            }
        }

        //bottom 10% die
        for (int d = 0; d < numDie; d++) {
            whaleList.erase(whaleList.begin() + d);
        }

        //top 10% reproduce
        for (int r = whaleList.size() - 1; r >= whaleList.size() - numReproduce; r--) {
            whale newWhale(whaleList[r].eatCloseFish, whaleList[r].eatDenseFish, whaleList[r].bubbleNetFeed, whaleList[r].position, whaleList[r].velocity, limits, framerate);
            whaleList.push_back(newWhale);
            r ++;
        }

        //make a vector of pointers
        for (int ww = 0; ww < whaleList.size(); ww ++)
        {
            pointerList.push_back(&whaleList[ww]);
        }

        //update each pod
        for (int wp = 0; wp < bnfPods.size(); wp ++)
        {
            bnfPods[wp].redoPointers(pointerList);
        }

        for (int ww = 0; ww < whaleList.size(); ww++)
        {
            cout << whaleList[ww].id << ": (" << whaleList[ww].age << "): " << whaleList[ww].fishCounter << ", dense: " << whaleList[ww].eatDenseFish << ", close: " << whaleList[ww].eatCloseFish << ", bubble net feed: " << whaleList[ww].bubbleNetFeed << endl;
        }
        cout << endl;

        cout << "num fish: " << fishList.size() << endl<< endl;
    }
}

void evolutionSim::fishReproduction()
{
    //make fish
    for (int f = 0; f < fishNum - fishList.size(); f++) {
        fish newFish(vector3((rand() % (int) limits.x), (rand() % (int) limits.y), (rand() % (int) limits.z)), vector3(), framerate);
        fishList.push_back(newFish);
    }
}

void evolutionSim::bnfBehavior()
{
    //find the groups of whales who are bnf & add to bnfPods vector
    //see if new whales need to be added
    updateBNFPods();

    //go through all the whale groups
    for (int bb = 0; bb < bnfPods.size(); bb++)
    {
        bnfPods[bb].bnfUpdate(fishList);

        //see if the group is no longer bubble net feeding
        if (bnfPods[bb].done)
        {
            bnfPods.erase(bnfPods.begin() + bb);
        }
    }
}

vector<whale *> evolutionSim::unassignedBNFWhales()
{
    vector<whale*> v;

    for (int ww = 0; ww < whaleList.size(); ww++){
        if(whaleList[ww].bnfCurrently && !whaleList[ww].isAssignedToPod) {
            v.push_back(&whaleList[ww]);
        }
    }

    return v;
}

void evolutionSim::updateBNFPods()
{
    /*
    For each whale that hasn't been assigned to a bnfGroup:
        - go through each bnf group
        - If there's a group within radius and not maxed out on members, join it and terminate loop
        - If not, make new group with just this whale
    */

    vector<whale *> unassigned = unassignedBNFWhales();

    bool whaleGotAssigned;

    for(whale * w: unassigned) {
        whaleGotAssigned = false;

        //go through each bnfGroup
        for(int ii = 0; ii < bnfPods.size(); ii ++)
        {
            bool podIsCloseEnough = bnfPods[ii].center.distance((*w).position) < bnfPods[ii].radius;
            bool podIsSmallEnough = bnfPods[ii].pod.size() < 5; //could change this number

            //if there's a group within radius, not maxed out on members, this whale will join that group
            if(podIsCloseEnough && podIsSmallEnough) {
                bnfPods[ii].addWhale(w);
                whaleGotAssigned = true;

                break;
            }
        }

        //if whale didn't get assigned, start a new pod
        if(!whaleGotAssigned) {
            vector<whale *> newGroup = {w};
            bnfPods.push_back(newGroup);
        }
    }
}
#endif // EVOLUTIONSIM_HPP_INCLUDED
