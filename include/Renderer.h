#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "ObjectLoader.h"
#include "MathUtils.h"
using namespace sf;
class Renderer {
public:
	Renderer(unsigned int width, unsigned int height);
	VertexArray pixels;
	Renderer() : pixels(PrimitiveType::Points, 0) {};
	void run();
private:
	void processEvents();
	void render();
	sf::Image framebuffer;
	sf::RenderWindow window;
	sf::Vector2f calculateOffset();
	void scaleModel(std::vector<Triangle>& model);
	void drawTriangleScanline(const Triangle& triangle);
	void drawTriangleEdges(const Triangle& triangle);
	std::vector<Triangle> model;
};
