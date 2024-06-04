// para compilar este programa es necesario primero comentar las partes mpi de los modulos

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../include/body.hpp"
#include "../include/vectors_operations.hpp"
#include <random>
#include <array>

const int number_bodies = 1000;
constexpr double GRAVITATIONAL_CONSTANT = 6.67430e-11;
const double DELTA_T = 0.001;

void calcular(std::array<Body, number_bodies>&);

int main() {
    const int width = 500, height = 500;
    const float radius = 0.3;
    std::array<Body, number_bodies> bodies;

    for (std::array<Body, number_bodies>::iterator it = bodies.begin(); it != bodies.end(); it++) {
        *it = Body();
        it->random_position(width, height);
    }
    
    sf::RenderWindow window(sf::VideoMode(width, height), "n-Bodies");
    std::array<sf::CircleShape, number_bodies> circles;

    for (std::array<sf::CircleShape, number_bodies>::iterator it = circles.begin(); it != circles.end(); it++) {
        *it = sf::CircleShape(radius);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 255);
        sf::Color random_color(dist(gen), dist(gen), dist(gen));
        it->setFillColor(random_color);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        calcular(bodies);

        for (auto it : bodies) {
            std::cout << '{'<< it.position[0] << ", " << it.position[1] << '}' << std::endl;
        }
        std::cout << std::endl;

        for (int i = 0; i < number_bodies; ++i) {
            circles[i].setPosition(bodies[i].position[0], height - bodies[i].position[1]);
            window.draw(circles[i]);
        }

        window.display();
    }
}

void calcular(std::array<Body, number_bodies>& bodies) {
    
    for (int i = 0; i < number_bodies; i++) {
        std::array<double, 2> sum_forces_i = {0, 0};
        for (int j = 0; j < number_bodies; j++) {
            if (i == j)
                continue;
            
            std::array<double, 2> vector_distancia = bodies[j].position - bodies[i].position;
            double norma_distancia = norm_of_a_vector(vector_distancia);
            std::array<double , 2> f_ij = (GRAVITATIONAL_CONSTANT * bodies[i].mass * bodies[j].mass * std::pow(norma_distancia, -3)) * vector_distancia;
            sum_forces_i += f_ij;
        }

        bodies[i].acceleration = std::pow(bodies[i].mass, -1) * sum_forces_i;
        bodies[i].velocity += DELTA_T * bodies[i].acceleration;
        bodies[i].position += DELTA_T * bodies[i].velocity;
    }
}
