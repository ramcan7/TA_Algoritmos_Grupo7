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

struct nodo{
    int punto;
    int distancia;
    int tiempo;
};

void solucionInicialGrasp(struct Problema &problemita, struct Solucion &solucionAux);
struct Vehiculo minruta(int ini,int fin, vector<vector<double>> &distancias, vector<Cliente> &clientes);
int verifica(vector <nodo>ciudades,double maxrcl);
bool compara(nodo a, nodo b);
void actualizaClientesDisponibles(vector<struct Cliente> &clientes, struct Vehiculo &vehiculo);
#endif /* GRASP_H */

