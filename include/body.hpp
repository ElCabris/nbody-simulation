#ifndef BODY_HPP
#define BODY_HPP

#include <array>
#include <mpi.h>

struct Body{
	std::array<float, 2> position; // (x, y)
	std::array<float, 2> speed; // (x, y)
	std::array<float, 2> acceleration;
	float mass;

	Body();
	Body(std::array<float, 2>, std::array<float, 2>, std::array<float, 2>, float);
	/* función para poner la posición inicial del cuerpo de pendiendo
	del tamño máximo en 'x' y en 'y'
	*/
	void random_position(int, int);
};

void crearTipoMPIBody(MPI_Datatype&);

#endif
