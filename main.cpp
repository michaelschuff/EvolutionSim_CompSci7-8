#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include<time.h>
#include <vector>
#include "vector3.hpp"
#include "triangle.hpp"
#include "mesh.hpp"
#include "camera.hpp"
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

float frameTime (int); //finds the amount of time the frame has lasted

int main() {
    srand(time(NULL));

    time_t startTime;
    time_t endTime;
    double timeDifference;
    vector<whale> whaleList;
    vector<fish> fishList;
    int numWhales = 10; //the total number of whales that have ever existed


    //make whales w/ random starting traits
    for (int ww = 0; ww < numWhales; ww ++)
    {
        whale newWhale((rand() % 10) + 1,(rand() % 10) + 1, vector3(), vector3(0,0,0));
        newWhale.id = ww;
        whaleList.push_back(newWhale);
    }

    //make fish
    for (int ww = 0; ww < 10; ww ++)
    {
        fish newFish (vector3 ((rand() % 10), (rand() % 10), (rand() % 10)), vector3 ());
        newFish.id = ww;
        fishList.push_back(newFish);
        cout << "x: " << fishList[ww].position.x << "y: " << fishList[ww].position.y << "z: " << fishList[ww].position.z << endl;
    }

    //see if fish can be eaten
    whaleList[0].sight(fishList);

    //see the ids of fish that can be eaten
    for (int cc = 0; cc < whaleList[0].foodList.size(); cc++)
    {
        cout << "id: " << whaleList[0].foodList[cc] << endl;
    }

    /*
    //loop of program
    while(true)
    {
        startTime = time(NULL);


        endTime = time(NULL);

        timeDifference = difftime(endTime, startTime);

        for (int ww = 0; ww < whaleList.size(); ww++)
        {
            whaleList[ww].updatePosition (timeDifference);
        }
    }
    */

    return EXIT_SUCCESS;
}

