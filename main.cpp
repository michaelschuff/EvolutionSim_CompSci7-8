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
    int numWhales = 10; //the total number of whales that have ever existed


    //make whales w/ random starting traits
    for (int ww = 0; ww < numWhales; ww ++)
    {
        whale newWhale((rand() % 10) + 1,(rand() % 10) + 1, vector3(), vector3(20,0,0));
        whaleList.push_back(newWhale);
    }

    //loop of program
    //while(true)
    //{
        startTime = time(NULL);

        cout << "size: " << whaleList.size() << endl;

        for (int ll = 0; ll < whaleList.size(); ll++)
        {
            cout << whaleList[ll].position.x << endl;
        }

        endTime = time(NULL);

        timeDifference = difftime(endTime, startTime);

        //for (int ww = 0; ww < whaleList.size(); ww++)
        //{
          //  whaleList[ww].updatePosition (timeDifference);
        //}
    //}

    return EXIT_SUCCESS;
}

