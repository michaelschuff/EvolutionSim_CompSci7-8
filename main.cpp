#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include "vector3.hpp"
#include "triangle.hpp"
#include "mesh.hpp"
#include "camera.hpp"
//#include "ResourcePath.hpp"

using namespace sf;
using namespace std;


RectangleShape Line(vector3, vector3);

int main(int, char const**) {
    const int width = 800, height = 800;
    mesh cube({
        // Front
        triangle(vector3(0, 0, 0), vector3(0, 1, 0), vector3(1, 1, 0)),
        triangle(vector3(0, 0, 0), vector3(1, 1, 0), vector3(1, 0, 0)),
        
        // Right
        triangle(vector3(1, 0, 0), vector3(1, 1, 0), vector3(1, 1, 1)),
        triangle(vector3(1, 0, 0), vector3(1, 1, 1), vector3(1, 0, 1)),
        
        // Back
        triangle(vector3(1, 0, 1), vector3(1, 1, 1), vector3(0, 1, 1)),
        triangle(vector3(1, 0, 1), vector3(0, 1, 1), vector3(0, 0, 1)),
        
        // Left
        triangle(vector3(0, 0, 1), vector3(0, 1, 1), vector3(0, 1, 0)),
        triangle(vector3(0, 0, 1), vector3(0, 1, 0), vector3(0, 0, 0)),
        
        // Top
        triangle(vector3(0, 1, 0), vector3(0, 1, 1), vector3(1, 1, 1)),
        triangle(vector3(0, 1, 0), vector3(1, 1, 1), vector3(1, 1, 0)),
        
        // Bottom
        triangle(vector3(1, 0, 1), vector3(0, 0, 1), vector3(0, 0, 0)),
        triangle(vector3(1, 0, 1), vector3(0, 0, 0), vector3(1, 0, 0))
    });
    
    vector<vector3> verticies = {
        vector3(-0.5, -0.5, -0.5),
        vector3( 0.5, -0.5, -0.5),
        vector3(-0.5,  0.5, -0.5),
        vector3(-0.5, -0.5,  0.5),
        vector3(-0.5,  0.5,  0.5),
        vector3( 0.5, -0.5,  0.5),
        vector3( 0.5,  0.5, -0.5),
        vector3( 0.5,  0.5,  0.5),
    };
    
    camera c(vector3(0, 0, 5), width, height, 0, 0, 0);
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
        c.position.rotate(vector3(0, 1, 1), .5 * M_PI / 180.0);
        
        c.look_at(vector3(0, 0, 0));
        vector<CircleShape> circles = c.get_screen(verticies);
        for (int i = 0; i < circles.size(); i++) {
            window.draw(circles[i]);
        }
        window.display();
    }

    return EXIT_SUCCESS;
}

RectangleShape Line(vector3 v1, vector3 v2) {
    v2.x = v2.x*50+400;
    v2.z = v2.z*50+400;
    v1.x = v1.x*50;
    v1.z = v1.z*50;
    RectangleShape line(Vector2f(sqrt(v1.x*v1.x+v1.z*v1.z),1));
    line.setOrigin(0.5, 0.5);
    line.rotate((atan2(v1.z,v1.x)*180.0/M_PI));
    line.setPosition(v2.x, v2.z);
    line.setFillColor(Color(150, 150, 150));
    return line;
}
