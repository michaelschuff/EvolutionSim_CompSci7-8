#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <iomanip>
#include "vector3.hpp"
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include "bubbles.hpp"
//#include "ResourcePath.hpp"

using namespace sf;
using namespace std;

int main()
{
    RenderWindow window(VideoMode(1020, 600), "Bubble Visualization");

    bubble FirstBubble(vector3(500, 0, 500), vector3(-55, -55, 55), vector3(-55, -55, 55));

    //Make visualization of bubbles
    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
        }
        FirstBubble.updateBubble();
        window.clear();
        window.draw(FirstBubble.myShape);

        window.display();
    }
    return 0;
}
