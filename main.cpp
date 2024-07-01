
/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       main.cpp
 >> Author:     Grupo 7
[/] */

#include <iomanip>
#include <iostream>
#include <fstream> 
#include <cstring>
using namespace std;
#include "resolucion.h"

// Modulo Principal
int main(int argc, char** argv) {
    // Declaracion del Problema
    struct Problema problemita;
    // Carga de Datos del Problema
    cargarMatriz(problemita.distancias,"Distancias.txt");
    cargarClientes(problemita.clientes,"Clientes.txt");
    cargarVehiculos(problemita.vehiculos);
    // Resolucion del Problema
    //inicialVecinoMasCercano
    GVNS(problemita, 1);
    //inicialGrasp
    GVNS(problemita, 2);
    
    return 0;
}
