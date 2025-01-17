
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
    
    //solucionInicial = VecinoMasCercano
    cout << "Solucion Inicial: Vecino Mas Cercano" << endl;
    GVNS(problemita, 1);
    
    //solucionInicial = Grasp
    cout << "Solucion Inicial: GRASP" << endl;
    GVNS(problemita, 2);
    
    return 0;
}
