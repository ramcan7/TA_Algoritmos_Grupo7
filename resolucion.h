
/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       resolucion.h
 >> Author:     Grupo 7
[/] */

#ifndef RESOLUCION_H
#define RESOLUCION_H
#include "Funciones Auxiliares.h"


                /*  - / > [ Declaracion de Funciones ] < / -  */
                  /*  - / > [ Funciones en main() ] < / -  */

void GVNS(struct Problema &);

              /*  - / > [ Funciones de Orden Principal ] < / -  */

void solucionInicial(struct Problema &,struct Solucion &);

void VND(struct Problema &,struct Solucion &);

void shaking(Solucion &,int,struct Problema &);

void cambiarVecindario(struct Problema &,struct Solucion &,
                       struct Solucion &,struct Solucion &,int &,int,int &);

             /*  - / > [ Funciones de Orden Secundario ] < / -  */

int masCercanoNodo(vector<vector<double>> &,vector<struct Cliente> &,int);

bool LSInsertar(struct Problema &, struct Solucion &,struct Solucion &, int);

bool LSIntercambiar(struct Problema &,struct Solucion &,struct Solucion &,int);

bool LSRealocar(struct Problema &,struct Solucion &,struct Solucion &,int);

void TInsertar(struct Problema &,struct Solucion &,int);

void TInsertarVehiculo(vector <vector <double>> &,struct Vehiculo &,int);

void TIntercambiar(struct Problema &,struct Solucion &,int);

void TIntercambiarVehiculo(vector <vector <double>> &,vector<struct Cliente> &,
                           struct Vehiculo &,struct Vehiculo &,int);

void TRealocar(struct Problema &,struct Solucion &,int);

void TRealocarVehiculo(vector <vector <double>> &,vector<struct Cliente> &,
                       struct Vehiculo &,struct Vehiculo &,int lClientes);

              /*  - / > [ Funciones de Orden Derivado ] < / -  */

bool posibleSolucion(const struct Solucion &);

double hallarDistancia(const vector<vector<double>> &,int,int);

double hallarFitness(struct Problema &,struct Solucion &);

double hallarCarga_Ruta(vector<struct Cliente> &,const vector<int>);

double hallarTiempoTotal_Ruta(vector<vector<double>> &,vector<struct Cliente> &,
                              const vector<int>);

double hallarDistancia_Ruta(vector<vector<double>> &,const vector<int>);

double hallarTiempoPorServicio_Ruta(vector<struct Cliente> &,const vector<int>);

int compararFitness(struct Solucion &,struct Solucion &);

#endif /* RESOLUCION_H */
