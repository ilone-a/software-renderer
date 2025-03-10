#pragma once

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
	sf::Vector2f calculateOffset();
	void scaleModel(std::vector<Triangle>& model);

	std::vector<Triangle> model;
};
