#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include "vector3.hpp"
#include "triangle.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "objects2d.hpp"
#include "evolutionSim.hpp"
#include "quaternion.hpp"
#include "color.hpp"
#include "whale.hpp"
#include "fish.hpp"
#include "SliderSFML.hpp"
#include "scene.hpp"
#include "loadDataFunctions.hpp"
#include "garbage.hpp"
#include "bnfGroup.hpp"
//#include "ResourcePath.hpp"

using namespace sf;
using namespace std;

int agent::nextID = 0;

int main(int, char const**) {
    srand(time(NULL));
    // MARK: Program vars
    const int width = 800, height = 800, framerate = 60;
    bool active = true, qDown = false, wDown = false, eDown = false,
         aDown = false, sDown = false, dDown = false, lcontrolDown = false,
         jDown = false, kDown = false, lDown = false, iDown = false,
         leftDown = false, hasStarted = false;

    RenderWindow window(VideoMode(width, height), "SFML window");
    window.setFramerateLimit(framerate);

    camera cam(vector3(0, 150, -80),
               vector3(0.253319, -0.590397, 0.76476).normalized(),
               vector3(0.185644, 0.805622, 0.560449).normalized(),
               vector3(-0.948137, 0, 0.314062).normalized(), 3.14159 / 3, 1);
    Font font;
//    if(!font.loadFromFile("/Users/michael/Downloads/oswald.ttf")){
    if(!font.loadFromFile("Resources/oswald.ttf")){
        cout<< "font error \n";
    }
    Text cohTxt, sepTxt, aliTxt, avoTxt;
    cohTxt.setString("Fish Cohesion");
    sepTxt.setString("Fish Separation");
    aliTxt.setString("Fish Alignment");
    avoTxt.setString("Fish Avoidance");
    cohTxt.setFont(font);
    sepTxt.setFont(font);
    aliTxt.setFont(font);
    avoTxt.setFont(font);
    cohTxt.setPosition(5,10);
    sepTxt.setPosition(5,70);
    aliTxt.setPosition(5,130);
    avoTxt.setPosition(500,10);

    Text titleTxt("Whale Bubble Net Feeding Simulation" ,font,45);
    Text subTxt("by Bronte McKinnis, Elaine Demetrion, Jack Weinberg and Michael Schuff ",font,22);
    Text startTxt("Start",font, 35);
    subTxt.setPosition(50, 250);
    titleTxt.setPosition(50, 150);
    RectangleShape button;
    button.setSize(Vector2f(140,60));
    button.setFillColor(Color::White);
    button.setPosition(50,500);
    startTxt.setPosition(50,500);
    startTxt.setFillColor(Color::Black);

    SliderSFML coh(5, 30);
    SliderSFML sep(5, 90);
    SliderSFML ali(5, 150);
    SliderSFML avo(500, 30);
    coh.create(0, 1);
    sep.create(0, 1);
    ali.create(0, 1);
    avo.create(0, 1);
    coh.setSliderValue(0.1);
    sep.setSliderValue(1);
    ali.setSliderValue(0.25);
    avo.setSliderValue(0);

    double sensitivity = 0.05, speed = 20;
    vector<color> fish_colors;

    evolutionSim simulation(10, 300, vector3(100, 100, 100), framerate);

    mesh fish_mesh = getFishMesh();
    mesh whale_mesh = getWhaleMesh();

    scene world(getPoints(), getLines(), vector<triangle>(0), getBodies(simulation.fishList, simulation.whaleList, fish_mesh, whale_mesh));



    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed: {
                    window.close();
                    break;
                } case Event::KeyPressed: {
                    HandleKeyPressedEvent;// garbage.hpp
                    break;
                } case Event::KeyReleased: {
                    HandleKeyReleasedEvent;// garbage.hpp
                    break;
                } case Event::MouseWheelScrolled: {
                    for (int i = 0; i < event.mouseWheelScroll.delta; i++) {
                        speed *= 1.01;
                    }
                    for (int i = 0; i > event.mouseWheelScroll.delta; i--) {
                        speed /= 1.01;
                    }
                    break;
                } case Event::MouseButtonPressed: {
                    if (event.mouseButton.button == Mouse::Left) {
                        leftDown = true;
                    }
                    break;
                } case Event::MouseButtonReleased: {
                    if (event.mouseButton.button == Mouse::Left) {
                        leftDown = false;
                    }
                    break;
                }
            }
        }



        window.clear();
        if (!hasStarted){
            window.draw(titleTxt);
            window.draw(subTxt);
            window.draw(button);
            window.draw(startTxt);
            Vector2i mouse_position = Mouse::getPosition(window);
            if(leftDown and button.getGlobalBounds().contains(mouse_position.x, mouse_position.y)){
                hasStarted = true;
            }
        } else {
            MoveCamera;// garbage.hpp
            simulation.updateSim(coh.getSliderValue(), sep.getSliderValue(), ali.getSliderValue(), avo.getSliderValue());

            world.meshes.clear();
            for (int i = 0; i < simulation.fishList.size() + simulation.whaleList.size(); i++) {
                if (i < simulation.fishList.size()) {
                    quaternion q = get_quaternion(vector3(1, 0, 0), simulation.fishList[i].velocity);
                    world.meshes.push_back(simulation.fishList[i].position + fish_mesh.rotated(q));
                } else {
                    quaternion q = get_quaternion(vector3(1, 0, 0), simulation.whaleList[i-simulation.fishList.size()].velocity);
                    world.meshes.push_back(simulation.whaleList[i-simulation.fishList.size()].position + whale_mesh.rotated(q));
                }
            }

            vector<triangle> tris = cam.get_view(world);
            for (triangle tri: tris) {
                if (tri.v2 != tri.v3) {
                    ConvexShape shape(3);
                    shape.setPoint(0, Vector2f(tri.v1.x, height-tri.v1.z));
                    shape.setPoint(1, Vector2f(tri.v2.x, height-tri.v2.z));
                    shape.setPoint(2, Vector2f(tri.v3.x, height-tri.v3.z));
                    shape.setFillColor(Color(tri.c.r, tri.c.g, tri.c.b));
                    window.draw(shape);
                } else {
                    float dx = tri.v2.x - tri.v1.x;
                    float dy = tri.v2.z - tri.v1.z;
                    RectangleShape shape(Vector2f(sqrt(dx*dx + dy*dy), 1));
                    shape.setOrigin(0.5, 0.5);
                    shape.setPosition(tri.v1.x, height-tri.v1.z);
                    shape.rotate(-atan2(dy, dx) * 180.0 / 3.14159);
                    shape.setFillColor(Color(tri.c.r, tri.c.g, tri.c.b));
                    window.draw(shape);

                }
            }

            //MARK: DRAW THEM
            coh.draw(window);
            sep.draw(window);
            ali.draw(window);
            avo.draw(window);
            window.draw(cohTxt);
            window.draw(sepTxt);
            window.draw(aliTxt);
            window.draw(avoTxt);
        }


        window.display();

    }


    return EXIT_SUCCESS;
}
