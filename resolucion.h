
/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       resolucion.h
 >> Author:     Grupo 7
[/] */

#ifndef RESOLUCION_H
#define RESOLUCION_H
#include "Funciones Auxiliares.h"


                /*  - / > [ Declaracion de Funciones ] < / -  */

void GVNS(struct Problema &);

void solucionInicial1(struct Problema &problemita, struct Solucion& solucionActual);

int masCercanoNodo1(vector<vector<double>> &distancias, vector<struct Cliente> &copiaClientes, int partida);

void inicializarSolucion(const vector<struct Vehiculo> &, struct Solucion &);

void solucionInicial(struct Problema &, struct Solucion &);

void actualizarRegistrosDeSolucion(vector<vector<double>> &,
                                   vector<struct Cliente> &,
                                   struct Solucion &);

int masCercanoNodo(vector<vector<double>> &, vector<struct Cliente> &, int);

double hallarDistancia(const vector<vector<double>> &,int,int);

int compararFitness(struct Solucion& solucion1, struct Solucion& solucion2);

double hallarFitness1(struct Problema &problemita,vector<struct Vehiculo> &solucion);

double hallarDistanciaRuta(vector<vector<double>> &distancias, const vector<int> ruta);

double hallarTiempoPorClienteRuta(vector<struct Cliente> &clientes, const vector<int> ruta);

double hallarTiempoRuta(vector<vector<double>> &distancias, 
        vector<struct Cliente> &clientes, const vector<int> ruta);

bool posibleSolucion(const struct Solucion &solucion);

double hallarCargaTotalRuta(vector<struct Cliente> &clientes, const vector<int> ruta);

void TRealocar(struct Problema& problemita, struct Solucion& solucion, int lClientes);

void TRealocarVehiculo(vector <vector <double>>& distancias, vector<struct Cliente> &clientes, 
        struct Vehiculo &vehiculo1, struct Vehiculo& vehiculo2, int lClientes);

void TIntercambiar(struct Problema& problemita, struct Solucion& solucion, int lClientes);

void TIntercambiarVehiculo(vector <vector <double>>& distancias, vector<struct Cliente> &clientes, 
        struct Vehiculo &vehiculo1, struct Vehiculo& vehiculo2, int lClientes);

void VND(struct Problema &problemita, struct Solucion &solucion);

void imprimirCasos(struct vector<int> ruta);

bool LSInsertar(struct Problema& problemita, struct Solucion& solucionActual, struct Solucion &solMejoradaPropuesta, int lClientes);

bool LSIntercambiar(struct Problema& problemita, struct Solucion& solucionActual, struct Solucion &solMejoradaPropuesta, int lClientes);

bool LSRealocar(struct Problema& problemita, struct Solucion& solucionActual, struct Solucion &solMejoradaPropuesta, int lClientes);

void TInsertar(struct Problema &problemita, struct Solucion& solucion, int lClientes);

void TInsertarVehiculo(vector <vector <double>>& distancias, struct Vehiculo &vehiculo, int lClientes);

struct Solucion busquedaLocalVehiculosInsertar( struct Problema &problemita, 
        struct Solucion &solucion, int l);

struct Solucion busquedaLocalVehiculosRealocar( struct Problema &problemita, 
        struct Solucion &solucion, int l);

struct Solucion busquedaLocalVehiculosIntercambiar( struct Problema &problemita, 
        struct Solucion &solucion, int l);

void cambiarVecindario(struct Problema &problemita,
        struct Solucion &solPropuesta, struct Solucion &solLocalMejorada, 
        struct Solucion &mejorSolucion, int &k, int &t, int mejorT);


void shaking(Solucion &solucion, int k, Problema &problemita);


#endif /* RESOLUCION_H */
