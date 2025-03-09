#include "../include/Renderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
/*
using namespace sf;

Renderer::Renderer(int width, int height)
    : window(sf::VideoMode(width, height), "Software Renderer") {}

void Renderer::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Renderer::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Renderer::render() {
    window.clear(sf::Color::Black);
    // Тут будет рендеринг
    window.display();
}
*/