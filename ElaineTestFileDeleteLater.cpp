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

    RenderWindow window(VideoMode(1000, 1000), "2DBoids");
    //two fish, 1 unit apart
    allFish.push_back(fish(vector3(100, 200, 0), vector3(-1, 0, 0)));
    allFish.push_back(fish(vector3(200, 200, 0), vector3(0, 0, 0)));
    allFish.push_back(fish(vector3(200, 100, 0), vector3(0, 3, 0)));
    allFish.push_back(fish(vector3(700, 700, 0), vector3(0, 0, 0)));
    allFish.push_back(fish(vector3(300, 500, 0), vector3(-1, 0, 0)));
    allFish.push_back(fish(vector3(500, 500, 0), vector3(0, 0, 0)));
    allFish.push_back(fish(vector3(900, 100, 0), vector3(0, 3, 0)));
    allFish.push_back(fish(vector3(400, 700, 0), vector3(0, 0, 0)));
    allFish.push_back(fish(vector3(140, 200, 0), vector3(-1, 0, 0)));
    allFish.push_back(fish(vector3(240, 200, 0), vector3(0, 0, 0)));
    allFish.push_back(fish(vector3(200, 140, 0), vector3(0, 3, 0)));
    allFish.push_back(fish(vector3(700, 740, 0), vector3(0, 0, 0)));
    allFish.push_back(fish(vector3(340, 500, 0), vector3(-1, 0, 0)));
    allFish.push_back(fish(vector3(500, 540, 0), vector3(0, 0, 0)));
    allFish.push_back(fish(vector3(940, 100, 0), vector3(0, 3, 0)));
    allFish.push_back(fish(vector3(400, 740, 0), vector3(0, 0, 0)));

    allFish[1].vSeparation().print();

    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
        }

        window.clear();
        //draw stuff here
        for(int ii = 0; ii < allFish.size(); ii ++){
            allFish[ii].updateFish();
            window.draw(allFish[ii].Shape2D);
        }
        window.display();
    }
    return EXIT_SUCCESS;
}
