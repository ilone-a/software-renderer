#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <algorithm>
#include <chrono>
#include <iostream>
#include "../include/Light.h"
#include "../include/Renderer.h"
#include "../include/MathUtils.h"
#include "VertexShader.cpp"

using namespace sf;
using namespace MathUtils;
using namespace std::chrono;


Light g_Light(sf::Vector2f(400.f, 300.f), 5000.f);
#define PROFILE_SCOPE(name) Profiler profiler##__LINE__(name);
#define USE_IS_POINT_IN_TRIANGLE 1

// Main renderer class
Renderer::Renderer(unsigned int width, unsigned int height)
	: window(sf::VideoMode(Vector2u{ width, height }), "Software Renderer"),
	width(width),
	height(height),
	pixels(sf::PrimitiveType::Points, 0),
	stencilBuffer(width* height, std::numeric_limits<float>::infinity())
{
	model = ObjLoader::Load("./models/teapot.obj");
}

// Main loop entry point
void Renderer::run() {
	while (window.isOpen()) {
		PROFILE_SCOPE("Frame");
		processEvents();
		//update();
		render();
	}
}

// Handles user input and window events
void Renderer::processEvents() {
	while (const std::optional<sf::Event> event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>())
			window.close();
	}
}

//// Update scene logic (currently empty)
//void Renderer::update() {}

// Generates a random transform matrix
Mat4 Renderer::generateRandomTransform() {
	return Mat4::identity();
}

//Point-in-triangle using barycentric coordinates
static bool isPointInTriangle(const MathUtils::Vec3& p, const MathUtils::Vec3& v0, const MathUtils::Vec3& v1, const MathUtils::Vec3& v2) {
	float denom = (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);
	if (std::abs(denom) < 1e-5f) return false;
	float a = ((v1.y - v2.y) * (p.x - v2.x) + (v2.x - v1.x) * (p.y - v2.y)) / denom;
	float b = ((v2.y - v0.y) * (p.x - v2.x) + (v0.x - v2.x) * (p.y - v2.y)) / denom;
	float c = 1.0f - a - b;
	return a >= -0.01f && b >= -0.01f && c >= -0.01f;
}

// Draws a filled triangle using a scanline-style loop and point-in-triangle test
void Renderer::drawTriangleScanline(const MathUtils::Triangle& triangle) {
	extern Light g_Light;

	MathUtils::Vec3 v0 = triangle.v0;
	MathUtils::Vec3 v1 = triangle.v1;
	MathUtils::Vec3 v2 = triangle.v2;

	float minX = std::min({ v0.x, v1.x, v2.x });
	float maxX = std::max({ v0.x, v1.x, v2.x });
	float minY = std::min({ v0.y, v1.y, v2.y });
	float maxY = std::max({ v0.y, v1.y, v2.y });

	minX = std::clamp(minX, 0.0f, float(width - 1));
	maxX = std::clamp(maxX, 0.0f, float(width - 1));
	minY = std::clamp(minY, 0.0f, float(height - 1));
	maxY = std::clamp(maxY, 0.0f, float(height - 1));

	float area = (v1.x - v0.x) * (v2.y - v0.y) - (v2.x - v0.x) * (v1.y - v0.y);
	if (std::abs(area) < 1e-5f) {
		std::cout << "[Scanline] Skipped degenerate triangle\n";
		return;
	}

	for (int y = int(minY); y <= int(maxY); ++y) {
		for (int x = int(minX); x <= int(maxX); ++x) {
			MathUtils::Vec3 p{ float(x) + 0.5f, float(y) + 0.5f, 0.0f };
#if USE_IS_POINT_IN_TRIANGLE
			if (MathUtils::isPointInTriangle(p, v0, v1, v2)) {
				float z = (v0.z + v1.z + v2.z) /20.0f;
				float minZ = -1.0f, maxZ = 1.0f;
				//int gray = static_cast<int>(512 * (1.0f - (z - minZ) / (maxZ - minZ)));
				//gray = std::clamp(gray, 0, 255);
				//pixels.append(Vertex{sf::Vector2f(p.x, p.y), sf::Color(gray, gray, gray)			});
				// 
				// 
				// calculate lighting at current point
				sf::Vector2f screenPos{ p.x, p.y };
				float light = g_Light.getLighting(screenPos);
				light = std::clamp(light, 0.0f, 1.0f);
				int brightness = static_cast<int>(255 * light);
				pixels.append(Vertex{ screenPos, sf::Color(brightness, brightness, brightness) });
			}
#else
			float w0 = (v1.x - v0.x) * (p.y - v0.y) - (v1.y - v0.y) * (p.x - v0.x);
			float w1 = (v2.x - v1.x) * (p.y - v1.y) - (v2.y - v1.y) * (p.x - v1.x);
			float w2 = (v0.x - v2.x) * (p.y - v2.y) - (v0.y - v2.y) * (p.x - v2.x);
			if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
				pixels.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Color::Green));
			}
