#include "../include/Renderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>

using namespace sf;

Renderer::Renderer(unsigned int width, unsigned int height)
    : window(sf::VideoMode( Vector2u { width, height }), "Software Renderer") {
    model = ObjLoader::Load("F:/projects2/SoftwareRenderer/models/teapot.obj"); // 
}

void Renderer::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Renderer::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Renderer::render() {
    window.clear(sf::Color::Black);
    
    sf::VertexArray triangle(sf::PrimitiveType::Triangles, 3);

    triangle[0].position = sf::Vector2f(100.f, 100.f);
    triangle[0].color = sf::Color::Red;

    triangle[1].position = sf::Vector2f(200.f, 100.f);
    triangle[1].color = sf::Color::Green;

    triangle[2].position = sf::Vector2f(150.f, 200.f);
    triangle[2].color = sf::Color::Blue;

    window.draw(triangle);
    window.display();
}
