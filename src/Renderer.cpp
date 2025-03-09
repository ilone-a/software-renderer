#include "../include/Renderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>

using namespace sf;

Renderer::Renderer(unsigned int width, unsigned int height)
    : window(sf::VideoMode( Vector2u { width, height }), "Software Renderer") {}

void Renderer::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Renderer::processEvents() {
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}

void Renderer::render() {
    window.clear(sf::Color::Black);
    // Render here
    window.display();
}
