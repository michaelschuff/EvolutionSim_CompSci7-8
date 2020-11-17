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

int main()
{
    fish bob(vector3(0, 0, 0), vector3(1, 1, 1), 60);

    agent bobbette = bob::agent;

    return 0;
}
