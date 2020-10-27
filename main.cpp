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
//#include "ResourcePath.hpp"

using namespace sf;
using namespace std;


bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
int main(int, char const**) {
    srand(time(NULL));
    const int width = 800, height = 800;
    vector3 p1(-0.5, -0.5, -0.5),
            p2( 0.5, -0.5, -0.5),
            p3(-0.5,  0.5, -0.5),
            p4(-0.5, -0.5,  0.5),
            p5(-0.5,  0.5,  0.5),
            p6( 0.5, -0.5,  0.5),
            p7( 0.5,  0.5, -0.5),
            p8( 0.5,  0.5,  0.5);
    int r, g, b;
    vector<color> v;
    for (int i = 0; i < 12; i++) {
        r = ((float) 255*rand()/RAND_MAX);
        g = ((float) 255*rand()/RAND_MAX);
        b = ((float) 255*rand()/RAND_MAX);
        v.push_back(color(r, g, b));
    }
    vector<object*> verticies = {
        new line(vector3(0, 0, 0), vector3(1.5, 0, 0), color(255, 0, 0)),
        new line(vector3(0, 0, 0), vector3(0, 1.5, 0), color(0, 255, 0)),
        new line(vector3(0, 0, 0), vector3(0, 0, 1.5), color(0, 0, 255)),
//        new line(p1, p2),
//        new line(p1, p3),
//        new line(p1, p4),
//        new line(p2, p6),
//        new line(p2, p7),
//        new line(p3, p5),
//        new line(p3, p7),
//        new line(p4, p5),
//        new line(p4, p6),
//        new line(p8, p5),
//        new line(p8, p6),
//        new line(p8, p7),
        
        new triangle(p4, p6, p8, v[0]),
        new triangle(p4, p8, p5, v[1]),
        new triangle(p6, p2, p7, v[2]),
        new triangle(p6, p7, p8, v[3]),
        new triangle(p2, p1, p3, v[4]),
        new triangle(p2, p3, p7, v[5]),
        new triangle(p1, p4, p5, v[6]),
        new triangle(p1, p5, p3, v[7]),
        new triangle(p5, p8, p7, v[8]),
        new triangle(p5, p7, p3, v[9]),
        new triangle(p6, p1, p2, v[10]),
        new triangle(p6, p4, p1, v[11]),
    };
//    vector<vector3> points;
//    vector<vector<int>> indicies;
//    ifstream myfile;
//    myfile.open(resourcePath() + "aca.obj");
//    string l;
//    int c = 0;
//    bool going = true;
//    while(getline(myfile, l)) {
//        going = true;
//        vector<string> split = {""};
//        for (int i = 0; i < l.size(); i++) {
//            if (l[i] == '/' || l[i] == '\r') {
//                going = false;
//            } else if (l[i] == ' ') {
//                going = true;
//                split.push_back("");
//            } else if (going) {
//                split[split.size() - 1] += l[i];
//            }
//        }
//        if (split[0] == "v") {
//            split.erase(split.begin());
////            c++;
////            if (c == 1) {
////                verticies.push_back(new vector3(stof(split[0]), -stof(split[1]), stof(split[2])));
////                c=0;
////            }
////            for (int i = 0; i < split.size(); i++) {
////                cout << split[i] << ", ";
////            }
////            cout << endl;
//            points.push_back(vector3(stof(split[1]), stof(split[2]), stof(split[3])));
//        } else if (split[0] == "f") {
//            split.erase(split.begin());
//            indicies.push_back(vector<int>());
//            for (int i = 0; i < split.size(); i++) {
//                if (is_number(split[i])) {
//                    indicies[indicies.size() - 1].push_back(stoi(split[i]));
//                }
//            }
//        }
//    }
//
//    myfile.close();
//
//    for (int i = 0; i < indicies.size(); i++) {
//        for (int j = 1; j + 1 < indicies[i].size(); j++) {
//            r = ((float) 255*rand()/RAND_MAX);
//            g = ((float) 255*rand()/RAND_MAX);
//            b = ((float) 255*rand()/RAND_MAX);
//            verticies.push_back(new triangle(vector3(points[indicies[i][0] - 1]), vector3(points[indicies[i][j] - 1]), vector3(points[indicies[i][j+1] - 1]), color(r, g, b)));
//        }
//    }
    double sensitivity = 0.1, speed = 5;
    RenderWindow window(VideoMode(width, height), "SFML window");
    window.setMouseCursorVisible(false);
    camera cam(vector3(5, 0, 0), vector3(-1, 0, 0).normalized(), vector3(0, 1, 0).normalized(), vector3(0, 0, -1).normalized(), 3.14159 / 3, 1);
    window.setFramerateLimit(60);
    
    
    bool active = true, qDown = false, wDown = false, eDown = false, aDown = false,sDown = false, dDown = false, lcontrolDown = false, jDown = false, kDown = false, lDown = false, iDown = false;
    while (window.isOpen()) {
//        cam.position.print();
//        cam.forward.print();
//        cam.up.print();
//        cam.right.print();
//        cout << endl;
        active = window.hasFocus();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::GainedFocus) {
                window.setMouseCursorVisible(false);
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
            Vector2i mouse_position = Mouse::getPosition(window);
            cout << mouse_position.x << "   " << mouse_position.y << endl;
            if (mouse_position.x != width / 2) {
//                cam.rotate(vector3(0, 1, 0), sensitivity * -(mouse_position.x - width/2) * 3.14159 / 180.0);
            }
            if (mouse_position.y != height / 2 && cam.up.rotated(cam.right, sensitivity * -(mouse_position.y - height/2) * 3.14159 / 180.0).y > 0) {
//                cam.rotate(cam.right, sensitivity * -(mouse_position.y - height/2) * 3.14159 / 180.0);
            }
//            Mouse::setPosition(Vector2i(width/2, height/2), window);
            
            if (iDown) {
                cam.rotate(cam.right, sensitivity * 10 * 3.14159 / 180.0);
            }
            
            if (kDown) {
                cam.rotate(cam.right, -sensitivity * 10 * 3.14159 / 180.0);
            }
            
            if (jDown) {
                cam.rotate(vector3(0, 1, 0), sensitivity * 10 * 3.14159 / 180.0);
            }
            
            if (lDown) {
                cam.rotate(vector3(0, 1, 0), -sensitivity * 10 * 3.14159 / 180.0);
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
            
            window.clear();
            
            circle* _circ = nullptr;
            rectangle* _rect = nullptr;
            convexshape* _convexshape = nullptr;
            
            
            
            vector<object2D*> shapes = cam.get_view(verticies);
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
            window.display();
        }
        
    }

    return EXIT_SUCCESS;
}
