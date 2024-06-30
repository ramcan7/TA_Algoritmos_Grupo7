
/* [/]
 >> Project:    ReposteameEstaPablo
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
    GVNS(problemita);
    // Impresion de Solucion
    imprimirSolucion(problemita.solucion,"Solucion.txt");
    
    return 0;
}