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
//#include "ResourcePath.hpp"

using namespace sf;
using namespace std;



int main(int, char const**) {
    const int width = 800, height = 800;
//    vector3 p1(-0.5, -0.5, -0.5),
//            p2( 0.5, -0.5, -0.5),
//            p3(-0.5,  0.5, -0.5),
//            p4(-0.5, -0.5,  0.5),
//            p5(-0.5,  0.5,  0.5),
//            p6( 0.5, -0.5,  0.5),
//            p7( 0.5,  0.5, -0.5),
//            p8( 0.5,  0.5,  0.5);
    vector3 p1(0,0,0),
            p2(1,0,0),
            p3(0,1,0),
            p4(0,0,1),
            p5(0,1,1),
            p6(1,0,1),
            p7(1,1,0),
            p8(1,1,1);
    vector<object*> verticies = {
        &p1,
        &p2,
        &p3,
        &p4,
        &p5,
        &p6,
        &p7,
        &p8,
        new line(vector3(), vector3(1, 0, 0), color(255, 0, 0)),
        new line(vector3(), vector3(0, 1, 0), color(0, 255, 0)),
        new line(vector3(), vector3(0, 0, 1), color(0, 0, 255)),
        new line(p1, p2),
        new line(p1, p3),
        new line(p1, p4),
        new line(p2, p6),
        new line(p2, p7),
        new line(p3, p5),
        new line(p3, p7),
        new line(p4, p5),
        new line(p4, p6),
        new line(p8, p5),
        new line(p8, p6),
        new line(p8, p7),
        new plane(vector3(0, -1, 0), vector3(0, 1, 0), vector3(0, 0, 1), 5, 5, color(0.7, 0.7, 0.7, 1.0)),
    };
    
    double sensitivity = 0.1;
    RenderWindow window(VideoMode(width, height), "SFML window");
    camera cam(vector3(5, 0, 0), vector3(-1, 0, 0), vector3(0, 1, 0), vector3(0, 0, 1));
    window.setFramerateLimit(60);
    bool permissions_granted = false;
    
    
    bool qDown = false, wDown = false, eDown = false, aDown = false,sDown = false, dDown = false;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
            
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Q) {
                qDown = true;
            }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::Q) {
                qDown = false;
            }
            
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::W) {
                wDown = true;
            }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::W) {
                wDown = false;
            }
            
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::E) {
                eDown = true;
            }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::E) {
                eDown = false;
            }
            
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::A) {
                aDown = true;
            }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::A) {
                aDown = false;
            }
            
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::S) {
                sDown = true;
            }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::S) {
                sDown = false;
            }
            
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::D) {
                dDown = true;
            }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::D) {
                dDown = false;
            }
        }
        
        Vector2i mouse_position = Mouse::getPosition(window);
        if (mouse_position.x != width / 2) {
            cam.rotate(vector3(0, 1, 0), sensitivity * -(mouse_position.x - width/2) * M_PI / 180.0);
        }
        if (mouse_position.y != height / 2 && cam.up.rotated(cam.right, sensitivity * (mouse_position.y - height/2) * M_PI / 180.0).y > 0) {
            cam.rotate(cam.right, sensitivity * (mouse_position.y - height/2) * M_PI / 180.0);
        }
        cam.up.print();
        Mouse::setPosition(Vector2i(width/2, height/2), window);
        
        if (wDown) {
            cam.position += cam.forward / 60.0;
        }
        if (sDown) {
            cam.position -= cam.forward / 60.0;
        }
        
        if (aDown) {
            cam.position += cam.right / 60.0;
        }
        if (dDown) {
            cam.position -= cam.right / 60.0;
        }
        
        if (eDown) {
            cam.position += cam.up / 60.0;
        }
        if (qDown) {
            cam.position -= cam.up / 60.0;
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
                c.setPosition(width/2 + width/4*_circ->x, height/2 - height/4*_circ->y);
                c.setFillColor(_circ->c);

                window.draw(c);
            } else if ((_rect = dynamic_cast<rectangle*>(shapes[i]))) {
                RectangleShape r(Vector2f(width/4 * _rect->width, _rect->height));
                r.setOrigin(0.5, 0.5);
                r.setPosition(width/2 + width/4*_rect->x, height/2 - height/4*_rect->y);
                r.rotate(-_rect->theta * 180.0 / M_PI);
                r.setFillColor(_rect->c);

                window.draw(r);
            } else if ((_convexshape = dynamic_cast<convexshape*>(shapes[i]))) {
                ConvexShape c(_convexshape->points.size());
                for (int j = 0; j < _convexshape->points.size(); j++) {
                    c.setPoint(j, Vector2f(width/2 + width/4*(_convexshape->points[j][0]), height/2 - height/4*(_convexshape->points[j][1])));
                }
                c.setFillColor(_convexshape->c);
                window.draw(c);
            }
        }
        window.display();
    }

    return EXIT_SUCCESS;
}
