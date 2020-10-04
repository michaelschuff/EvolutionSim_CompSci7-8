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
//#include "ResourcePath.hpp"

using namespace sf;
using namespace std;



int main(int, char const**) {
    const int width = 800, height = 800;
    
    vector3 p1(-0.5, -0.5, -0.5),//000
            p2( 0.5, -0.5, -0.5),//100
            p3(-0.5,  0.5, -0.5),//010
            p4(-0.5, -0.5,  0.5),//001
            p5(-0.5,  0.5,  0.5),//011
            p6( 0.5, -0.5,  0.5),//101
            p7( 0.5,  0.5, -0.5),//110
            p8( 0.5,  0.5,  0.5);//111
    vector<object*> verticies = {
        &p1,
        &p2,
        &p3,
        &p4,
        &p5,
        &p6,
        &p7,
        &p8,
        new line(vector3(), vector3(1.5, 0, 0), color(255, 0, 0)),
        new line(vector3(), vector3(0, 1.5, 0), color(0, 255, 0)),
        new line(vector3(), vector3(0, 0, 1.5), color(0, 0, 255)),
        
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
        new plane(vector3(0, 0, -0.5), vector3(0, 0, 1), 2, 2)
    };
    
    double counter = 0;
//    camera cam(vector3(5, 5, 1), width, height);
    camera cam(vector3(5, 1, 5), width, height);
//    camera cam(vector3(1, 5, 5), width, height);
    
    RenderWindow window(VideoMode(width, height), "SFML window");

    window.setFramerateLimit(60);
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
        }

        window.clear();
//        cam.position.rotate(vector3(0, 0, 1), 0.5 * M_PI / 180.0);
        cam.position.rotate(vector3(0, 1, 0), 0.5 * M_PI / 180.0);
//        cam.position.rotate(vector3(1, 0, 0), 0.5 * M_PI / 180.0);
        cam.look_at(vector3(0, 0, 0));
        
        circle* _circ = nullptr;
        rectangle* _rect = nullptr;
        convexshape* _convexshape = nullptr;
        
        vector<object2D*> shapes = cam.get_screen(verticies);
        for (int i = 0; i < shapes.size(); i++) {
            if ((_circ = dynamic_cast<circle*>(shapes[i]))) {
                CircleShape c(_circ->r);
                c.setOrigin(_circ->r, _circ->r);
                c.setPosition(_circ->x, _circ->y);
                c.setFillColor(_circ->c);

                window.draw(c);
            }
            else if ((_rect = dynamic_cast<rectangle*>(shapes[i]))) {
                RectangleShape r(Vector2f(_rect->width, _rect->height));
                r.setOrigin(0.5, 0.5);
                r.setPosition(_rect->x, _rect->y);
                r.rotate(_rect->theta * 180.0 / M_PI);
                r.setFillColor(_rect->c);

                window.draw(r);
            } else if ((_convexshape = dynamic_cast<convexshape*>(shapes[i]))) {
                ConvexShape c(_convexshape->points.size());
                for (int j = 0; j < _convexshape->points.size(); j++) {
                    c.setPoint(j, Vector2f(400+200*(_convexshape->points[j][0]), 400+200*(_convexshape->points[j][1])));
                }
                c.setFillColor(_convexshape->c);
                window.draw(c);
            }
        }
        window.display();
    }

    return EXIT_SUCCESS;
}
