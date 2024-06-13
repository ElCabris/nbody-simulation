# nbody-simulation

**Para ejecutar este programa es necesario tener instalado la libreria sfml**
Para instalar sfml en sistemas basados en debian es suficiente ejecutando el siguiente comando
```
sudo apt install libsfml-dev
```

la implementación con mpi fracasó.

Para compilar y ejecutar el programa
```
g++ -fopenmp openmp_implementation.cpp src/models/body.cpp src/models/vectors_operations.cpp -lsfml-graphics -lsfml-window -lsfml-system
```
## Parámetros de la simulación
Esta simulación permite cambiar ciertas constantes para poder llegar a la simulación deseada. Esas constantes son:

- `number_bodies`: para el número de cuerpos en la simulación.
- `width` y `height`: medidas del ancho y largo del espacio visible en la simulación
- `CUTTOF`: medida del cuttof (variar este parámetro influye directamente en el rendimineto de la simulación) 
