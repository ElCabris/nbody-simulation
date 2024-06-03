#include <vector>
#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "body.hpp"
#include <mpi.h>

void operator+=(const std::array<float, 2>&, const std::array<float, 2>&);

std::array<float, 2> operator+(const std::array<float, 2>&,
                                const std::array<float, 2>&);

std::array<float, 2> operator-(const std::array<float, 2>&,
                               const std::array<float, 2>&);

std::array<float, 2> operator*(const float&, const std::array<float, 2>&);

// norm | magnitude | length of a vector
float norm_of_a_vector(const std::array<float, 2>&);

int distribute_bodies(int, int, int);

constexpr float GRAVITATIONAL_CONSTANT = 6.67430e-11f;
const float DELTA_T = 0.001f; // constante del paso del tiempo para el método de euler

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

    // Comunicador de los procesos que hacen los cálculos (todos menos el 0)
    MPI_Comm calculators;
    int color = rank == 0 ? 1 : 0;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &calculators);

    if (rank == 0) {
        // Crear los cuerpos y ponerlos en posiciones aleatorias dentro del espacio
        for (auto& body : bodies) {
            body = Body();
            body.random_position(width, height);
        }
    }

    // Difundir los cuerpos desde el proceso 0 a todos los demás procesos
    MPI_Bcast(bodies.data(), number_bodies, mpi_body_type, 0, MPI_COMM_WORLD);

    // Calcular la distribución de cuerpos para cada proceso
    std::vector<int> elemen_distribution(size - 1);
    std::vector<int> index_distribution(size - 1);
    for (int i = 1; i < size; i++) {
        elemen_distribution[i - 1] = distribute_bodies(i, size, number_bodies);
    }
    index_distribution[0] = 0;
    for (int i = 1; i < size - 1; i++) {
        index_distribution[i] = elemen_distribution[i - 1] + index_distribution[i - 1];
    }

    // Calcula la distribución para cada proceso
    int local_count = distribute_bodies(rank, size, number_bodies);
    int local_start = (rank == 0) ? 0 : index_distribution[rank - 1];

    std::vector<int> recvcounts(size);
    std::vector<int> displs(size);
    recvcounts[0] = local_count;
    displs[0] = 0;

    for (int i = 1; i < size; i++) {
        recvcounts[i] = elemen_distribution[i - 1];
        displs[i] = index_distribution[i - 1];
    }

    if (rank == 0) {
        const int radius = 5;
        sf::RenderWindow window(sf::VideoMode(width, height), "n-Bodies");

        std::vector<sf::CircleShape> circles(number_bodies);

        for (auto& circle : circles) {
            circle = sf::CircleShape(radius);

            // Generar colores aleatorios
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, 255);
            sf::Color random_color(dist(gen), dist(gen), dist(gen)); // r, g, b

            circle.setFillColor(random_color);
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color::Black);

            for (int i = 0; i < number_bodies; ++i) {
                /*
                el origen, para graficar en sfml (y la mayoría de librerias graficas), se encuentra
                en la parte superior izquierda, las 'x' aumentan a la derecha y las 'y' aumentan hacia
                abajo, por lo tanto la formula para pasar las coordenadas cartesianas (en el primer cuadrante)
                sería:
                    cartesianas -> sfml = (x, y) -> (x, height - y)
                */
                circles[i].setPosition(bodies[i].position[0], height - bodies[i].position[1]);
                window.draw(circles[i]);
            }

            window.display();
        }
    } else {
        while (true) {

            // Realizar cálculos para los cuerpos asignados al proceso
            for (int i = local_start; i < local_start + local_count; ++i) {
                // sum_forces_i representa la fuerza que experimienta el cuerpo i
                // con respecto a las demás cuerpos
                std::array<float, 2> sum_foces_i = {0, 0};
                for (int j = 0; j < number_bodies; ++j) {
                    if (j == i) {
                        continue;
                    }

                    // vector_distancia es representa el vector r_j - r_i
                    std::array<float, 2> vector_distancia = bodies[j].position - bodies[i].position;
                    // norma_distancia es el módulo del vector distancia 
                    float norma_distancia =  norm_of_a_vector(vector_distancia);

                    // f_ij representa la fuerza entre pares de cuerpos
                    std::array<float, 2> f_ij = (GRAVITATIONAL_CONSTANT * bodies[i].mass * bodies[j].mass * static_cast<float>(std::pow(norma_distancia, -3))) * vector_distancia;
                    sum_foces_i += f_ij;
                }

                bodies[i].acceleration = static_cast<float>(std::pow(bodies[i].mass, -1)) * sum_foces_i;
                bodies[i].speed += DELTA_T * bodies[i].acceleration;
                bodies[i].position += DELTA_T * bodies[i].speed;

                MPI_Barrier(calculators);
            }
        }
    }
    MPI_Comm_free(&calculators);
    MPI_Type_free(&mpi_body_type);
    MPI_Finalize();
    return 0;
}

void operator+=(std::array<float, 2>& v1, const std::array<float, 2>& v2) {
    v1[0] += v2[0];
    v1[1] += v2[0];
}

std::array<float, 2> operator+(const std::array<float, 2>& v1,
                               const std::array<float, 2>& v2) {
    return {v1[0] + v2[0], v1[1] + v2[1]};
}

std::array<float, 2> operator-(const std::array<float, 2>& v1,
                               const std::array<float, 2>& v2) {
    return {v1[0] + v2[0], v1[1] + v2[1]};
}

std::array<float, 2> operator*(const float& n, const std::array<float, 2>& v) {
    return {n * v[0], n * v[1]};
}


float norm_of_a_vector(const std::array<float, 2>& v) {
    return static_cast<float>(std::pow(std::pow(v[0], 2) + std::pow(v[1], 2), 0.5));
}

int distribute_bodies(int rank, int size, int number_bodies) {
    --size;
    int div = number_bodies / size;
    int residuo = number_bodies % size;

    return (residuo > 0 && rank <= residuo ? ++div : div);
}
