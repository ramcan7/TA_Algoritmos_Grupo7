
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

void solucionInicial(struct Problema &, struct Solucion&);

int masCercanoNodo(vector<vector<double>> &, vector<struct Cliente> &, int);



void actualizarRegistrosDeSolucion(vector<vector<double>> &,
                                   vector<struct Cliente> &,
                                   struct Solucion &);


double hallarDistancia(const vector<vector<double>> &,int,int);

void VND(struct Problema &, struct Solucion &);

bool LSInsertar(struct Problema&, struct Solucion&, struct Solucion &, int);

void imprimirCasos(struct vector<int>& ruta);

bool LSIntercambiar(struct Problema&, struct Solucion&, struct Solucion &, int);

bool LSRealocar(struct Problema&, struct Solucion&, struct Solucion &, int);

void TInsertar(struct Problema &, struct Solucion&, int);

void TInsertarVehiculo(vector <vector <double>>&, struct Vehiculo &, int);

int compararFitness(struct Solucion& solucion1, struct Solucion& solucion2);

double hallarFitness1(struct Problema &problemita,vector<struct Vehiculo> &solucion);

double hallarTiempoRuta(vector<vector<double>> &distancias, 
        vector<struct Cliente> &clientes, const vector<int> ruta);

double hallarDistanciaRuta(vector<vector<double>> &distancias, const vector<int> ruta);

double hallarTiempoPorClienteRuta(vector<struct Cliente> &clientes, const vector<int> ruta);

void shaking(Solucion &solucion, int k, Problema &problemita);

bool posibleSolucion(const struct Solucion &solucion);

double hallarCargaTotalRuta(vector<struct Cliente> &clientes, const vector<int> ruta);

void TRealocar(struct Problema& problemita, struct Solucion& solucion, int lClientes);

void TRealocarVehiculo(vector <vector <double>>& distancias, vector<struct Cliente> &clientes, 
        struct Vehiculo &vehiculo1, struct Vehiculo& vehiculo2, int lClientes);

void TIntercambiar(struct Problema& problemita, struct Solucion& solucion, int lClientes);

void TIntercambiarVehiculo(vector <vector <double>>& distancias, vector<struct Cliente> &clientes, 
        struct Vehiculo &vehiculo1, struct Vehiculo& vehiculo2, int lClientes);

void cambiarVecindario(struct Problema &problemita,
        struct Solucion &solPropuesta, struct Solucion &solLocalMejorada, 
        struct Solucion &mejorSolucion, int &k, int &t, int mejorT);



#endif /* RESOLUCION_H */
