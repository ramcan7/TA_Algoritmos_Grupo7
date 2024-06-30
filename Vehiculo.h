
/* [/]
 >> Project:    ReposteameEstaPablo
 >> File:       Vehiculo.h
 >> Author:     Grupo 7
[/] */

#ifndef VEHICULO_H
#define VEHICULO_H
#include <vector>
#include <algorithm>
using namespace std;
#include "DatosVehiculo.h"

struct Vehiculo {
    struct DatosVehiculo datosVehiculo;
    double capacidad_actual;
    double tiempo_total; //Tiempo que demora con cada cliente
    double distancia_total; //Distancia de la ruta
    vector<int> ruta; // IDs de los clientes en la ruta
};


#endif /* VEHICULO_H */
