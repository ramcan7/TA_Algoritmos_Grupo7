
/* [/]
 >> Project:    ReposteameEstaPablo
 >> File:       resolucion.h
 >> Author:     Grupo 7
[/] */

#ifndef RESOLUCION_H
#define RESOLUCION_H
#include "Funciones Auxiliares.h"


                /*  - / > [ Declaracion de Funciones ] < / -  */

void GVNS(struct Problema &);

void inicializarSolucion(const vector<struct DatosVehiculo>& datosVehiculos, struct Solucion& solucionActual);

void solucionInicial(struct Problema &problemita, struct Solucion& solucionActual);

int masCercanoNodo(vector<vector<double>> &distancias, vector<struct Cliente> &copiaClientes, int partida);

double hallarDistancia(const vector<vector<double>> &distancias, int partida, int llegada);

double hallarFitness(struct Problema &problemita, vector<struct Vehiculo> &solucion);

double hallarDistanciaRuta(vector<vector<double>> &distancias, const vector<int> ruta);

double hallarTiempoPorClienteRuta(vector<struct Cliente> &clientes, const vector<int> ruta);

double hallarCargaTotalRuta(vector<struct Cliente> &clientes, const vector<int> ruta);

double hallarTiempoRuta(vector<vector<double>> &distancias, 
        vector<struct Cliente> &clientes, const vector<int> ruta);

bool posibleSolucion(const vector<struct Vehiculo> &solucion);

void TInsertar(struct Solucion& solucion, int lClientes);

void TInsertarVehiculo(vector <vector <double>>& distancias, struct Vehiculo &vehiculo, int lClientes);

void reacomodarDistanciaPorSaque(const vector <vector <double>>& distancias, struct Vehiculo &vehiculo, int lClientes, int pos, int cantAdmisibles);

void reacomodarDistanciaPorIngreso(const vector <vector <double>>& distancias, 
        struct Vehiculo &vehiculo, int lClientes, int nuevaPos, int cantAdmisibles, const vector <int> &porcion);

void TRealocarVehiculo(vector <vector <double>>& distancias, vector<struct Cliente> &clientes, 
        struct Vehiculo &vehiculo1, struct Vehiculo& vehiculo2, int lClientes);

void TIntercambiarVehiculo(vector <vector <double>>& distancias, vector<struct Cliente> &clientes, 
        struct Vehiculo &vehiculo1, struct Vehiculo& vehiculo2, int lClientes);









void mostraRuta(vector<int> ruta);

#endif /* RESOLUCION_H */
