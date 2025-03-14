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
	int width; int height;
	Renderer() : pixels(PrimitiveType::Points, 0) {};
	void run();
private:
	std::vector<int> stencilBuffer;
	void processEvents();
	void render();
	sf::Image framebuffer;
	sf::RenderWindow window;
	sf::Vector2f calculateOffset();
	void scaleModel(std::vector<Triangle>& model);
	void drawTriangleScanline(const Triangle& triangle);
	void drawTriangleScanline2(const Triangle& triangle);

	void drawDebugScanline(const Triangle& triangle);
	void drawTriangleEdges(const Triangle& triangle);
	std::vector<Triangle> model;
	sf::FloatRect getBoundingBox(const std::vector<Triangle>& model);
	int getStencilIndex(int x, int y) const {
		return y * width + x;  // Индекс для доступа к пикселю в буфере стенсила
	}

	void setStencilValue(int x, int y, int value) {
		int index = getStencilIndex(x, y);
		if (index >= 0 && index < stencilBuffer.size()) {
			stencilBuffer[index] = value;  // Устанавливаем значение стенсила для пикселя
		}
	}
	void updateStencilBuffer();
	int getStencilValue(int x, int y) const {
		int index = getStencilIndex(x, y);
		if (index >= 0 && index < stencilBuffer.size()) {
			return stencilBuffer[index];  // Возвращаем значение стенсила для пикселя
		}
		else return 0;  // Если индекс вне границ, возвращаем 0
	}
};
