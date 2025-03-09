#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Renderer {
public:
    Renderer(unsigned int width, unsigned int height);
    void run();
private:
    void processEvents();
    void render();

    sf::RenderWindow window;
};

#endif // RENDERER_H