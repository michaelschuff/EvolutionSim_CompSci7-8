#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
#include <iomanip>
#include "vector3.hpp"
#include "triangle.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "objects2d.hpp"
#include "evolutionSim.hpp"
#include <math.h>
#include "quaternion.hpp"
#include <cstdlib>
#include <ctime>
#include "color.hpp"
#include <fstream>
#include <string>
#include "whale.hpp"
#include "fish.hpp"
#include "SliderSFML.hpp"
//#include "ResourcePath.hpp"

using namespace sf;
using namespace std;

int agent::nextID = 0;

int main(int, char const**) {
    // MARK: Program vars
    srand(time(NULL));
    const int width = 800, height = 800, framerate = 60;
    bool active = true, qDown = false, wDown = false, eDown = false, aDown = false,sDown = false, dDown = false, lcontrolDown = false, jDown = false, kDown = false, lDown = false, iDown = false;
    RenderWindow window(VideoMode(width, height), "SFML window");
//    window.setMouseCursorVisible(false);
    camera cam(vector3(0, 150, -80), vector3(0.253319, -0.590397, 0.76476).normalized(), vector3(0.185644, 0.805622, 0.560449).normalized(), vector3(-0.948137, 0, 0.314062).normalized(), 3.14159 / 3, 1);
    window.setFramerateLimit(framerate);
    int frameCounter = 0;
    Font font;
    if(!font.loadFromFile("oswald.ttf")){
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

    vector<object*> objects = {
        new line(vector3(0, 0, 0), vector3(1.5, 0, 0), color(255, 0, 0)),
        new line(vector3(0, 0, 0), vector3(0, 1.5, 0), color(0, 255, 0)),
        new line(vector3(0, 0, 0), vector3(0, 0, 1.5), color(0, 0, 255)),
    };
    // MARK: Evolution vars
    //whale model points
    vector<vector3> pointlist;
    vector<triangle> tlist;
    string check;
    float x;
    float y;
    float z;

    int counter = 0;

    ifstream objfile;
    objfile.open("whalemodel.obj", ios::in);
    bool vert = true;

    while(true){
        objfile>>check;
        if(check == 'v'){
            objfile>>x;
            objfile>>y;
            objfile>>z;
            cout<< x << " "<< y << " " << z << endl;
            pointlist.push_back(vector3(x,y,z));

        } else if (check == 'f'){
            objfile>>x;
            objfile>>y;
            objfile>>z;
            cout<< x << " "<< y << " " << z << endl;
            tlist.push_back(triangle(pointlist[x-1],pointlist[y-1],pointlist[z-1]));

        } else{
            string s;
            getline(objfile,s);
        }

        if(objfile.eof()){
            break;
        }


    }
    objfile.close();
    //fish model points
    vector3  fp1(1,   0,     0),
             fp2(0, 0.2,     0),
             fp3(0,-0.1,-0.172),
             fp4(0,-0.1, 0.172);

    double sensitivity = 0.1, speed = 5;
    vector<color> whale_colors, fish_colors;
    for (int i = 0; i < 19; i++) {
        whale_colors.push_back(color((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX));
    }
    for (int i = 0; i < 4; i++) {
        fish_colors.push_back(color((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX));
    }

    //create a simulation object
    int numWhales = 10;
    //400 is too many for processing
    int numFish = 300;
    vector3 limits(100, 100, 100);

    evolutionSim simulation(numWhales, numFish, limits, framerate);
    bool start = false;
    bool leftDown = false;

    while (window.isOpen()) {
        // MARK: Handle Events
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::GainedFocus) {
//                window.setMouseCursorVisible(false);
            } else if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                    case Keyboard::A:
                        aDown = true;
                        break;
                    case Keyboard::D:
                        dDown = true;
                        break;
                    case Keyboard::S:
                        sDown = true;
                        break;
                    case Keyboard::W:
                        wDown = true;
                        break;
                    case Keyboard::I:
                        iDown = true;
                        break;
                    case Keyboard::J:
                        jDown = true;
                        break;
                    case Keyboard::K:
                        kDown = true;
                        break;
                    case Keyboard::L:
                        lDown = true;
                        break;
                    case Keyboard::LShift:
                        qDown = true;
                        break;
                    case Keyboard::LControl:
                        lcontrolDown = true;
                        break;
                    case Keyboard::Space:
                        eDown = true;
                        break;
                    case Keyboard::Left:
                        aDown = true;
                        break;
                    case Keyboard::Right:
                        dDown = true;
                        break;
                    case Keyboard::Up:
                        wDown = true;
                        break;
                    case Keyboard::Down:
                        sDown = true;
                        break;
                    default:
                        break;
                }
            } else if (event.type == Event::KeyReleased) {
                switch (event.key.code) {
                    case Keyboard::A:
                        aDown = false;
                        break;
                    case Keyboard::D:
                        dDown = false;
                        break;
                    case Keyboard::S:
                        sDown = false;
                        break;
                    case Keyboard::W:
                        wDown = false;
                        break;
                    case Keyboard::I:
                        iDown = false;
                        break;
                    case Keyboard::J:
                        jDown = false;
                        break;
                    case Keyboard::K:
                        kDown = false;
                        break;
                    case Keyboard::L:
                        lDown = false;
                        break;
                    case Keyboard::Escape:
                        window.close();
                        break;
                    case Keyboard::LShift:
                        qDown = false;
                        break;
                    case Keyboard::LControl:
                        lcontrolDown = false;
                        break;
                    case Keyboard::Space:
                        eDown = false;
                        break;
                    case Keyboard::Left:
                        aDown = false;
                        break;
                    case Keyboard::Right:
                        dDown = false;
                        break;
                    case Keyboard::Up:
                        wDown = false;
                        break;
                    case Keyboard::Down:
                        sDown = false;
                        break;
                    default:
                        break;
                }
            } else if (event.type == Event::MouseWheelScrolled) {
                for (int i = 0; i < event.mouseWheelScroll.delta; i++) {
                    speed *= 1.01;
                }
                for (int i = 0; i > event.mouseWheelScroll.delta; i--) {
                    speed /= 1.01;
                }
            } else if (event.type == Event::MouseButtonPressed){
                if(event.mouseButton.button == Mouse::Left){
                    leftDown = true;
                }

            }else if (event.type == Event::MouseButtonReleased and leftDown == true){
                if(event.mouseButton.button == Mouse::Left){
                    leftDown = false;
                }

            }

            //if button is pushed make start = true

        }

        // MARK: Handle Inputs
        Vector2i mouse_position = Mouse::getPosition(window);
        if (mouse_position.x != width / 2) {
//            cam.rotate(vector3(0, 1, 0), sensitivity * -(mouse_position.x - width/2) * 3.14159 / 180.0);
        }
        if (mouse_position.y != height / 2 && cam.up.rotated(cam.right, sensitivity * -(mouse_position.y - height/2) * 3.14159 / 180.0).y > 0) {
//            cam.rotate(cam.right, sensitivity * -(mouse_position.y - height/2) * 3.14159 / 180.0);
        }
//        Mouse::setPosition(Vector2i(width/2, height/2), window);

        if (iDown) {
            cam.rotate(cam.right, sensitivity * 20 * 3.14159 / 180.0);
        }
        if (kDown) {
            cam.rotate(cam.right, -sensitivity * 20 * 3.14159 / 180.0);
        }
        if (jDown) {
            cam.rotate(vector3(0, 1, 0), sensitivity * 20 * 3.14159 / 180.0);
        }
        if (lDown) {
            cam.rotate(vector3(0, 1, 0), -sensitivity * 20 * 3.14159 / 180.0);
        }
        if (wDown) {
            cam.position += speed * cam.forward / 60.0;
        }
        if (sDown) {
            cam.position -= speed * cam.forward / 60.0;
        }
        if (aDown) {
            cam.position -= speed * cam.right / 60.0;
        }
        if (dDown) {
            cam.position += speed * cam.right / 60.0;
        }
        if (eDown) {
            cam.position += speed * cam.up / 60.0;
        }
        if (qDown) {
            cam.position -= speed * cam.up / 60.0;
        }

        simulation.updateSim(coh.getSliderValue(), sep.getSliderValue(), ali.getSliderValue(), avo.getSliderValue());

        window.clear();


        objects = {
            new line(vector3(0, 0, 0), vector3(100, 0, 0), color(0.5, 0.5, 0.5)),
            new line(vector3(100, 0, 0), vector3(100, 0, 100), color(0.5, 0.5, 0.5)),
            new line(vector3(100, 0, 100), vector3(0, 0, 100), color(0.5, 0.5, 0.5)),
            new line(vector3(0, 0, 100), vector3(0, 0, 0), color(0.5, 0.5, 0.5)),
            new line(vector3(0, 0, 0), vector3(0, 100, 0), color(0.5, 0.5, 0.5)),
            new line(vector3(100, 0, 0), vector3(100, 100, 0), color(0.5, 0.5, 0.5)),
            new line(vector3(0, 0, 100), vector3(0, 100, 100), color(0.5, 0.5, 0.5)),
            new line(vector3(100, 0, 100), vector3(100, 100, 100), color(0.5, 0.5, 0.5)),
            new line(vector3(0, 100, 0), vector3(100, 100, 0), color(0.5, 0.5, 0.5)),
            new line(vector3(100, 100, 0), vector3(100, 100, 100), color(0.5, 0.5, 0.5)),
            new line(vector3(100, 100, 100), vector3(0, 100, 100), color(0.5, 0.5, 0.5)),
            new line(vector3(0, 100, 100), vector3(0, 100, 0), color(0.5, 0.5, 0.5)),
        };

        for (int i = 0; i < simulation.fishList.size(); i++) {
            quaternion q = get_quaternion(vector3(1, 0, 0), simulation.fishList[i].velocity);
            objects.push_back(new triangle(simulation.fishList[i].position + fp2.rotated(q),
                                           simulation.fishList[i].position + fp3.rotated(q),
                                           simulation.fishList[i].position + fp4.rotated(q),
                                           fish_colors[0]));
            objects.push_back(new triangle(simulation.fishList[i].position + fp1.rotated(q),
                                           simulation.fishList[i].position + fp3.rotated(q),
                                           simulation.fishList[i].position + fp2.rotated(q),
                                           fish_colors[1]));
            objects.push_back(new triangle(simulation.fishList[i].position + fp1.rotated(q),
                                           simulation.fishList[i].position + fp2.rotated(q),
                                           simulation.fishList[i].position + fp4.rotated(q),
                                           fish_colors[2]));
            objects.push_back(new triangle(simulation.fishList[i].position + fp1.rotated(q),
                                           simulation.fishList[i].position + fp3.rotated(q),
                                           simulation.fishList[i].position + fp4.rotated(q),
                                           fish_colors[3]));

        }

        for (int j = 0; j < simulation.whaleList.size(); j++) {
            quaternion q = get_quaternion(vector3(1, 0, 0), simulation.whaleList[j].velocity);
            for (int i = 0; i < tlist.size(); i++) {
                objects.push_back(new triangle(simulation.whaleList[j].position + tlist[i].v1.rotated(q),
                                               simulation.whaleList[j].position + tlist[i].v2.rotated(q),
                                               simulation.whaleList[j].position + tlist[i].v3.rotated(q),
                                               whale_colors[i%19]));
            }
        }


        // MARK: Draw Shapes to Window
        if(start == false){
            window.draw(titleTxt);
            window.draw(subTxt);
            window.draw(button);
            window.draw(startTxt);
            if(leftDown == true and button.getGlobalBounds().contains(mouse_position.x,mouse_position.y)){
                start = true;
                cout<< "started\n";
            }

        }
        else{
            circle* _circ = nullptr;
            rectangle* _rect = nullptr;
            convexshape* _convexshape = nullptr;
            vector<object2D*> shapes = cam.get_view(objects);
            for (int i = 0; i < shapes.size(); i++) {
                if ((_circ = dynamic_cast<circle*>(shapes[i]))) {
                    CircleShape c(_circ->r);
                    c.setOrigin(_circ->r, _circ->r);
                    c.setPosition(width/2 + (width/2)*_circ->x, height/2 - (height/2)*_circ->y);
                    c.setFillColor(_circ->c);

                    window.draw(c);
                } else if ((_rect = dynamic_cast<rectangle*>(shapes[i]))) {
                    RectangleShape r(Vector2f(width/2 * _rect->width, _rect->height));
                    r.setOrigin(0.5, 0.5);
                    r.setPosition(width/2 + (width/2)*_rect->x, height/2 - (height/2)*_rect->y);
                    r.rotate(-_rect->theta * 180.0 / 3.14159);
                    r.setFillColor(_rect->c);

                    window.draw(r);
                } else if ((_convexshape = dynamic_cast<convexshape*>(shapes[i]))) {
                    ConvexShape c(_convexshape->points.size());
                    for (int j = 0; j < _convexshape->points.size(); j++) {
                        c.setPoint(j, Vector2f(width/2 + (width/2)*(_convexshape->points[j][0]), height/2 - (height/2)*(_convexshape->points[j][1])));
                    }
                    c.setFillColor(_convexshape->c);
                    window.draw(c);
                }
            }
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
