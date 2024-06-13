# nbody-simulation

**Para ejecutar este programa es necesario tener instalado la libreria sfml**
Para instalar sfml en sistemas basados en debian es suficiente ejecutando el siguiente comando
```
sudo apt install libsfml-devwo
```

la implementación con mpi fracasó.

Para compilar y ejecutar el programa
```
g++ -fopenmp openmp_implementation.cpp src/models/body.cpp src/models/vectors_operations.cpp -lsfml-graphics -lsfml-window -lsfml-system
```
