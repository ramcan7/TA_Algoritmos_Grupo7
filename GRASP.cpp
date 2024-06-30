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
#define ITERACIONES 1000

using namespace std;

//

bool comparaTiempo(nodo a, nodo b){
    return a.tiempo < b.tiempo;
}

bool comparaDistancia(nodo a, nodo b){
    return a.distancia < b.distancia;
}

bool compara(nodo a, nodo b){
    return a.distancia < b.distancia or (a.distancia == b.distancia and a.tiempo < b.tiempo);
}

int verifica(vector <nodo>ciudades,double maxrcl){
    int cont=0;
    for(int i=0;i<ciudades.size();i++) 
        if(maxrcl>=ciudades[i].distancia)
            cont++;
    return cont;    
}

struct Vehiculo minruta(int ini,int fin, vector<vector<double>> &distancias, vector<Cliente> &clientes){
    int beta,tau,ciudad,indmax,inda;
    double maxrcl;
    double total,min=999999;
    struct Vehiculo mejorVehiculo{};
    mejorVehiculo.velocidad = VEL_PROM;
    mejorVehiculo.distancia_total = min;
    
    //int ayuda = 0;
    for(int k=0;k<ITERACIONES;k++){
        ciudad=ini;
        total = 0;

        struct Vehiculo vehiculo{};
        vehiculo.velocidad = VEL_PROM;
        vehiculo.capacidad_max = CARGAMAX;
        while(1){
            vector<nodo>vecinos;
            nodo paso;
            //a que clientes puede ir
            for(int i=0;i<distancias.size();i++){
                if(distancias[ciudad][i]>0 and !clientes[i].atendido){
                    paso.distancia=distancias[ciudad][i];
                    paso.punto=i;
                    vecinos.push_back(paso);
                }
            }    
            if(!vecinos.empty()){
                do{
                    sort(vecinos.begin(),vecinos.end(),compara);
                    beta = vecinos[0].distancia;
                    tau = vecinos[vecinos.size()-1].distancia;
                    maxrcl = round(beta + alfa*(tau-beta));
                    indmax = verifica(vecinos,maxrcl);
                    srand(time(NULL));
                    inda = rand()%indmax;
                    struct nodo nd;
                    nd = vecinos[inda];
                    struct Cliente cliente;
                    cliente = clientes[vecinos[inda].punto];
                    if(cliente.id!=-1 and cliente.demanda + vehiculo.capacidad_actual <= vehiculo.capacidad_max)
                    {
                        if(cliente.tiempo_servicio  +vehiculo.tiempo_total <= TIEMPOMAX){ 
                            clientes[vecinos[inda].punto].atendido=true;
                            ciudad = vecinos[inda].punto;
                            vehiculo.ruta.push_back(vecinos[inda].punto);
                            vehiculo.capacidad_actual += clientes[vecinos[inda].punto].demanda;
                            vehiculo.distancia_total += vecinos[inda].distancia;
                            vehiculo.tiempo_total += clientes[vecinos[inda].punto].tiempo_servicio;
                            break;
                        }
                    }
                    else vecinos.erase(vecinos.begin()+inda);
                    if(vecinos.empty()) break;
                }while(true);
                
            }
            //if(vehiculo.ruta.size()>=(clientes.size()))
            if(ciudad==fin) {
                //vehiculo.ruta.erase(vehiculo.ruta.begin()+vehiculo.ruta.size()-1);
                break;
            }
            if(vecinos.empty()){
                //cout <<"No se encontro solución" <<endl;
                //vehiculo.tiempo_total=0;
                break;
            }
        }    
        if(vehiculo.distancia_total<mejorVehiculo.distancia_total){
            mejorVehiculo = vehiculo;
        } 
        for(int i = 0; i < vehiculo.ruta.size();i++){
            clientes[vehiculo.ruta[i]].atendido = false;
        }
        if(k%50==0)
            cout << "Iteración: " << k << endl;
    }
    mejorVehiculo.distancia_total += distancias[mejorVehiculo.ruta[mejorVehiculo.ruta.size()-1]][POSINICIO];
    return mejorVehiculo;
}


void actualizaClientesDisponibles(vector<struct Cliente> &clientes, struct Vehiculo &vehiculo){
    for(int i = 0; i < vehiculo.ruta.size(); i++){
        clientes[vehiculo.ruta[i]].atendido = true;
    }
}

void solucionInicialGrasp(struct Problema &problemita, struct Solucion &solucionAux){
    //acá los datos de vehículo de SolucionAux están inicializados
    solucionAux.vehiculos = problemita.vehiculos;
    for(int i = 0; i < solucionAux.vehiculos.size(); i++){
        solucionAux.vehiculos[i] = minruta(POSINICIO, POSINICIO, problemita.distancias, problemita.clientes);
        actualizaClientesDisponibles(problemita.clientes, solucionAux.vehiculos[i]);
    }
    solucionAux.fitness = hallarFitness1(problemita, solucionAux.vehiculos);
    problemita.solucion = solucionAux;
}

