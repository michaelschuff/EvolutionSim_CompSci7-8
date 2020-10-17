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

//global variable. Turn into other type variable once I figure out how
vector<fish> allFish = vector<fish>();

int agent::nextID = 0; ///Necessary line

int main(int, char const**) {

    //add new fish to the allAgents vector and update an id variable
    for(int ii = 0; ii < 10; ii ++) {
        fish MrDarp(vector3(0, 100*(ii%5+1), ii*50), vector3(0, ii, 0));
        allFish.push_back(MrDarp);
        cout<<MrDarp.id<<endl;
        fish JohnNash;
        cout<<JohnNash.id<<endl;
    }


    return EXIT_SUCCESS;
}
