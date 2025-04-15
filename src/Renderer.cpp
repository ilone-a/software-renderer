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
#define PROFILE_SCOPE(name) Profiler profiler##__LINE__(name);
#define USE_IS_POINT_IN_TRIANGLE 1

Renderer::Renderer(unsigned int width, unsigned int height)
	: window(sf::VideoMode(Vector2u{ width, height }), "Software Renderer"),
	width(width),
	height(height),
	pixels(sf::PrimitiveType::Points, 0),
	stencilBuffer(width* height, std::numeric_limits<float>::infinity())
{
	model = ObjLoader::Load("./models/teapot.obj");
}

void Renderer::run() {
	while (window.isOpen()) {
		PROFILE_SCOPE("Frame");
		processEvents();
		update();
		render();
	}
}

void Renderer::processEvents() {
	while (const std::optional<sf::Event> event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>())
			window.close();
	}
}

void Renderer::update() {}

Mat4 Renderer::generateRandomTransform() {
	return Mat4::identity();
}

static bool isPointInTriangle(const MathUtils::Vec3& p, const MathUtils::Vec3& v0, const MathUtils::Vec3& v1, const MathUtils::Vec3& v2) {
	float denom = (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);
	if (std::abs(denom) < 1e-5f) return false;
	float a = ((v1.y - v2.y) * (p.x - v2.x) + (v2.x - v1.x) * (p.y - v2.y)) / denom;
	float b = ((v2.y - v0.y) * (p.x - v2.x) + (v0.x - v2.x) * (p.y - v2.y)) / denom;
	float c = 1.0f - a - b;
	return a >= -0.01f && b >= -0.01f && c >= -0.01f;
}

void Renderer::drawTriangleScanline(const MathUtils::Triangle& triangle) {
	std::cout << "[Scanline] Drawing triangle\n";
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
				pixels.append(Vertex{ sf::Vector2f(p.x, p.y), sf::Color::Green });
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
void Renderer::render() {
	std::cout << "[Render] Starting frame\n";
	window.clear(sf::Color::Black);
	pixels.clear();
	stencilBuffer.assign(width * height, 1.0f);

	SoftRender::Matrix m2s(SoftRender::kTrivial);
	SoftRender::VertexShader vs(width, height);
	vs.modelToScreen(m2s);


	std::cout << "[Render] Model-to-Screen Matrix:\n";
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << m2s.rc[i][j] << "\t";
		}
		std::cout << "\n";
	}

	std::vector<Triangle> transformedModel;
//	transformedModel.push_back({
	//	{-0.5f, -0.5f, 1.0f},
	//	{ 0.5f, -0.5f, 1.0f},
	//	{ 0.0f,  0.5f, 1.0f}
	//	});

	//for (Triangle& triangle : transformedModel) {
	//	SoftRender::RasterizerVertex<SoftRender::Vector4> verts[3];
	//	verts[0].location = { triangle.v0.x, triangle.v0.y, triangle.v0.z, 1.0f };
		// Load the teapot model into transformedModel
	transformedModel = model;

	for (Triangle& triangle : transformedModel) {
		SoftRender::RasterizerVertex<SoftRender::Vector4> verts[3];
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
		//verts[1].location = { triangle.v1.x, triangle.v1.y, triangle.v1.z, 1.0f };
		//verts[2].location = { triangle.v2.x, triangle.v2.y, triangle.v2.z, 1.0f };

	//	std::cout << "[Transform] Before: "
		//	<< triangle.v0.x << "," << triangle.v0.y << " | "
		//	<< triangle.v1.x << "," << triangle.v1.y << " | "
		//	<< triangle.v2.x << "," << triangle.v2.y << "\n";

		//vs.transform_triangle(verts, m2s);
/*
		for (int i = 0; i < 3; ++i) {
			float w = verts[i].location.w;
			verts[i].location.x /= w;
			verts[i].location.y /= w;
			verts[i].location.z /= w;
		}

		triangle.v0 = { verts[0].location.x, verts[0].location.y, verts[0].location.z };
		triangle.v1 = { verts[1].location.x, verts[1].location.y, verts[1].location.z };
		triangle.v2 = { verts[2].location.x, verts[2].location.y, verts[2].location.z };
		
		std::cout << "[Transform] After: "
			<< triangle.v0.x << "," << triangle.v0.y << " | "
			<< triangle.v1.x << "," << triangle.v1.y << " | "
			<< triangle.v2.x << "," << triangle.v2.y << "\n";
	}*/

	for (const Triangle& triangle : transformedModel) {
		drawTriangleScanline(triangle);
	}

	std::cout << "[Render] Total vertices: " << pixels.getVertexCount() << "\n";
	window.draw(pixels);
	window.display();
}