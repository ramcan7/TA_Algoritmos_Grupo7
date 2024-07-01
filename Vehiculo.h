
/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       Vehiculo.h
 >> Author:     Grupo 7
[/] */

#ifndef VEHICULO_H
#define VEHICULO_H
#include <vector>
#include <algorithm>
using namespace std;

// Definicion de 'Vehiculo'
struct Vehiculo {
    int id;                         // Identificador del Vehiculo
    int velocidad;                  // Velocidad Promedio del Vehiculo
    double capacidad_max;           // Capacidad Maxima del Vehiculo
    double capacidad_actual;        // Capacidad Actual del Vehiculo
    double tiempo_total;            // Tiempo que demora con cada cliente
    double distancia_total;         // Distancia de la ruta
    vector<int> ruta;               // Vector de los identificadores de los clientes de su Ruta
};

#endif /* VEHICULO_H */
