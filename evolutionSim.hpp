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
    evolutionSim(int, vector3, int);

    void fishReproduction();
    void whaleReproduction();

    vector<whale> whaleList;
    vector<fish> fishList;

    int numReproduce;
    int numDie;
    int numWhales;

    vector3 limits;

    void updateSim(float, float, float);
    void killFish();

};

evolutionSim::evolutionSim(int num, vector3 lim, int framerate) {
    numWhales = num;
    limits = lim;
    numReproduce = numWhales * 0.10;
    numDie = numWhales * 0.10;

    //make whales w/ random starting traits
    for (int w = 0; w < numWhales; w++) {
        whale newWhale((rand() % 10) + 1,(rand() % 10) + 1, vector3((rand() % (int) limits.x), (rand() % (int) limits.y), (rand() % (int) limits.z)), vector3(rand(), rand(), rand()).normalized(), limits, framerate);
        whaleList.push_back(newWhale);
    }

    //make fish
    for (int f = 0; f < 300; f++) {
        fish newFish(vector3((rand() % (int) limits.x), (rand() % (int) limits.y), (rand() % (int) limits.z)), vector3(), framerate);
        fishList.push_back(newFish);
    }


}

void evolutionSim::updateSim(float coh, float sep, float ali)
{
    // MARK: Update Agents
    for (int i = 0; i < fishList.size(); i++) {
        fishList[i].cohesion = coh;
        fishList[i].separation = sep;
        fishList[i].alignment = ali;
        fishList[i].updateFish(fishList);
    }

    for (int ww = 0; ww < whaleList.size(); ww++) {
        whaleList[ww].decision(fishList);
        if (whaleList[ww].eat == true) {// when whales are able to eat
            //killFish();
        } else { //move whale
            whaleList[ww].updatePosition();
        }
    }
}

void evolutionSim::killFish()
{
    /*
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
    */
}

///deal with this
void evolutionSim::whaleReproduction(int frameCounter)
{
    //go through whales and see which ones die
    //sort the whales based on number of fish they've eaten
    for (int i = 0; i < whaleList.size(); i++) {
        int j = i;
        while (j > 0 and ((float) whaleList[j].fishCounter / whaleList[j].age) < ((float) whaleList[j-1].fishCounter / whaleList[j-1].age)) {
            swap(whaleList[j], whaleList[j - 1]);
            j = j - 1;
        }
    }

    if ((frameCounter % 500) == 0)
    {
        //bottom 10% die
        for (int d = 0; d < numDie; d++) {
            whaleList.erase(whaleList.begin() + d);
        }

        //top 10% reproduce
        for (int r = whaleList.size() - 1; r > whaleList.size() - numReproduce - 1; r--) {
            whale newWhale(whaleList[r].eatCloseFish, whaleList[r].eatDenseFish, whaleList[r].position, whaleList[r].velocity, limits, framerate);
            totalWhales++;
            newWhale.id = totalWhales;
            whaleList.push_back(newWhale);
        }

        for (int ww = 0; ww < whaleList.size(); ww++)
        {
            cout << whaleList[ww].id << ": (" << whaleList[ww].age << "): " << whaleList[ww].fishCounter << ", dense: " << whaleList[ww].eatDenseFish << ", close: " << whaleList[ww].eatCloseFish << endl;
        }
        cout << endl;
    }

    frameCounter ++;
}
#endif // EVOLUTIONSIM_HPP_INCLUDED
