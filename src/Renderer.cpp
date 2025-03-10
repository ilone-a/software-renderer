#include "../include/Renderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include "../include/Light.h"

using namespace sf;

Renderer::Renderer(unsigned int width, unsigned int height)
	: window(sf::VideoMode(Vector2u{ width, height }), "Software Renderer") {
	model = ObjLoader::Load("./models/teapot.obj"); // TODO remove
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

//Move the model to window center
sf::Vector2f Renderer::calculateOffset() {
	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::min();

	for (const Triangle& triangle : model) {
		minX = std::min({ minX, triangle.v0.x, triangle.v1.x, triangle.v2.x });
		maxX = std::max({ maxX, triangle.v0.x, triangle.v1.x, triangle.v2.x });
		minY = std::min({ minY, triangle.v0.y, triangle.v1.y, triangle.v2.y });
		maxY = std::max({ maxY, triangle.v0.y, triangle.v1.y, triangle.v2.y });
	}

	float centerX = (minX + maxX) / 2.0f;
	float centerY = (minY + maxY) / 2.0f;

	// Calculate offset
	float offsetX = window.getSize().x / 2.0f - centerX;
	float offsetY = window.getSize().y / 2.0f - centerY;

	return sf::Vector2f(offsetX, offsetY);
}

// Scale the model to 0.5 window space
void Renderer::scaleModel(std::vector<Triangle>& model) {
	// Get model scale
	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::min();

	for (const Triangle& triangle : model) {
		minX = std::min({ minX, triangle.v0.x, triangle.v1.x, triangle.v2.x });
		maxX = std::max({ maxX, triangle.v0.x, triangle.v1.x, triangle.v2.x });
		minY = std::min({ minY, triangle.v0.y, triangle.v1.y, triangle.v2.y });
		maxY = std::max({ maxY, triangle.v0.y, triangle.v1.y, triangle.v2.y });
	}

	// Get minmax
	float modelWidth = maxX - minX;
	float modelHeight = maxY - minY;

	// Choose max half x or half y
	float scale = std::min(window.getSize().x, window.getSize().y) / 2.0f;
	scale /= std::max(modelWidth, modelHeight);

	// Scale model
	for (Triangle& triangle : model) {
		triangle.v0.x = (triangle.v0.x - minX) * scale;
		triangle.v0.y = (triangle.v0.y - minY) * scale;
		triangle.v1.x = (triangle.v1.x - minX) * scale;
		triangle.v1.y = (triangle.v1.y - minY) * scale;
		triangle.v2.x = (triangle.v2.x - minX) * scale;
		triangle.v2.y = (triangle.v2.y - minY) * scale;
	}
}



void Renderer::render() {
	window.clear(sf::Color::Black);

	scaleModel(model);

	//Light light(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 4.0f), 1.0f);

	sf::Vector2f offset = calculateOffset();


	//Draw all triangles
	for (const Triangle& triangle : model) {
		sf::VertexArray shape(sf::PrimitiveType::Triangles, 3);

		shape[0].position = sf::Vector2f(triangle.v0.x + offset.x, triangle.v0.y + offset.y);
		shape[1].position = sf::Vector2f(triangle.v1.x + offset.x, triangle.v1.y + offset.y);
		shape[2].position = sf::Vector2f(triangle.v2.x + offset.x, triangle.v2.y + offset.y);


		shape[0].color = sf::Color::White;
		shape[1].color = sf::Color::White;
		shape[2].color = sf::Color::White;

		window.draw(shape);
	}
	window.display();
}
