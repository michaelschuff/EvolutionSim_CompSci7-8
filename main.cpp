#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include<time.h>
#include <vector>
#include "vector3.hpp"
#include "triangle.hpp"
#include "mesh.hpp"
//#include "camera.hpp"
#include "fish.hpp"
#include<stdio.h>
#include<stdlib.h>
#include<ctime>
#include "agent.hpp"
#include "whale.hpp"
#include <cstdlib>
#include <unistd.h>
//#include "ResourcePath.hpp"

using namespace sf;
using namespace std;

int main() {
    srand(time(NULL));

    vector<whale> whaleList;
    vector<fish> fishList;
    int numWhales = 10;
    vector3 limits (1000,1000,1000); //the size of the ocean
    int numReproduce = numWhales * 0.10;
    int numDie = numWhales * 0.10;
    int totalWhales = numWhales; //the total number of whales that have ever existed


    //make whales w/ random starting traits
    for (int ww = 0; ww < numWhales; ww ++)
    {
        whale newWhale((rand() % 10) + 1,(rand() % 10) + 1, vector3(), vector3(0,0,0), limits);
        newWhale.id = ww;
        whaleList.push_back(newWhale);
    }


    //make fish
    for (int ff = 0; ff < 10000; ff ++)
    {
        fish newFish (vector3 ((rand() % 1000), (rand() % 1000), (rand() % 1000)), vector3 ());
        newFish.id = ff;
        fishList.push_back(newFish);
    }

//*****************loop of program
    while(true)
    {
        //*********************whales make decisions about what to do
        for (int ww = 0; ww < whaleList.size(); ww++)
        {
            whaleList[ww].decision(fishList);

            //*************when whales are able to eat
            if (whaleList[ww].eat == true)
            {
                //go through the list of food
                for (int ee = whaleList[ww].foodList.size() -1; ee >= 0; ee--)
                {
                    //see if the IDs match those of fish
                    for (int ff = fishList.size() -1; ff >= 0; ff --)
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

            //**************move whale
            else
            {
                whaleList[ww].updatePosition(5.4);  //the number passed is the "length" of a frame
            }
        }

        cout << endl;

        //sort the whales based on number of fish they've eaten
        for (int i = 0; i < whaleList.size(); i++)
        {
            int j = i;

            while (j > 0 and (float)((float)whaleList[j].fishCounter / (float)whaleList[j].age) < (float)((float)whaleList[j-1].fishCounter / (float)whaleList[j-1].age))
            {
                swap(whaleList[j], whaleList[j - 1]);
                j = j -1;
            }
        }

        //bottom 10% die
        for (int dd = 0; dd < numDie; dd++)
        {
            whaleList.erase(whaleList.begin() + dd);
        }

        //top 10% reproduce
        for (int rr = whaleList.size() -1; rr > whaleList.size() - numReproduce -1; rr --)
        {
            whale newWhale(whaleList[rr].eatCloseFish,whaleList[rr].eatDenseFish, whaleList[rr].position, vector3(), limits);
            totalWhales ++;
            newWhale.id = totalWhales;
            whaleList.push_back(newWhale);
        }

        for (int ww = 0; ww < whaleList.size(); ww++)
        {
            cout << whaleList[ww].id << ": (" << whaleList[ww].age << ") " << whaleList[ww].fishCounter << ", dense: " << whaleList[ww].eatDenseFish << ", close: " << whaleList[ww].eatCloseFish << endl;
        }

        cout << endl;
    }


    return EXIT_SUCCESS;
}

