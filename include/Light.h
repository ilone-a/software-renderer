#pragma once
// Light.h
#ifndef LIGHT_H
#define LIGHT_H
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

class Light {
public:
	sf::Vector2f position; //
	float intensity;       //

	Light(const sf::Vector2f& pos, float intensity)
		: position(pos), intensity(intensity) {}

	// 
	float getLighting(const sf::Vector2f& point) const {
		// 
		float distance = std::sqrt(std::pow(point.x - position.x, 2) + std::pow(point.y - position.y, 2));
		return intensity / (distance * distance + 1);  // 
	}

};

#endif // LIGHT_H