#ifndef BODY
#define BODY

#include <array>

struct Body{
	std::array<float, 2> position; // (x, y)
	std::array<float, 2> speed; // (x, y)
	float density;

	Body();
	Body(std::array<float, 2>, std::array<float, 2>, float);
	/* función para poner la posición inicial del cuerpo de pendiendo
	del tamño máximo en 'x' y en 'y'
	*/
	void random_position(int, int);
};

#endif
