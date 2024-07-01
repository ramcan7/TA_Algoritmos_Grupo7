/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       GRASP.h
 >> Author:     Grupo 7
[/] */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include "Problema.h"
#include "Solucion.h"
#include "resolucion.h"
#include "GRASP.h"     
#define alfa 0.3
#define NO_ENCONTRADO -1
#define PEOR_FITNESS 9999
#define ITERACIONES 1500

using namespace std;

bool compara(Nodo a, Nodo b){
    return a.distancia < b.distancia or (a.distancia == b.distancia and a.tiempo < b.tiempo);
}

int verifica(vector <Nodo>ciudades,double maxrcl){
    int cont=0;
    for(int i=0;i<ciudades.size();i++) 
        if(maxrcl>=ciudades[i].distancia)
            cont++;
    return cont;    
}

void solucionInicialGrasp(struct Problema &problemita, struct Solucion &solucionAux){
    struct Solucion mejorSolucion;
    mejorSolucion.vehiculos = problemita.vehiculos;
    mejorSolucion.fitness = PEOR_FITNESS;
    for(int k = 0; k < ITERACIONES; k++){
        // Declaracion & Inicializacion de Variables
        int cantVehi = problemita.vehiculos.size(),posCercana,i,partida = POSINICIO;
        vector<struct Cliente> clientesAux = problemita.clientes;
        solucionAux.vehiculos = problemita.vehiculos;
        // Eliminación del punto de partida
        clientesAux.erase(clientesAux.begin() + POSINICIO);
        //Selección de los primeros pedidos atendidos
        for(i = 0;i < cantVehi;i++){
            posCercana = asignaClienteRCL(solucionAux.vehiculos[i], problemita.distancias, clientesAux, partida);
            if(posCercana == NO_ENCONTRADO) break;
            solucionAux.vehiculos[i].ruta.push_back(posCercana);
        }
        //Selección de los siguientes Nodos atendidos
        for(i = 0;true;i++){
            partida = solucionAux.vehiculos[i].ruta[solucionAux.vehiculos[i].ruta.size()-1];
            posCercana = asignaClienteRCL(solucionAux.vehiculos[i], problemita.distancias, clientesAux, partida);
            if(posCercana == NO_ENCONTRADO) break;
            solucionAux.vehiculos[i].ruta.push_back(posCercana);
            if(i + 1 == cantVehi) i = -1;
        }
        solucionAux.fitness = hallarFitness(problemita,solucionAux);
        if(compararFitness(mejorSolucion, solucionAux)<0){
            mejorSolucion = solucionAux;
        }
        if(k%1000==0)
            cout << "  Iteración: " << k << endl;
    }
    solucionAux = mejorSolucion;
    cout<<setw(12)<<'['<<solucionAux.fitness<<']'<<endl;
}

int asignaClienteRCL(struct Vehiculo &vehiculo, vector<vector<double>> &distancias, vector<Cliente> &clientes, int partida){
    int indmax,inda;
    double beta,tau, maxrcl;
    vector<Nodo> candidatos;
    Nodo paso;
    
    //selecciona a los clientes donde se puede ir (tdavia no han sido atentidos)
    for(int i=0;i<distancias.size();i++){
        if(distancias[partida][i]>0 and !clientes[i].atendido){
            paso.distancia=distancias[partida][i];
            paso.tiempo = clientes[i].tiempo_servicio;
            paso.punto=i;
            candidatos.push_back(paso);
        }
    }    
    
    //si hay clientes a los que se puede ir
    if(!candidatos.empty()){
        //hasta que encuentre un cliente que cumpla las condiciones del ejercicio
        do{
            //ordena candidatos
            sort(candidatos.begin(),candidatos.end(),compara);
            beta = candidatos[0].distancia;
            tau = candidatos[candidatos.size()-1].distancia;
            maxrcl = round(beta + alfa*(tau-beta));
            indmax = verifica(candidatos,maxrcl);
            srand(time(NULL));
            //se elige randonomicamente uno de los candidatos dentro del rango
            inda = rand()%indmax;
            struct Cliente cliente = clientes[candidatos[inda].punto];
            //Nodo candidato = candidatos[inda];
            if(cliente.id!=-1 and cliente.demanda + vehiculo.capacidad_actual <= vehiculo.capacidad_max)
            {
                if(cliente.tiempo_servicio + vehiculo.tiempo_total <= MAX_TIEMPO){ 
                    clientes[candidatos[inda].punto].atendido=true;
                    vehiculo.capacidad_actual += clientes[candidatos[inda].punto].demanda;
                    vehiculo.distancia_total += candidatos[inda].distancia;
                    vehiculo.tiempo_total += clientes[candidatos[inda].punto].tiempo_servicio;
                    return candidatos[inda].punto;
                }
            }
            else candidatos.erase(candidatos.begin()+inda);
            //si ningun cliente cumple las condiciones, finaliza busqueda
            if(candidatos.empty()) break;
        }while(true);
    }
    
    return NO_ENCONTRADO;
}

