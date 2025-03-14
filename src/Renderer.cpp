
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <algorithm>
#include "../include/Light.h"
#include "../include/Renderer.h"

using namespace sf;

//Init
Renderer::Renderer(unsigned int width, unsigned int height)
    : window(sf::VideoMode(Vector2u{ width, height }), "Software Renderer"),
    width(width), 
    height(height),
    pixels(PrimitiveType::Points, 0),

    stencilBuffer(width* height, 0) {  
    model = ObjLoader::Load("./models/teapot.obj"); // TODO remove
    //updateStencilBuffer(); // TODO change update point
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

// Renderer settings: offset
// Move the model to window center
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

// Renderer settings: scale
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




//TODO: mark as debug function
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



void Renderer::moveTriangle(Triangle& triangle, Vector2f& offset)
{
    triangle.v0.x += offset.x;
    triangle.v0.y += offset.y;
    triangle.v1.x += offset.x;
    triangle.v1.y += offset.y;
    triangle.v2.x += offset.x;
    triangle.v2.y += offset.y;
}

//Scanline method: sort points
void Renderer::drawTriangleScanline(const Triangle& triangle) {
    // Sort Y vertex
    auto sortVerticesY = [](Vec3& a, Vec3& b) {
        return a.y < b.y;
        };

    // Copy triangle vertices
    Vec3 v0 = triangle.v0;
    Vec3 v1 = triangle.v1;
    Vec3 v2 = triangle.v2;

    // Sort Y vertex (ascending)
    if (!sortVerticesY(v0, v1)) std::swap(v0, v1);
    if (!sortVerticesY(v1, v2)) std::swap(v1, v2);
    if (!sortVerticesY(v0, v1)) std::swap(v0, v1);

    // Interpolation function to calculate X
    //TODO fix it!
    auto interpolate = [](float y1, float y2, float x1, float x2, float y) {
        if (y1 == y2) return x1; // Avoid division by zero
        return x1 + (x2 - x1) * ((y - y1) / (y2 - y1));
        };

    // Check if is the point inside triangle (with barycentric coords)
    auto isPointInTriangle = [](const Vec3& p, const Vec3& v0, const Vec3& v1, const Vec3& v2) {
        // Barycentric
        float denom = (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);
        float a = ((v1.y - v2.y) * (p.x - v2.x) + (v2.x - v1.x) * (p.y - v2.y)) / denom;
        float b = ((v2.y - v0.y) * (p.x - v2.x) + (v0.x - v2.x) * (p.y - v2.y)) / denom;
        float c = 1 - a - b;

        return a >= 0 && b >= 0 && c >= 0;
        };

    // Function to draw scan line
    auto drawScanline = [&](int y, float x1, float x2) {
        // Check if x1 < x2
        if (x1 > x2) std::swap(x1, x2);

        // Crop x-coords to window size
        x1 = std::max(0.0f, std::min(window.getSize().x - 1.0f, x1));
        x2 = std::max(0.0f, std::min(window.getSize().x - 1.0f, x2));

        // Draw pixels between x1 and x2
        for (int x = static_cast<int>(std::ceil(x1)); x <= static_cast<int>(std::floor(x2)); ++x) {
            if (x >= 0 && x < window.getSize().x && y >= 0 && y < window.getSize().y) {
                // Check if pixel inside triangle
                if (isPointInTriangle(Vec3{ float(x), float(y), 0 }, v0, v1, v2)) {
                    pixels.append(Vertex{ sf::Vector2f(static_cast<float>(x), static_cast<float>(y)), sf::Color::White });
                }
            }
        }
        };

    // Top part: from v1 to v2
    for (int y = static_cast<int>(v0.y); y <= static_cast<int>(v1.y); ++y) {
        // Left bound (interp between v0 and v2)
        float xA = interpolate(v0.y, v2.y, v0.x, v2.x, y); // Left bound
        // Right bound (interp between  v1 and v2)
        float xB = interpolate(v0.y, v1.y, v0.x, v1.x, y); // Right bound
        drawScanline(y, xA, xB); // Draw scanline for this Y
    }

    // Bottom part: from v1 to v2
    for (int y = static_cast<int>(v1.y); y <= static_cast<int>(v2.y); ++y) {
        // Left bound (interp between v0 and v2)
        float xA = interpolate(v0.y, v2.y, v0.x, v2.x, y); // Left bound
        // Right bound (interp between  v1 and v2)
        float xB = interpolate(v1.y, v2.y, v1.x, v2.x, y); // Right bound
        drawScanline(y, xA, xB); // Draw scanline for this Y
    }
}
void Renderer::render() {
    window.clear(Color::Black);
    pixels.clear();

    scaleModel(model);
    Vector2f offset = calculateOffset();
    //updateStencilBuffer();
    for (Triangle& triangle : model) {
        moveTriangle(triangle, offset);
        drawTriangleScanline(triangle);
    }

    window.draw(pixels);
    window.display();
}
