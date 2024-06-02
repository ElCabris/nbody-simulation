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

void crearTipoMPIBody(MPI_Datatype& mpi_body_type) {
    Body body;

    int bloques = 4;
    int longitudes[bloques] = {2, 2, 2, 1};

    MPI_Aint desplazamientos[bloques];
    desplazamientos[0] = offsetof(Body, position);
    desplazamientos[1] = offsetof(Body, speed);
    desplazamientos[2] = offsetof(Body, acceleration);
    desplazamientos[3] = offsetof(Body, mass);

    MPI_Datatype tipos[bloques] = {MPI_FLOAT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT};

    MPI_Type_create_struct(bloques, longitudes, desplazamientos, tipos, &mpi_body_type);
    MPI_Type_commit(&mpi_body_type);
}