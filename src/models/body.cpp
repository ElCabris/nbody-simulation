#include "../../include/body.hpp"
#include <random>

Body::Body() : speed({0, 0}), density(1.0f) {}

Body::Body(std::array<float, 2> position, std::array<float, 2> speed, float density):
	position(position), speed(speed), density(density) {}

void Body::random_position(int max_x, int max_y) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	position[0] = dis(gen) * max_x;
	position[1] = dis(gen) * max_y;
}
