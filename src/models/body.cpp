#include "../../include/body.hpp"
#include <random>

Body::Body() : speed({0, 0}), acceleration({0, 0}), mass(1.0f) {}

Body::Body(std::array<float, 2> position,
		std::array<float, 2> speed,
		std::array<float, 2> acceleration,
		float mass):
	position(position), speed(speed), acceleration(acceleration), mass(mass) {}

void Body::random_position(int max_x, int max_y) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	position[0] = dis(gen) * max_x;
	position[1] = dis(gen) * max_y;
}

