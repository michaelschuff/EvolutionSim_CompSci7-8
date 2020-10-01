#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "vector3.hpp"
//#include "ResourcePath.hpp"

using namespace sf;
using namespace std;

int main(int, char const**) {
    RenderWindow window(VideoMode(800, 600), "SFML window");


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

        window.display();
    }

    return EXIT_SUCCESS;
}
