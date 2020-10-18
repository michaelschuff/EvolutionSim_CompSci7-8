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
vector<fish> allFish = vector<fish>();
int agent::nextID = 0; ///Necessary line
vector<fish> *fish::allFishList = &allFish;

int main(int, char const**) {


    //add new fish to the allAgents vector and update an id variable
    for(int ii = 0; ii < 10; ii ++) {
        allFish.push_back(fish(vector3(), vector3()));
        allFish[ii].reportFishNumbers();
    }
    allFish[0].reportFishNumbers();

    return EXIT_SUCCESS;
}
