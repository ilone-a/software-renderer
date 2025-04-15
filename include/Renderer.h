#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <algorithm>
#include "ObjectLoader.h"
#include "MathUtils.h"
#include "Profiler.h"
#include "Light.h"
#include <chrono>
using namespace std::chrono;

using namespace sf;
using namespace MathUtils;



class Renderer {
public:
	Renderer(unsigned int width, unsigned int height);
	VertexArray pixels;
	int width; int height;
	std::vector<Triangle> model;//opened file to render
	void run();

private:


	sf::RenderWindow window;

	std::vector<float> stencilBuffer;//float cause stencil is used as Zbuf

	float interpolationFactor;
	Mat4 targetTransform;
	Mat4 transform;
	Mat4 generateRandomTransform();
	//Main
	void processEvents();
	void render();
	//void update();
	// 
	//Rasterize
	void drawTriangleScanline(const Triangle& triangle);

	//Renderer Settings
	sf::Vector2f calculateOffset();
	//void scaleModel(std::vector<Triangle>& model);
	//void moveTriangle(Triangle& triangle, Vector2f& offset);

	//Renderer Debug
	void drawDebugEdges(const Triangle& triangle);




};
