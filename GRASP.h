/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* Project: ReposteameEstaPablo
 * File:   GRASP.h
 * Author: Maria Fernanda Ramos Cantu
 *
 * Created on 29 de junio de 2024, 16:05
 */

#ifndef GRASP_H
#define GRASP_H

#include <vector>
using namespace std;

struct Nodo{
    double tiempo;
    double distancia;
    int punto;
};

void solucionInicialGrasp(struct Problema &problemita, struct Solucion &solucionAux);
int verifica(vector <Nodo>ciudades,double maxrcl);
bool compara(Nodo a, Nodo b);
int asignaClienteRCL(struct Vehiculo &vehiculo, vector<vector<double>> &distancias, vector<Cliente> &clientes, int partida);
#endif /* GRASP_H */