#endif
		}
	}
}

// Main render pass: clear, transform mesh, rasterize triangles, display result
void Renderer::render() {
	std::cout << "[Render] Starting frame\n";
	window.clear(sf::Color::Black);
	pixels.clear();
	stencilBuffer.assign(width * height, 1.0f);

	MathUtils::Matrix m2s(MathUtils::kEmpty);
	SoftRender::VertexShader vs(width, height);
	vs.modelToScreen(m2s);

	Light g_Light(sf::Vector2f(width / 2.0f, height / 2.0f), 5000.0f); // move it
	std::cout << "[Render] Model-to-Screen Matrix:\n";

	std::vector<Triangle> transformedModel;

	transformedModel = model;

	for (Triangle& triangle : transformedModel) {
		SoftRender::RasterizerVertex<MathUtils::Vector4> verts[3];
		verts[0].location = { triangle.v0.x, triangle.v0.y, triangle.v0.z, 1.0f };
		verts[1].location = { triangle.v1.x, triangle.v1.y, triangle.v1.z, 1.0f };
		verts[2].location = { triangle.v2.x, triangle.v2.y, triangle.v2.z, 1.0f };

		vs.transform_triangle(verts, m2s);

		for (int i = 0; i < 3; ++i) {
			float w = verts[i].location.w;
			verts[i].location.x /= w;
			verts[i].location.y /= w;
			verts[i].location.z /= w;
		}

		triangle.v0 = { verts[0].location.x, verts[0].location.y, verts[0].location.z };
		triangle.v1 = { verts[1].location.x, verts[1].location.y, verts[1].location.z };
		triangle.v2 = { verts[2].location.x, verts[2].location.y, verts[2].location.z };
	};

	for (const Triangle& triangle : transformedModel) {
		drawTriangleScanline(triangle);
	}

	std::cout << "[Render] Total vertices: " << pixels.getVertexCount() << "\n";
	window.draw(pixels);
	window.display();
}

//TODO: mark as debug function
// Draws white lines along triangle edges
void Renderer::drawDebugEdges(const Triangle& triangle) {
	// Directly draw the edges of the triangle by connecting the vertices
	auto drawLine = [&](Vec3 vStart, Vec3 vEnd) {
		// Make sure we are in the window's bounds
		if (vStart.x < 0 || vStart.x >= window.getSize().x || vEnd.x < 0 || vEnd.x >= window.getSize().x ||
			vStart.y < 0 || vStart.y >= window.getSize().y || vEnd.y < 0 || vEnd.y >= window.getSize().y) {
			return; // Don't draw lines out of the window bounds
		}
		// Interpolate points to draw the line between them (Bresenham or simple interpolation)
		float dx = vEnd.x - vStart.x;
		float dy = vEnd.y - vStart.y;
		int steps = std::max(abs(dx), abs(dy));  // How many steps to interpolate
		float xInc = dx / static_cast<float>(steps);
		float yInc = dy / static_cast<float>(steps);

		float x = vStart.x;
		float y = vStart.y;

		for (int i = 0; i <= steps; ++i) {
			if (x >= 0 && x < window.getSize().x && y >= 0 && y < window.getSize().y) {
				pixels.append(Vertex{ sf::Vector2f(static_cast<float>(x), static_cast<float>(y)), sf::Color::White });
			}
			x += xInc;
			y += yInc;
		}
		};

	// Draw edges between vertices
	drawLine(triangle.v0, triangle.v1);  // v0 -> v1
	drawLine(triangle.v1, triangle.v2);  // v1 -> v2
	drawLine(triangle.v2, triangle.v0);  // v2 -> v0
}

