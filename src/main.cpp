#include <vector>
#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "body.hpp"
#include <mpi.h>

constexpr float GRAVITATIONAL_CONSTANT = 6.67430e-11f;
const float DELTA_T = 0.001f; // cosntante del paso del tiempo para el método de euler

int distribute_bodies(int rank, int size, int number_bodies) {
    
    --size;
	int div = number_bodies / size,
	residuo = number_bodies % size;

	return (residuo > 0 && rank <= residuo ? ++div : div);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Para ejecutar el programa se deben ingresar 3 argumentos en el ejecutable: altura, ancho, cantidad de cuerpos
    int height = std::stoi(argv[2]),
        width = std::stoi(argv[1]),
        number_bodies = std::stoi(argv[3]);
    std::vector<Body> bodies(number_bodies);

    // Se define la estructura body para MPI
    MPI_Datatype mpi_body_type;
    crearTipoMPIBody(mpi_body_type);

    if (rank == 0) {
        // Crear los cuerpos y ponerlos en posiciones aleatorias dentro del espacio
        for (auto& body : bodies) {
            body = Body();
            body.random_position(width, height);
        }
    }

    // Difundir los cuerpos desde el proceso 0 a todos los demás procesos
    MPI_Bcast(bodies.data(), number_bodies, mpi_body_type, 0, MPI_COMM_WORLD);

    if (rank == 0) {

		const int radius = 5;
        sf::RenderWindow window(sf::VideoMode(width, height), "n-Bodies");

        std::vector<sf::CircleShape> circles(number_bodies);

		for (std::vector<sf::CircleShape>::iterator it = circles.begin();
			 it != circles.end(); it++) {

			*it = sf::CircleShape(radius);

			// Generar colores aleatorios
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, 255);
			sf::Color random_color(dist(gen), dist(gen), dist(gen)); // r, g, b

			it->setFillColor(random_color);
		}

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color::Black);

			for (int i = 0; i < number_bodies; i++) {
				circles[i].setPosition(bodies[i].position[0], bodies[i].position[1]);
				window.draw(circles[i]);
			}

            window.display();
        }
    }
	else {
		int elemen_distribution[size - 1],
		index_distribution[size - 1];

		// definir cuantos elementos le tocan a cada proceso
		for (int i = 1; i < size; i++) {
			elemen_distribution[i - 1] = distribute_bodies(i, size, number_bodies);
		}

		// determinar desde que indice parte cada proceso
		index_distribution[0] = 0;
		for (int i = 1; i < size; i++) {
			index_distribution[i] += elemen_distribution[i - 1] + index_distribution[i - 1];
		}

		for (int i = index_distribution[rank - 1];
		i < (index_distribution[rank - 1] + elemen_distribution[rank - 1]); i++) {
			
			// los siguientes cálculos son para i con respecto a j
			for (int j = 0; j < number_bodies; j++)  {
				if (j == i) {
					continue;
				}

				// calcular la distancia entre el cuerpo i y j (|rj - ri|)
				float distancia = std::fabs(std::pow(bodies[j].position[0] - bodies[i].position[0], 2)
								+ std::pow(bodies[j].position[1] - bodies[i].position[1], 2));
				

				// con la distancia calculada ahora se puede agregar el kutof (o como se llame)
				// no es más que agregar un if

				// aceleración para el cuerpo en el proximo instante de tiempo es:

				/*
				F_ij = G(m_i)(m_j) (r_j-r_i) / |r_j - r_i|^3 = m*a -> (masa * aceleracion) despejando acelearcion tenemos:

				aceleracion = (G(m_j) / |r_j - r_i|^3) * (r_j - r_i)
				*/

				bodies[i].acceleration = {
					GRAVITATIONAL_CONSTANT * bodies[j].mass * static_cast<float>(std::pow(distancia, -3))
					* (bodies[j].position[0] - bodies[i].position[0]),
					GRAVITATIONAL_CONSTANT * bodies[j].mass * static_cast<float>(std::pow(distancia, -3))
					* (bodies[j].position[1] - bodies[i].position[1]) 
				};

				bodies[i].speed = {
					bodies[i].speed[0] + bodies[i].acceleration[0] * DELTA_T,
					bodies[i].speed[1] + bodies[i].acceleration[1] * DELTA_T
				};

				bodies[i].position = {
					bodies[i].position[0] + bodies[i].speed[0] * DELTA_T,
					bodies[i].position[1] + bodies[i].speed[1] * DELTA_T
				};


			}
		}
	}

    MPI_Type_free(&mpi_body_type);
    MPI_Finalize();
    return 0;
}
