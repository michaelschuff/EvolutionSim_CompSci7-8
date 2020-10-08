#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include<time.h>
#include <vector>
#include<stdio.h>
#include<stdlib.h>
#include "agent.hpp"
#include "whale.hpp"
//#include "ResourcePath.hpp"

using namespace sf;
using namespace std;


int main() {
    srand(time(NULL));

    vector<whale> whaleList;
    int numWhales = 10;

    //make whales w/ random starting traits
    for (int ww = 0; ww < numWhales; ww ++)
    {
        whale newWhale((rand() % 10) + 1,(rand() % 10) + 1, vector3(), vector3(20,0,0));
        whaleList.push_back(newWhale);
    }

    return EXIT_SUCCESS;
}

