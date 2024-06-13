#ifndef BODY_HPP
#define BODY_HPP

#include <array>
//#include <mpi.h>

struct Body{
	std::array<double , 2> position; // (x, y)
	std::array<double , 2> velocity; // (x, y)
	std::array<double , 2> acceleration;
	double  mass;

	Body();
	Body(std::array<double , 2>, std::array<double , 2>, std::array<double , 2>, double );
	/* función para poner la posición inicial del cuerpo de pendiendo
	del tamño máximo en 'x' y en 'y'
	*/
	void random_position(int, int);
};

//void crearTipoMPIBody(MPI_Datatype&);

#endif
