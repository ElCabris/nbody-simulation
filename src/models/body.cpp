#include "../../include/body.hpp"
#include <random>

Body::Body() : acceleration({0, 0}), mass(300000.0) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<double> dis(-1.0f, 1.0f);
	velocity[0] = dis(gen);
	velocity[1] = dis(gen);


}

Body::Body(std::array<double , 2> position,
		std::array<double , 2> velocity,
		std::array<double , 2> acceleration,
		double  mass):
	position(position), velocity(velocity), acceleration(acceleration), mass(mass) {}

void Body::random_position(int max_x, int max_y) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<double> dis(0.0f, 1.0f);

	position[0] = dis(gen) * max_x;
	position[1] = dis(gen) * max_y;
}

// void crearTipoMPIBody(MPI_Datatype& mpi_body_type) {
 //   Body body;

   // int bloques = 4;
    //int longitudes[bloques] = {2, 2, 2, 1};

   // MPI_Aint desplazamientos[bloques];
    //desplazamientos[0] = offsetof(Body, position);
    //desplazamientos[1] = offsetof(Body, velocity);
   // desplazamientos[2] = offsetof(Body, acceleration);
    //desplazamientos[3] = offsetof(Body, mass);

    //MPI_Datatype tipos[bloques] = {MPI_FLOAT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT};

    //MPI_Type_create_struct(bloques, longitudes, desplazamientos, tipos, &mpi_body_type);
    //MPI_Type_commit(&mpi_body_type);
//}
