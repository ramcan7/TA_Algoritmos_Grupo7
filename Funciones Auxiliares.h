
/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       Funciones_Auxiliares.h
 >> Author:     Grupo 7
[/] */

#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#include "Problema.h"
#include "resolucion.h"

                /*  - / > [ Declaracion de Funciones ] < / -  */

void cargarMatriz(vector<vector<double>> &,const char *);

void cargarClientes(vector<struct Cliente> &,const char *);

void cargarVehiculos(vector <struct Vehiculo> &);

void imprimirSolucion(struct Problema& problemita, struct Solucion& solucion, const char *, int);

ifstream abrirArchivo_IFS(const char *);

ofstream abrirArchivo_OFS(const char *);

void imprimirLinea(ofstream &,int,char);

void imprimirEncabezado(ofstream &,int,char);

#endif /* FUNCIONES_AUXILIARES_H */
