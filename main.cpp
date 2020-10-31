#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <iomanip>
#include "vector3.hpp"
#include "triangle.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "objects2d.hpp"
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
    
    SliderSFML coh(5, 30);
    SliderSFML sep(5, 90);
    SliderSFML ali(5, 150);
    coh.create(0, 1);
    sep.create(0, 1);
    ali.create(0, 1);
    coh.setSliderValue(0.1);
    sep.setSliderValue(1);
    ali.setSliderValue(0.25);
    vector<object*> objects = {
        new line(vector3(0, 0, 0), vector3(1.5, 0, 0), color(255, 0, 0)),
        new line(vector3(0, 0, 0), vector3(0, 1.5, 0), color(0, 255, 0)),
        new line(vector3(0, 0, 0), vector3(0, 0, 1.5), color(0, 0, 255)),
    };
    // MARK: Evolution vars
    //whale model points
    vector3  wp1(  -1,  1.2,     0),
             wp2( 0.5,    1,   0.5),
             wp3( 0.5,  0.1,  0.35),
             wp4( 0.5,    1,  -0.5),
             wp5( 0.5,  0.1, -0.35),
             wp6(   1,    1,   0.5),
             wp7(   1,    1,  -0.5),
             wp8(   1,  0.3,   0.4),
             wp9(   1,  0.3,  -0.4),
            wp10( 2.2,    1,     0),
            wp11(-1.3,  1.6,   0.5),
            wp12(-1.3,  1.6,  -0.5),
            wp13( 1.7,  0.8,   0.2),
            wp14( 1.8,  0.8,  0.18),
            wp15(1.75, 0.85,  0.19),
            wp16( 1.7,  0.8,  -0.2),
            wp17( 1.8,  0.8, -0.18),
            wp18(1.75, 0.85, -0.19);
    //fish model points
    vector3  fp1(1,   0,     0),
             fp2(0, 0.2,     0),
             fp3(0,-0.1,-0.172),
             fp4(0,-0.1, 0.172);
    vector<whale> whaleList;
    vector<fish> fishList;
    double sensitivity = 0.1, speed = 5;
    vector<color> whale_colors, fish_colors;
    for (int i = 0; i < 19; i++) {
        whale_colors.push_back(color((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX));
    }
    for (int i = 0; i < 4; i++) {
        fish_colors.push_back(color((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX));
    }
    int numWhales = 10;
    vector3 limits(100, 100, 100);
    int numReproduce = numWhales * 0.10;
    int numDie = numWhales * 0.10;
    int totalWhales = numWhales;
    //make whales w/ random starting traits
    for (int w = 0; w < numWhales; w++) {
        whale newWhale((rand() % 10) + 1,(rand() % 10) + 1, vector3((rand() % (int) limits.x), (rand() % (int) limits.y), (rand() % (int) limits.z)), vector3(rand(), rand(), rand()).normalized(), limits, framerate);
        newWhale.id = w;
        whaleList.push_back(newWhale);
    }
    //make fish
    for (int f = 0; f < 300; f++) {
        fish newFish(vector3((rand() % (int) limits.x), (rand() % (int) limits.y), (rand() % (int) limits.z)), vector3(), framerate);
        newFish.id = f;
        fishList.push_back(newFish);
    }
    
    
    while (window.isOpen()) {
        active = window.hasFocus();
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
            }
        }
        
        if (active) {
            // MARK: Handle Inputs
            Vector2i mouse_position = Mouse::getPosition(window);
            if (mouse_position.x != width / 2) {
//                cam.rotate(vector3(0, 1, 0), sensitivity * -(mouse_position.x - width/2) * 3.14159 / 180.0);
            }
            if (mouse_position.y != height / 2 && cam.up.rotated(cam.right, sensitivity * -(mouse_position.y - height/2) * 3.14159 / 180.0).y > 0) {
//                cam.rotate(cam.right, sensitivity * -(mouse_position.y - height/2) * 3.14159 / 180.0);
            }
//            Mouse::setPosition(Vector2i(width/2, height/2), window);
            
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
            
            // MARK: Update Agents
            for (int i = 0; i < fishList.size(); i++) {
                fishList[i].cohesion = coh.getSliderValue();
                fishList[i].separation = sep.getSliderValue();
                fishList[i].alignment = ali.getSliderValue();
                fishList[i].updateFish(fishList);
            }
            vector<int> deadFishId;
            for (int w = 0; w < whaleList.size(); w++) {
                whaleList[w].decision(fishList);
                if (whaleList[w].eat == true) {// when whales are able to eat
                    for (int e = whaleList[w].foodList.size() - 1; e >= 0; e--) {//go through the list of food
                        for (int f = fishList.size() - 1; f >= 0; f--) {//see if the IDs match those of fish
                            if (fishList[f].id == whaleList[w].foodList[e]) {
                                //eat the fish
//                                deadFishId.push_back(f);
//                                whaleList[w].foodList.erase(whaleList[w].foodList.begin() + e);
                            }
                        }
                    }
                } else { //move whale
                    whaleList[w].updatePosition();
                }
            }
            
            for (int i = 0; i < deadFishId.size(); i++) {
//                fishList.erase(fishList.begin() + deadFishId[i]);
            }
            
            //go through whales and see which ones die
            //sort the whales based on number of fish they've eaten
            for (int i = 0; i < whaleList.size(); i++) {
//                int j = i;
//                while (j > 0 and ((float) whaleList[j].fishCounter / whaleList[j].age) < ((float) whaleList[j-1].fishCounter / whaleList[j-1].age)) {
//                    swap(whaleList[j], whaleList[j - 1]);
//                    j = j - 1;
//                }
            }

            //bottom 10% die
            for (int d = 0; d < numDie; d++) {
//                whaleList.erase(whaleList.begin() + d);
            }

            //top 10% reproduce
            for (int r = whaleList.size() - 1; r > whaleList.size() - numReproduce - 1; r--) {
//                whale newWhale(whaleList[r].eatCloseFish, whaleList[r].eatDenseFish, whaleList[r].position, vector3(), limits);
//                totalWhales++;
//                newWhale.id = totalWhales;
//                whaleList.push_back(newWhale);
            }
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
            
//            for (int i = 0; i < fishList.size(); i++) {
//                objects.push_back(new vector3(fishList[i].position));
//            }
            
            for (int i = 0; i < fishList.size(); i++) {
                quaternion q = get_quaternion(vector3(1, 0, 0), fishList[i].velocity);
                objects.push_back(new triangle(fishList[i].position + fp2.rotated(q), fishList[i].position + fp3.rotated(q), fishList[i].position + fp4.rotated(q), fish_colors[0]));
                objects.push_back(new triangle(fishList[i].position + fp1.rotated(q), fishList[i].position + fp3.rotated(q), fishList[i].position + fp2.rotated(q), fish_colors[1]));
                objects.push_back(new triangle(fishList[i].position + fp1.rotated(q), fishList[i].position + fp2.rotated(q), fishList[i].position + fp4.rotated(q), fish_colors[2]));
                objects.push_back(new triangle(fishList[i].position + fp1.rotated(q), fishList[i].position + fp3.rotated(q), fishList[i].position + fp4.rotated(q), fish_colors[3]));
            }
//            objects.push_back(new triangle(fp2, fp3, fp4, fish_colors[0]));
//            objects.push_back(new triangle(fp1, fp3, fp2, fish_colors[1]));
//            objects.push_back(new triangle(fp1, fp2, fp4, fish_colors[2]));
//            objects.push_back(new triangle(fp1, fp3, fp4, fish_colors[3]));
//            for (int i = 0; i < whaleList.size(); i++) {
//                quaternion q = get_quaternion(vector3(1, 0, 0), whaleList[i].velocity);
//                objects.push_back(new triangle(whaleList[i].position + wp1.rotated(q), whaleList[i].position + wp5.rotated(q), whaleList[i].position + wp3.rotated(q), whale_colors[0]));
//                objects.push_back(new triangle(whaleList[i].position + wp5.rotated(q), whaleList[i].position + wp1.rotated(q), whaleList[i].position + wp3.rotated(q), whale_colors[0]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp2.rotated(q), whaleList[i].position + wp1.rotated(q), whaleList[i].position + wp3.rotated(q), whale_colors[1]));
//                objects.push_back(new triangle(whaleList[i].position + wp1.rotated(q), whaleList[i].position + wp2.rotated(q), whaleList[i].position + wp3.rotated(q), whale_colors[1]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp1.rotated(q), whaleList[i].position + wp4.rotated(q), whaleList[i].position + wp2.rotated(q), whale_colors[2]));
//                objects.push_back(new triangle(whaleList[i].position + wp4.rotated(q), whaleList[i].position + wp1.rotated(q), whaleList[i].position + wp2.rotated(q), whale_colors[2]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp1.rotated(q), whaleList[i].position + wp4.rotated(q), whaleList[i].position + wp5.rotated(q), whale_colors[3]));
//                objects.push_back(new triangle(whaleList[i].position + wp4.rotated(q),whaleList[i].position + wp1.rotated(q) , whaleList[i].position + wp5.rotated(q), whale_colors[3]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp2.rotated(q), whaleList[i].position + wp3.rotated(q), whaleList[i].position + wp6.rotated(q), whale_colors[4]));
//                objects.push_back(new triangle(whaleList[i].position + wp3.rotated(q), whaleList[i].position + wp2.rotated(q), whaleList[i].position + wp6.rotated(q), whale_colors[4]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp4.rotated(q), whaleList[i].position + wp7.rotated(q), whaleList[i].position + wp5.rotated(q), whale_colors[5]));
//                objects.push_back(new triangle(whaleList[i].position + wp5.rotated(q), whaleList[i].position + wp7.rotated(q), whaleList[i].position + wp4.rotated(q), whale_colors[5]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp8.rotated(q), whaleList[i].position + wp6.rotated(q), whaleList[i].position + wp3.rotated(q), whale_colors[6]));
//                objects.push_back(new triangle(whaleList[i].position + wp6.rotated(q), whaleList[i].position + wp8.rotated(q), whaleList[i].position + wp3.rotated(q), whale_colors[6]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp9.rotated(q), whaleList[i].position + wp7.rotated(q), whaleList[i].position + wp5.rotated(q), whale_colors[7]));
//                objects.push_back(new triangle(whaleList[i].position + wp7.rotated(q), whaleList[i].position + wp9.rotated(q), whaleList[i].position + wp5.rotated(q), whale_colors[7]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp4.rotated(q), whaleList[i].position + wp2.rotated(q), whaleList[i].position + wp7.rotated(q), whale_colors[8]));
//                objects.push_back(new triangle(whaleList[i].position + wp2.rotated(q), whaleList[i].position + wp4.rotated(q), whaleList[i].position + wp7.rotated(q), whale_colors[8]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp7.rotated(q), whaleList[i].position + wp2.rotated(q), whaleList[i].position + wp6.rotated(q), whale_colors[9]));
//                objects.push_back(new triangle(whaleList[i].position + wp2.rotated(q), whaleList[i].position + wp7.rotated(q), whaleList[i].position + wp6.rotated(q), whale_colors[9]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp3.rotated(q), whaleList[i].position + wp5.rotated(q), whaleList[i].position + wp8.rotated(q), whale_colors[10]));
//                objects.push_back(new triangle(whaleList[i].position + wp5.rotated(q), whaleList[i].position + wp3.rotated(q), whaleList[i].position + wp8.rotated(q), whale_colors[10]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp5.rotated(q), whaleList[i].position + wp9.rotated(q), whaleList[i].position + wp8.rotated(q), whale_colors[11]));
//                objects.push_back(new triangle(whaleList[i].position + wp9.rotated(q), whaleList[i].position + wp5.rotated(q), whaleList[i].position + wp8.rotated(q), whale_colors[11]));
//                        //Head
//                objects.push_back(new triangle(whaleList[i].position + wp10.rotated(q), whaleList[i].position + wp9.rotated(q), whaleList[i].position + wp8.rotated(q), whale_colors[12]));
//                objects.push_back(new triangle(whaleList[i].position + wp9.rotated(q), whaleList[i].position + wp10.rotated(q), whaleList[i].position + wp8.rotated(q), whale_colors[12]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp7.rotated(q), whaleList[i].position + wp9.rotated(q), whaleList[i].position + wp10.rotated(q), whale_colors[13]));
//                objects.push_back(new triangle(whaleList[i].position + wp9.rotated(q), whaleList[i].position + wp7.rotated(q), whaleList[i].position + wp10.rotated(q), whale_colors[13]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp7.rotated(q), whaleList[i].position + wp6.rotated(q), whaleList[i].position + wp10.rotated(q), whale_colors[14]));
//                objects.push_back(new triangle(whaleList[i].position + wp6.rotated(q), whaleList[i].position + wp7.rotated(q), whaleList[i].position + wp10.rotated(q), whale_colors[14]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp6.rotated(q), whaleList[i].position + wp8.rotated(q), whaleList[i].position + wp10.rotated(q), whale_colors[15]));
//                objects.push_back(new triangle(whaleList[i].position + wp8.rotated(q), whaleList[i].position + wp6.rotated(q), whaleList[i].position + wp10.rotated(q), whale_colors[15]));
//
//                //Tail
//                objects.push_back(new triangle(whaleList[i].position + wp12.rotated(q), whaleList[i].position + wp11.rotated(q), whaleList[i].position + wp1.rotated(q), whale_colors[16]));
//                objects.push_back(new triangle(whaleList[i].position + wp11.rotated(q), whaleList[i].position + wp12.rotated(q), whaleList[i].position + wp1.rotated(q), whale_colors[16]));
//
//                //Eyes
//                objects.push_back(new triangle(whaleList[i].position + wp13.rotated(q), whaleList[i].position + wp15.rotated(q), whaleList[i].position + wp14.rotated(q), whale_colors[17]));
//                objects.push_back(new triangle(whaleList[i].position + wp15.rotated(q), whaleList[i].position + wp13.rotated(q), whaleList[i].position + wp14.rotated(q), whale_colors[17]));
//
//                objects.push_back(new triangle(whaleList[i].position + wp16.rotated(q), whaleList[i].position + wp17.rotated(q), whaleList[i].position + wp18.rotated(q), whale_colors[18]));
//                objects.push_back(new triangle(whaleList[i].position + wp17.rotated(q), whaleList[i].position + wp16.rotated(q), whaleList[i].position + wp18.rotated(q), whale_colors[18]));
//            }
            
            // MARK: Draw Shapes to Window
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
            window.display();
        }
        
    }

    return EXIT_SUCCESS;
}
