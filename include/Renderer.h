#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "ObjectLoader.h"
#include "MathUtils.h"

class Renderer {
public:
    Renderer(unsigned int width, unsigned int height);

    void run();
private:
    void processEvents();
    void render();
    sf::Image framebuffer;
    sf::RenderWindow window;
    

    std::vector<Triangle> model;
};

#endif // RENDERER_H