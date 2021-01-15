//
//  agent.hpp
//  CompSci78
//
//  Created by Bronte McKinnis and Elaine Demetrion on 11/12/20.
//  Copyright © 2020 Bronte McKinnis and Elaine Demetrion. All rights reserved.
//

#ifndef EVOLUTIONSIM_HPP_INCLUDED
#define EVOLUTIONSIM_HPP_INCLUDED
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "vector3.hpp"
#include "agent.hpp"
#include "fish.hpp"
#include "whale.hpp"

class evolutionSim {
public:
    evolutionSim(int, int, vector3, int);

    vector<whale> whaleList;
    vector<fish> fishList;

    void updateSim(float, float, float, float);

private:
    int numReproduce;
    int numDie;
    int frameCounter;
    int framerate;

    vector3 limits;

    void killFish();
    void whaleReproduction();
    //void fishReproduction();


    void evaluateBubbleNetFeeding(); //run at some constant interval
    ///The following two functions should be lists OF references (or a list of lists of references to whales)
    vector<vector<whale*>> groupsOfBubbleNettingWhales; //an empty list to be filled by a function
    vector<whale*> currentBubbleNettingWhales; //another empty list to be filled by a function
    //These two functions evaluate the variables above
    void groupWhales(); //evaluates groupsOfBubbleNettingWhales
    void findBubbleNettingWhales(); //evaluates groupsOfBubbleNettingWhales
};

evolutionSim::evolutionSim(int numWhales, int numFish, vector3 lim, int rate) {
    limits = lim;
    numReproduce = numWhales * 0.10;
    numDie = numWhales * 0.10;
    frameCounter = 0;
    framerate = rate;

    //make whales w/ random starting traits
    for (int w = 0; w < numWhales; w++) {
        whale newWhale((rand() % 10) + 1,(rand() % 10) + 1, vector3((rand() % (int) limits.x), (rand() % (int) limits.y), (rand() % (int) limits.z)), vector3(rand(), rand(), rand()).normalized(), limits, framerate);
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
    }

    for (int ww = 0; ww < whaleList.size(); ww++) {

        whaleList[ww].decision(fishList);

        /*
        // when whales are able to eat
        if (whaleList[ww].eat == true) {
            killFish();
        }
        */
    }

    whaleReproduction ();
    frameCounter ++;
}

void evolutionSim::killFish()
{
    for (int ww = whaleList.size() -1; ww >= 0; ww--)
    {
        //go through the list of food
        for (int ee = whaleList[ww].foodList.size() -1; ee >= 0; ee--)
        {
            //see if the IDs match those of fish
            for (int ff = fishList.size() - 1; ff >= 0; ff--)
            {

                if (fishList[ff].id == whaleList[ww].foodList[ee])
                {
                    //eat the fish

                    fishList.erase(fishList.begin() + ff);
                    whaleList[ww].foodList.erase(whaleList[ww].foodList.begin() + ee);
                }
            }
        }
    }
}

void evolutionSim::whaleReproduction()
{
    //sort the whales based on number of fish they've eaten
    for (int i = 0; i < whaleList.size(); i++) {
        int j = i;
        while (j > 0 and ((float) whaleList[j].fishCounter / whaleList[j].age) < ((float) whaleList[j-1].fishCounter / whaleList[j-1].age)) {
            swap(whaleList[j], whaleList[j - 1]);
            j = j - 1;
        }
    }

    //every 500 frames whales reproduce/die
    if ((frameCounter % 500) == 0)
    {
        //bottom 10% die
        for (int d = 0; d < numDie; d++) {
            whaleList.erase(whaleList.begin() + d);
        }

        //top 10% reproduce
        for (int r = whaleList.size() - 1; r > whaleList.size() - numReproduce - 1; r--) {
            whale newWhale(whaleList[r].eatCloseFish, whaleList[r].eatDenseFish, whaleList[r].position, whaleList[r].velocity, limits, framerate);
            whaleList.push_back(newWhale);
        }

        for (int ww = 0; ww < whaleList.size(); ww++)
        {
            cout << whaleList[ww].id << ": (" << whaleList[ww].age << "): " << whaleList[ww].fishCounter << ", dense: " << whaleList[ww].eatDenseFish << ", close: " << whaleList[ww].eatCloseFish << endl;
        }
        cout << endl;
    }
}

/**************Everything below here is untested********************************************
Things that really need to be tested:
  * Whether these functions are actually adding whale REFERENCES to their respective variables.
    They should NOT be adding copies of whales, that'd be bad.
*/
void evolutionSim::evaluateBubbleNetFeeding()
{
    findBubbleNettingWhales();
    groupWhales();
    //actually evaluate whether the bubble feeding is successful

    //
}
//evaluates the value of currentBubbleNettingWhales
void evolutionSim::findBubbleNettingWhales()
{
    //search through all whales. Check if they are bubble net feeding.
    currentBubbleNettingWhales.clear();

    for(int ww = 0; ww < whaleList.size(); ww ++) {
        if( true /*whaleList[ww].isBNF*/) {
            whale *myWhale = &whaleList[ww]; //creates a pointer
            currentBubbleNettingWhales.push_back(myWhale); //adds the pointer to this list
        }
    }
}
//function to sort whales into individual groups based on location
//basically just evaluates the value of groupsOfBubbleNettingWhales
void evolutionSim::groupWhales()
{
    vector<whale*> ungroupedWhales = currentBubbleNettingWhales;
    ///maybe randomize order of ungrouped whales?
    vector<whale*> groupedWhales; ///what's this vector for right now?

    vector<whale*> group;

    for(int ww = 0; ww < ungroupedWhales.size(); ww ++) { ///check for out of bounds error if ungroupedWhales.size() doesn't update as ungroupedWhales shrinks
        for(int tt = ungroupedWhales.size() - 1; tt >= 0; tt --){
            if(group.size() <= 5 && /**it's within r units of the original group center*/)
        }
    }
}
#endif // EVOLUTIONSIM_HPP_INCLUDED
