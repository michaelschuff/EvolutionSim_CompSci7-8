#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include "vector3.hpp"
#include "triangle.hpp"
#include "mesh.hpp"
#include "agent.hpp"
#include "fish.hpp"
//#include "ResourcePath.hpp"

using namespace sf;
using namespace std;

//global variables and static var instantiations. Turn into other type variable once I figure out how
//For now, copy this into main.cpp
vector<fish> allFish = vector<fish>();
int agent::nextID = 0; ///Necessary line
vector<fish> *fish::allFishList = &allFish;

int main(int, char const**) {

    //two fish, 1 unit apart
    allFish.push_back(fish(vector3(-1, 0, 0), vector3(1, 0, 0)));
    allFish.push_back(fish(vector3(0, 1, 0), vector3(-1, 0, 0)));
    allFish.push_back(fish(vector3(1, 0, 0), vector3(1, 0, 0)));
    allFish.push_back(fish(vector3(0, -1, 0), vector3(0, 0, 0)));

    allFish[3].vAlignment().print();
    return EXIT_SUCCESS;
}
