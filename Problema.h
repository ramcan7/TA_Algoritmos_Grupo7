
/* [/]
 >> Project:    ReposteameEstaPablo
 >> File:       Problema.h
 >> Author:     Grupo 7
[/] */

#ifndef PROBLEMA_H
#define PROBLEMA_H
#include <vector>
#include <algorithm>
using namespace std;
#include "Cliente.h"
#include "Solucion.h"
#define POSINICIO 2
#define CANT_VECHICULOS 5
#define CARGAMAX 100
#define VEL_PROM 40 //Falta definir la escala del tiempo y distancia

struct Problema {
    vector<vector<double>> distancias; // Matriz de distancias entre clientes
    vector<struct Cliente> clientes;
    vector<struct DatosVehiculo> vehiculos;
    struct Solucion solucion;
    
};



#endif /* PROBLEMA_H */
