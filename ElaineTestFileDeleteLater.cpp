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
vector<agent> allAgents = vector<agent>();

int main(int, char const**) {
    fish MrDarp(vector3(), vector3(20, 0, 0));

    allAgents.push_back(MrDarp);

    cout<<"Mr. Darp's x velocity is "<<MrDarp.velocity.x<<endl;

    MrDarp.velocity += vector3(0, 50, 0);
    cout<<"Mr. Darp's y velocity is now "<<MrDarp.velocity.y<<endl;

    return EXIT_SUCCESS;
}
