#include <iostream>
#include "../include/body.hpp"

int main() {
	Body n;

	std::cout << n.position[0] << ' ' << n.position[1] << std::endl <<
	n.speed[0] << ' ' << n.speed[1] << std::endl << n.density << std::endl;	
	return 0;
}
