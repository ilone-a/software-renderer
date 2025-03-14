#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

//Stencil
class StencilBuffer {
public:
    StencilBuffer(unsigned int width, unsigned int height) {};
	int width; int height;
	std::vector<int> stencilBuffer;


    sf::FloatRect getBoundingBox(const std::vector<Triangle>& model) {
        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::min();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::min();

        // Pass all model triangles
        for (const Triangle& triangle : model) {
            minX = std::min({ minX, triangle.v0.x, triangle.v1.x, triangle.v2.x });
            maxX = std::max({ maxX, triangle.v0.x, triangle.v1.x, triangle.v2.x });
            minY = std::min({ minY, triangle.v0.y, triangle.v1.y, triangle.v2.y });
            maxY = std::max({ maxY, triangle.v0.y, triangle.v1.y, triangle.v2.y });
        }

        // Return bbox with 4 params
        return sf::FloatRect(sf::Vector2f(minX, minY), sf::Vector2f(maxX - minX, maxY - minY));
    }

    void updateStencilBuffer(sf::RenderWindow window,
    std::vector<Triangle> model) {
        // Get bounds rectangle for model
        sf::FloatRect boundingBox = getBoundingBox(model);

        // For each pixel on screen check if it is inside bound rectangle
        for (int y = 0; y < window.getSize().y; ++y) {
            for (int x = 0; x < window.getSize().x; ++x) {
                // Check if pixel is inside bounding rect
                if (boundingBox.contains(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)))) {
                    // If is found, set stencil to 1, if not — 0
                    stencilBuffer[y * window.getSize().x + x] = 1; // or another value if need
                }
                else {
                    stencilBuffer[y * window.getSize().x + x] = 0;
                }
            }
        }
    }

	int getStencilIndex(int x, int y) {
		return y * width + x;  // Index to get pixel in stencil buffer
	}

	void setStencilValue(int x, int y, int value) {
		int index = getStencilIndex(x, y);
		if (index >= 0 && index < stencilBuffer.size()) {
			stencilBuffer[index] = value;  // Set stencil settings for pixel
		}
	}

	int getStencilValue(int x, int y, std::vector<int> stencilBuffer) {
		int index = getStencilIndex(x, y);
		if (index >= 0 && index < stencilBuffer.size()) {
			return stencilBuffer[index];  // Return stencil value for pixel
		}
		else return 0;  // If index is not in stencil buffer, return 0
	};
};