
/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       Problema.h
 >> Author:     Grupo 7
[/] */

#ifndef PROBLEMA_H
#define PROBLEMA_H
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;
#include "Cliente.h"
#include "Solucion.h"
#define POSINICIO 2
#define CANT_VECHICULOS 5
#define CARGAMAX 400
#define TIEMPOMAX 3000
#define VEL_PROM 2 // En hm/min

struct Problema {
    vector<vector<double>> distancias; // Matriz de distancias entre clientes
    vector<struct Cliente> clientes;
    vector<struct Vehiculo> vehiculos;
    struct Solucion solucion;
};



#endif /* PROBLEMA_H */
