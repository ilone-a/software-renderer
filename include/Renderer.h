#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <algorithm>
#include "ObjectLoader.h"
#include "MathUtils.h"
#include "Stencil.h"

using namespace sf;
class Renderer {
public:
	Renderer(unsigned int width, unsigned int height);
	VertexArray pixels;
	int width; int height;

	void run();
private:
	sf::RenderWindow window;
	std::vector<Triangle> model;//opened file to render
	std::vector<int> stencilBuffer;


	//Main
	void processEvents();
	void render();

	//Rasterize
	void drawTriangleScanline(const Triangle& triangle);

	//Renderer Settings
	sf::Vector2f calculateOffset();
	void scaleModel(std::vector<Triangle>& model);
	void moveTriangle(Triangle& triangle, Vector2f& offset);

	//Renderer Debug
	void drawDebugEdges(const Triangle& triangle);




};
