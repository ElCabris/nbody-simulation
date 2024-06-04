#include <vector>
#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "body.hpp"
#include "vectors_operations.hpp"
#include <mpi.h>

int distribute_bodies(int, int, int);

constexpr double GRAVITATIONAL_CONSTANT = 6.67430e-11f;
const double DELTA_T = 0.001f;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int height = std::stoi(argv[2]), width = std::stoi(argv[1]), number_bodies = std::stoi(argv[3]);
    std::vector<Body> bodies(number_bodies);

    MPI_Datatype mpi_body_type;
    crearTipoMPIBody(mpi_body_type);

    MPI_Comm calculators;
    int color = rank == 0 ? 1 : 0;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &calculators);

    if (rank == 0) {
        for (auto& body : bodies) {
            body = Body();
            body.random_position(width, height);
        }
    }

    MPI_Bcast(bodies.data(), number_bodies, mpi_body_type, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        const int radius = 5;
        sf::RenderWindow window(sf::VideoMode(width, height), "n-Bodies");
        std::vector<sf::CircleShape> circles(number_bodies);

        for (auto& circle : circles) {
            circle = sf::CircleShape(radius);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, 255);
            sf::Color random_color(dist(gen), dist(gen), dist(gen));
            circle.setFillColor(random_color);
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                   window.close();
            }

            MPI_Recv(bodies.data(), bodies.size(), mpi_body_type, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            window.clear(sf::Color::Black);

            for (int i = 0; i < number_bodies; ++i) {
                circles[i].setPosition(bodies[i].position[0], height - bodies[i].position[1]);
                window.draw(circles[i]);
            }

            window.display();
            sf::sleep(sf::seconds(0.5));
        }
    } else {
        // element_distribution 
        std::vector<int> element_distribution(size - 1);
        std::vector<int> index_distribution(size - 1);

        for (int i = 1; i < size; i++) {
            element_distribution[i - 1] = distribute_bodies(i, size, number_bodies);
        }
        index_distribution[0] = 0;
        for (int i = 1; i < size - 1; i++) {
            index_distribution[i] = element_distribution[i - 1] + index_distribution[i - 1];
        }

        std::cout << "rank " << rank << ": " << index_distribution[rank - 1] << " -> " << index_distribution[rank - 1] + element_distribution[rank - 1] << std::endl;
        while (true) {
            for (int i = index_distribution[rank - 1]; i < index_distribution[rank - 1] + element_distribution[rank - 1]; ++i) {
                std::array<double, 2> sum_foces_i = {0, 0};
                for (int j = 0; j < number_bodies; ++j) {
                    if (j == i) {
                        continue;
                    }

                    std::array<double, 2> vector_distancia = bodies[j].position - bodies[i].position;
                    double norma_distancia = norm_of_a_vector(vector_distancia);
                    std::array<double, 2> f_ij = (GRAVITATIONAL_CONSTANT * bodies[i].mass * bodies[j].mass * std::pow(norma_distancia, -3)) * vector_distancia;
                    sum_foces_i += f_ij;
                }

                bodies[i].acceleration = std::pow(bodies[i].mass, -1) * sum_foces_i;
                bodies[i].velocity += DELTA_T * bodies[i].acceleration;
                bodies[i].position += DELTA_T * bodies[i].velocity;
            }
            MPI_Barrier(calculators);

            std::cout << rank << std::endl;
            for (auto it : bodies) {
                std::cout << '{' << it.position[0] << ", " << it.position[1] << '}' << ' ';
            }

            MPI_Allgatherv(MPI_IN_PLACE, element_distribution[rank - 1], mpi_body_type, 
                        bodies.data(), element_distribution.data(), index_distribution.data(), 
                        mpi_body_type, calculators);

            if (rank == 1) {
                MPI_Send(bodies.data(), bodies.size(), mpi_body_type, 0, 0, MPI_COMM_WORLD);
            }
        }
    }
    MPI_Comm_free(&calculators);
    MPI_Type_free(&mpi_body_type);
    MPI_Finalize();
    return 0;
}

int distribute_bodies(int rank, int size, int number_bodies) {
    --size;
    int div = number_bodies / size;
    int residuo = number_bodies % size;

    return (residuo > 0 && rank <= residuo ? ++div : div);
}
