/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       Solucion.h
 >> Author:     Grupo 7
[/] */

#ifndef SOLUCION_H
#define SOLUCION_H
#include "Vehiculo.h"

// Definicion de 'Solucion'
struct Solucion{
    vector<struct Vehiculo> vehiculos;  // 'Vehiculos' de la 'Solucion'
    double fitness;                     // Punto de Comparacion entre Soluciones [En este caso, el menor fitness es el mejor]
};

#endif /* SOLUCION_H */
