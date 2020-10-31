//
//  SliderSFML.hpp
//  CompSci78
//
//  Created by Michael Schuff on 10/30/20.
//  Copyright © 2020 Michael Schuff. All rights reserved.
//

#ifndef SliderSFML_hpp
#define SliderSFML_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class SliderSFML
{
    sf::RectangleShape slider;
    sf::RectangleShape axis;
//    sf::Font font;
//    sf::Text text;
    int minValue;
    int maxValue;
    int xCord;
    int yCord;
    int axisWidth;
    int axisHeight;
    int sliderWidth;
    int sliderHeight;
    float sliderValue;
    bool held;
public:
    SliderSFML(int x, int y);
//    sf::Text returnText(int x, int y, std::string z, int fontSize);
    void create(int min, int max);
    void logic(sf::RenderWindow &window);
    float getSliderValue();
    void setSliderValue(float newValue);
    void setSliderPercentValue(float newPercentValue);
    void draw(sf::RenderWindow & window);
};

SliderSFML::SliderSFML(int x, int y) {
    xCord = x;
    yCord = y;
    axisHeight = 10;
    axisWidth = 200;
    sliderWidth = 20;
    sliderHeight = 30;

    axis.setPosition(x, y);
    axis.setOrigin(0, axisHeight / 2);
    axis.setSize(sf::Vector2f(axisWidth, axisHeight));
    axis.setFillColor(sf::Color(63,63,63));
    slider.setPosition(x, y);
    slider.setOrigin(sliderWidth / 2, sliderHeight / 2);
    slider.setSize(sf::Vector2f(sliderWidth, sliderHeight));
    slider.setFillColor(sf::Color(192,192,192));
}

void SliderSFML::create(int min, int max)
{
    sliderValue = min;
    minValue = min;
    maxValue = max;
}

void SliderSFML::logic(sf::RenderWindow &window)
{
    if (slider.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)
        && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        held = true;
        
    }
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        held = false;
    }
    if (held) {
        float mx = sf::Mouse::getPosition(window).x;
        float my = sf::Mouse::getPosition(window).y;
        if (mx > xCord + axisWidth) {
            slider.setPosition(xCord + axisWidth, yCord);
        } else if (mx <= xCord) {
            slider.setPosition(xCord, yCord);
        } else {
            slider.setPosition(mx, yCord);
        }
        sliderValue = (minValue + ((slider.getPosition().x - xCord) / axisWidth * (maxValue - minValue)));
    }
}

float SliderSFML::getSliderValue()
{
    return sliderValue;
}

void SliderSFML::setSliderValue(float newValue)
{
    if (newValue >= minValue && newValue <= maxValue)
    {
        sliderValue = newValue;
        float diff = maxValue - minValue;
        float diff2 = newValue - minValue;
        float zzz = axisWidth / diff;
        float posX = zzz*diff2;
        posX += xCord;
        slider.setPosition(posX, yCord);
    }
}

void SliderSFML::setSliderPercentValue(float newPercentValue)
{
    if (newPercentValue >= 0 && newPercentValue <= 100)
    {
        sliderValue = newPercentValue / 100 * maxValue;
        slider.setPosition(xCord + (axisWidth*newPercentValue / 100), yCord);
    }
}

void SliderSFML::draw(sf::RenderWindow &window)
{
    logic(window);
    window.draw(axis);
    window.draw(slider);
}
#endif /* SliderSFML_hpp */

