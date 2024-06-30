
/* [/]
 >> Project:    ReposteameEstaPablo
 >> File:       Funciones Auxiliares.h
 >> Author:     
[/] */

#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#include "Problema.h"

                /*  - / > [ Declaracion de Funciones ] < / -  */

void cargarMatriz(vector<vector<double>> &,const char *);

void cargarClientes(vector<struct Cliente> &,const char *);

void cargarVehiculos(vector <struct DatosVehiculo> &);

void imprimirSolucion(const struct Solucion &,const char *);

ifstream abrirArchivo_IFS(const char *);

ofstream abrirArchivo_OFS(const char *);

void imprimirLinea(ofstream &,int,char);

#endif /* FUNCIONES_AUXILIARES_H */
