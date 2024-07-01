
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
#include <chrono>
using namespace std;
#include "Cliente.h"
#include "Solucion.h"
#define POSINICIO 2                     // POSICION DE INICIAL DE PARTIDA DE LOS VEHICULOS EN MATRIZ
#define CANT_VECHICULOS 5               // CANTIDAD DE VEHICULOS
#define MAX_ATENCIONES 100              // LIMITE DE ATENCIONES POR VEHICULO
#define MAX_CARGA 400                   // LIMITE DE CARGA POR VEHICULO
#define MAX_TIEMPO 3000                 // LIMITE DE TIEMPO DE DEMORA DE RUTA POR VEHICULO
#define VEL_PROM 2                      // VELOCIDAD PROMEDIO DE VEHICULO [Se toma en hm/min]

// Definicion de 'Problema'
struct Problema {
    vector<vector<double>> distancias;  // Matriz de distancias entre clientes
    vector<struct Cliente> clientes;    // Vector de 'Clientes'
    vector<struct Vehiculo> vehiculos;  // Vector de 'Vehiculos'
    struct Solucion solucion;           // 'Solucion' del Problema
};

#endif /* PROBLEMA_H */
