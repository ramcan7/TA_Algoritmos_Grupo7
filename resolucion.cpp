
/* [/]
 >> Project:    ReposteameEstaPablo
 >> File:       resolucion.cpp
 >> Author:     Grupo 7
[/] */

#include <random>
#include <ctime>

#include <valarray>

#include "resolucion.h"
#define L_MIN 1
#define L_MAX 2
#define K_MIN 3
#define K_MAX 5
#define T_MAX 300
#define CLI_POR_VEH 100
#define NO_ENCONTRADO -1

//Puntos faltantes con **





                /*  - / > [ Definicion de Funciones ] < / -  */

//
void GVNS(struct Problema &problemita){
    struct Solucion solucionActual, x_best;
    int t = 0, t_best=0;
    inicializarSolucion(problemita.vehiculos, solucionActual);
    
    srand(time(NULL));
    
    //Falta hacerlo admisible **
    solucionInicial(problemita, solucionActual);
    imprimirSolucion(solucionActual,"SolucionIni.txt");
    
    VND(problemita, solucionActual);
    x_best = solucionActual;
    
    do {
        int k = K_MIN;
        solucionActual = x_best;
        
        while (k <= K_MAX && t < T_MAX) {
            Solucion x_prima;
            do {
                x_prima = solucionActual;
                shaking(x_prima, k, problemita);
            } while (!posibleSolucion(x_prima.solucionVehiculos));
            
            Solucion x_prima_doble = x_prima;
            VND(problemita, x_prima_doble);
            t++; // Incrementar el tiempo
            
            // NeighborhoodChange
            cambiarVecindario(problemita, solucionActual, x_prima_doble, x_best, k, t, t_best);
                
        }
    } while (t < T_MAX);
    problemita.solucion = x_best;
	
    
    
    imprimirSolucion(x_best,"SolucionGNV.txt");
    
    
    //Falta hacer las respectivas asignaciones **
    problemita.solucion = solucionActual;
    
    //Falta el VND(); **
    //Falta shaking **
    //Falta NeighborhoodChangeP **
    
}
//
void shaking(Solucion &solucion, int k, Problema &problemita){
     srand(time(NULL));
      // Perturbar la solución actual para diversificación
    for (int i = 0; i < k; ++i) {
        // Selección aleaotoria de los tipos de vecindario para insertar, realocar e intercambiar
        int neighborhood = rand() % 3;
        // Seleccionar el tamaño de vecindario de manera aleatoria
        int l = L_MIN + rand() % (L_MAX - L_MIN + 1);

        switch (neighborhood) {
            case 0:
                // Insertar l clientes en la misma ruta
                TInsertar(problemita, solucion, l);
                break;
            case 1: {
                // Realocar l clientes entre rutas diferentes
                TRealocar(problemita, solucion, l);
                break;
            }
            case 2: {
                // Intercambiar l clientes entre rutas diferentes
                TIntercambiar(problemita, solucion, l);
                break;
            }
        }
    }
    // Recalcular fitness después del shaking
    hallarFitness(problemita, solucion.solucionVehiculos);
}
//
void inicializarSolucion(const vector<struct DatosVehiculo>& datosVehiculos, struct Solucion& solucionActual){
    for(int i=0; i<datosVehiculos.size(); i++){
        struct Vehiculo vehi;
        vehi.datosVehiculo = datosVehiculos[i];
        vehi.capacidad_actual = 0;
        vehi.tiempo_total = 0;
        solucionActual.solucionVehiculos.push_back(vehi);
    }
}
//
void solucionInicial(struct Problema &problemita, struct Solucion& solucionActual){
    vector<struct Cliente> copiaClientes = problemita.clientes;
    //Eliminación del punto de partida
    copiaClientes.erase(copiaClientes.begin()+POSINICIO);
    
    int cantVehi = problemita.vehiculos.size();
    
    
    int posCercana;
    //Selección de los primeros pedidos atendidos
    for(int i=0; i<cantVehi; i++){
        posCercana = masCercanoNodo(problemita.distancias, copiaClientes, POSINICIO);
        if(posCercana==NO_ENCONTRADO) break;
        solucionActual.solucionVehiculos[i].ruta.push_back(posCercana);
    }
    //Selección de los siguientes nodos atendidos
    int i=0;
    while(true){
        posCercana = masCercanoNodo(problemita.distancias, copiaClientes, solucionActual.solucionVehiculos[i].ruta[solucionActual.solucionVehiculos[i].ruta.size()-1]);
        if(posCercana==NO_ENCONTRADO) break;
        solucionActual.solucionVehiculos[i].ruta.push_back(posCercana);
        i++;
        if(i==cantVehi){
            i=0;
        }
    }
    //Falta hacerle los cambios para que sea admisible **
}
//
int masCercanoNodo(vector<vector<double>> &distancias, vector<struct Cliente> &copiaClientes, int partida){
    double distanciaMin;
    int posI;
    
    int i=0;
    if(i < copiaClientes.size()){
        distanciaMin = hallarDistancia(distancias, partida, i);
        posI = i;
        for(int i=1; i < copiaClientes.size(); i++){
            double dist = hallarDistancia(distancias, partida, i);
            if(dist < distanciaMin){
                distanciaMin = dist;
                posI = i;
            }
        }
        int posMin = copiaClientes[posI].id;
        //copiaClientes.erase(copiaClientes.begin()+i);
        copiaClientes.erase(copiaClientes.begin()+posI);
        return posMin;
    }
    
    return NO_ENCONTRADO;
    
}
//
double hallarDistancia(const vector<vector<double>> &distancias, int partida, int llegada){
    return distancias[partida][llegada];
}
//
double hallarFitness(struct Problema &problemita, vector<struct Vehiculo> &solucion){
    double tiempoAcumulado=0;
    int i;
    for(i=0; i<solucion.size(); i++){
        tiempoAcumulado += hallarTiempoRuta(problemita.distancias, problemita.clientes, solucion[i].ruta);
    }
    
    return tiempoAcumulado/i;
}
//
double hallarDistanciaRuta(vector<vector<double>> &distancias, const vector<int> ruta){
    double distancia;
    int cantClientes = ruta.size();
    if(cantClientes>0){
        distancia = hallarDistancia(distancias, POSINICIO, ruta[0]);

        for(int i=1; i<cantClientes; i++){
            distancia += hallarDistancia(distancias, ruta[i], ruta[i-1]);
        }
        
        distancia += hallarDistancia(distancias, POSINICIO, ruta[cantClientes-1]);
        
        return distancia;
    }
    
    return 0;
    
}
//
double hallarTiempoPorClienteRuta(vector<struct Cliente> &clientes, const vector<int> ruta){
    int tiempo=0;
    for(int i=0; i<ruta.size(); i++){
        tiempo += clientes[ruta[i]].tiempo_servicio;
    }
    return tiempo;
}
//
double hallarCargaTotalRuta(vector<struct Cliente> &clientes, const vector<int> ruta){
    int demanda=0;
    for(int i=0; i<ruta.size(); i++){
        demanda += clientes[ruta[i]].demanda;
    }
    return demanda;
}
//
double hallarTiempoRuta(vector<vector<double>> &distancias, 
        vector<struct Cliente> &clientes, const vector<int> ruta){
    double distancia = hallarDistanciaRuta(distancias, ruta);
    double tiempoClientes = hallarTiempoPorClienteRuta(clientes, ruta);
    
    return tiempoClientes + distancia/VEL_PROM; //Falta definir la escala del tiempo
}
//
bool posibleSolucion(const vector<struct Vehiculo> &solucion){
    //Falta determinar si una solución es admisible
    //Cada vehículo hace, a lo más, un ruta
    
    
    for(int i=0; i<solucion.size(); i++){
        //Cantidad de clientes por vehículo máxima
        if(solucion[i].ruta.size() > CLI_POR_VEH){
            return false;
        }
        
        
    }
    return true;
}
//
//Falta insert, realocate y exchange en español
void TInsertar(struct Problema& problemita, struct Solucion& solucion, int lClientes){
    int vehiculoElegido; //Por random
    vehiculoElegido = rand() % solucion.solucionVehiculos.size();
    TInsertarVehiculo(problemita.distancias, solucion.solucionVehiculos[vehiculoElegido], lClientes);
}
//
 void TInsertarVehiculo(vector <vector <double>>& distancias, struct Vehiculo &vehiculo, int lClientes){
    //Para ejecutar esta parte se debe corroborar que vehiculo.ruta.size() - lClientes > 0
     
     
    int cantAdmisibles = vehiculo.ruta.size() - lClientes + 1;
    int pos = rand()%cantAdmisibles;
    
    
    //Actualizar la distancia
    reacomodarDistanciaPorSaque(distancias, vehiculo, lClientes, pos, cantAdmisibles);
    //Sacar l clientes
    vector<int> porcion(vehiculo.ruta.begin()+pos, vehiculo.ruta.begin()+pos+lClientes);
    vehiculo.ruta.erase(vehiculo.ruta.begin()+pos, vehiculo.ruta.begin()+pos+lClientes);
    
    
    cantAdmisibles = vehiculo.ruta.size();
    
    int nuevaPos= rand()%cantAdmisibles;
    if(nuevaPos>=pos) nuevaPos++;
    
    //Actualizar la distancia
    reacomodarDistanciaPorIngreso(distancias, vehiculo, lClientes, nuevaPos, cantAdmisibles, porcion);
    //Insertar l clientes
    vehiculo.ruta.insert(vehiculo.ruta.begin()+nuevaPos, porcion.begin(), porcion.end());
    
    
}
//
void reacomodarDistanciaPorSaque(const vector <vector <double>>& distancias, struct Vehiculo &vehiculo, int lClientes, int pos, int cantAdmisibles){
    int antesCorte, despuesCorte;
    //Restar las distancias de los arcos por eliminar
    if(pos == 0){
        antesCorte = POSINICIO;
    }
    else{
        antesCorte = vehiculo.ruta[pos-1];
    }
    vehiculo.distancia_total -= hallarDistancia(distancias, antesCorte, vehiculo.ruta[pos]);
    
    if(pos == cantAdmisibles - 1){
        despuesCorte = POSINICIO;
    }
    else{
        despuesCorte = vehiculo.ruta[pos + lClientes];
    }
    vehiculo.distancia_total -= hallarDistancia(distancias, vehiculo.ruta[pos + lClientes - 1], despuesCorte);
    
    //Sumar la distancia del arco generado
    vehiculo.distancia_total += hallarDistancia(distancias, antesCorte, despuesCorte);
}
//
void reacomodarDistanciaPorIngreso(const vector <vector <double>>& distancias, 
        struct Vehiculo &vehiculo, int lClientes, int nuevaPos, int cantAdmisibles, const vector <int> &porcion){
    int antesCorte, despuesCorte;
    //Sumar las distancias de los arcos por ingresar
    if(nuevaPos == 0){
        antesCorte = POSINICIO;
    }
    else{
        antesCorte = vehiculo.ruta[nuevaPos-1];
    }
    vehiculo.distancia_total += hallarDistancia(distancias, antesCorte, porcion[0]);
    
    if(nuevaPos == cantAdmisibles - 1){
        despuesCorte = POSINICIO;
    }
    else{
        despuesCorte = vehiculo.ruta[nuevaPos];
    }
    vehiculo.distancia_total += hallarDistancia(distancias, porcion[lClientes - 1], despuesCorte);
    //Restar la distancia del arco eliminado
    vehiculo.distancia_total -= hallarDistancia(distancias, antesCorte, despuesCorte);
}
//
void TRealocar(struct Problema& problemita, struct Solucion& solucion, int lClientes){
    int v1, v2; //Por random
    while(true){
        v1 = rand() % solucion.solucionVehiculos.size();
        v2 = rand() % solucion.solucionVehiculos.size();
        if(v1!=v2){
            TRealocarVehiculo(problemita.distancias, problemita.clientes, 
                              solucion.solucionVehiculos[v1], solucion.solucionVehiculos[v2], lClientes);
            break;
        }
    }
}

void TRealocarVehiculo(vector <vector <double>>& distancias, vector<struct Cliente> &clientes, 
        struct Vehiculo &vehiculo1, struct Vehiculo& vehiculo2, int lClientes){
    //Para ejecutar esta parte se debe corroborar que vehiculo1.ruta.size() - lClientes > 0
    int cantAdmisibles = vehiculo1.ruta.size() - lClientes + 1;
    int pos = rand()%cantAdmisibles;
    
    
    //Actualizar la distancia
    reacomodarDistanciaPorSaque(distancias, vehiculo1, lClientes, pos, cantAdmisibles);
    //Sacar l clientes de vehículo 1
    vector<int> porcion(vehiculo1.ruta.begin()+pos, vehiculo1.ruta.begin()+pos+lClientes);
    vehiculo1.ruta.erase(vehiculo1.ruta.begin()+pos, vehiculo1.ruta.begin()+pos+lClientes);
    
    
    cantAdmisibles = vehiculo2.ruta.size() + 1;
    int nuevaPos= rand()%cantAdmisibles;
    
    //Actualizar la distancia
    reacomodarDistanciaPorIngreso(distancias, vehiculo2, lClientes, nuevaPos, cantAdmisibles, porcion);
    //Insertar l clientes en vehículo 2
    vehiculo2.ruta.insert(vehiculo2.ruta.begin()+nuevaPos, porcion.begin(), porcion.end());
    
    
    //Modificar los tiempos totales por cliente
    double tiempoCambiado = hallarTiempoPorClienteRuta(clientes, porcion);
    vehiculo1.tiempo_total -= tiempoCambiado;
    vehiculo2.tiempo_total += tiempoCambiado;
    
    //Modificar las cargas totales
    double cargaCambiada = hallarCargaTotalRuta(clientes, porcion);
    vehiculo1.capacidad_actual -= cargaCambiada;
    vehiculo2.capacidad_actual += cargaCambiada;
}
//
void TIntercambiar(struct Problema& problemita, struct Solucion& solucion, int lClientes){
    int v1, v2; //Por random
    while(true){
        v1 = rand() % solucion.solucionVehiculos.size();
        v2 = rand() % solucion.solucionVehiculos.size();
        if(v1!=v2){
            TIntercambiarVehiculo(problemita.distancias, problemita.clientes, 
                              solucion.solucionVehiculos[v1], solucion.solucionVehiculos[v2], lClientes);
            break;
        }
    }
}
//
void TIntercambiarVehiculo(vector <vector <double>>& distancias, vector<struct Cliente> &clientes, 
        struct Vehiculo &vehiculo1, struct Vehiculo& vehiculo2, int lClientes){
    //Para ejecutar esta parte se debe corroborar que vehiculo1.ruta.size() - lClientes > 0
    //Para ejecutar esta parte se debe corroborar que vehiculo2.ruta.size() - lClientes > 0
    int cantAdmisibles = vehiculo1.ruta.size() - lClientes + 1;
    int pos1 = rand()%cantAdmisibles;
    
    //Actualizar la distancia
    reacomodarDistanciaPorSaque(distancias, vehiculo1, lClientes, pos1, cantAdmisibles);
    //Sacar l clientes de vehículo 1
    vector<int> porcion1(vehiculo1.ruta.begin()+pos1, vehiculo1.ruta.begin()+pos1+lClientes);
    vehiculo1.ruta.erase(vehiculo1.ruta.begin()+pos1, vehiculo1.ruta.begin()+pos1+lClientes);
    
    cantAdmisibles = vehiculo2.ruta.size() - lClientes + 1;
    int pos2 = rand()%cantAdmisibles;
    
    //Actualizar la distancia
    reacomodarDistanciaPorSaque(distancias, vehiculo2, lClientes, pos2, cantAdmisibles);
    //Sacar l clientes de vehículo 2
    vector<int> porcion2(vehiculo2.ruta.begin()+pos2, vehiculo2.ruta.begin()+pos2+lClientes);
    vehiculo2.ruta.erase(vehiculo2.ruta.begin()+pos2, vehiculo2.ruta.begin()+pos2+lClientes);
    
    
    
    
    //Actualizar la distancia
    reacomodarDistanciaPorIngreso(distancias, vehiculo1, lClientes, pos1, cantAdmisibles, porcion2);
    //Insertar l clientes en vehículo 1
    vehiculo1.ruta.insert(vehiculo1.ruta.begin()+pos1, porcion2.begin(), porcion2.end());
    
    
    
    //Actualizar la distancia
    reacomodarDistanciaPorIngreso(distancias, vehiculo2, lClientes, pos2, cantAdmisibles, porcion1);
    //Insertar l clientes en vehículo 2
    vehiculo2.ruta.insert(vehiculo2.ruta.begin()+pos2, porcion1.begin(), porcion1.end());
    
    
    //Modificar los tiempos totales por cliente
    double tiempoCambiado = hallarTiempoPorClienteRuta(clientes, porcion1);
    vehiculo1.tiempo_total -= tiempoCambiado;
    vehiculo2.tiempo_total += tiempoCambiado;
    
    tiempoCambiado = hallarTiempoPorClienteRuta(clientes, porcion2);
    vehiculo2.tiempo_total -= tiempoCambiado;
    vehiculo1.tiempo_total += tiempoCambiado;
    
    //Modificar las cargas totales
    double cargaCambiada = hallarCargaTotalRuta(clientes, porcion1);
    vehiculo1.capacidad_actual -= cargaCambiada;
    vehiculo2.capacidad_actual += cargaCambiada;
    
    cargaCambiada = hallarCargaTotalRuta(clientes, porcion2);
    vehiculo2.capacidad_actual -= cargaCambiada;
    vehiculo1.capacidad_actual += cargaCambiada;
    
}

//NeighborhoodChange
void cambiarVecindario(struct Problema &problemita,
        struct Solucion &solucionActual, struct Solucion &x_prima_doble, 
        struct Solucion &x_best, int &k, int &t, int mejorT){
    x_prima_doble.fitnessSolucion = hallarFitness(problemita, x_prima_doble.solucionVehiculos);
    x_best.fitnessSolucion = hallarFitness(problemita, x_best.solucionVehiculos);
    if(x_prima_doble.fitnessSolucion < x_best.fitnessSolucion){
        x_best = x_prima_doble;
        solucionActual = x_prima_doble;
        k = K_MIN;
        t = mejorT;
    }
    else{
        k++;
    }
}

//VND - Variable Neighborhood Descent
void VND(struct Problema &problemita, struct Solucion &solucion){
    struct Solucion solMejoradaPropuesta;
    int i=1;
    while(i <= 3){
        switch(i){
            case 1:
                solMejoradaPropuesta = busquedaLocalVehiculosInsertar(problemita, solucion, 1);
                break;
            case 2:
                solMejoradaPropuesta = busquedaLocalVehiculosRealocar(problemita, solucion, 1);
                break;
            case 3:
                solMejoradaPropuesta = busquedaLocalVehiculosIntercambiar(problemita, solucion, 1);
                break;
        }
        
        solucion.fitnessSolucion = hallarFitness(problemita, solucion.solucionVehiculos);
        solMejoradaPropuesta.fitnessSolucion = hallarFitness(problemita, solMejoradaPropuesta.solucionVehiculos);
        if(solMejoradaPropuesta.fitnessSolucion<solucion.fitnessSolucion){
            solucion = solMejoradaPropuesta;
            i = 1;
        }
        else{
            i++;
        }
    }
}

struct Solucion busquedaLocalVehiculosInsertar( struct Problema &problemita, 
        struct Solucion &solucion, int lClientes){
    struct Solucion solPropuesta = solucion;
    // Insertar l clientes en la misma ruta
    TInsertar(problemita, solPropuesta, lClientes);
    return solPropuesta;
}

struct Solucion busquedaLocalVehiculosRealocar( struct Problema &problemita, 
        struct Solucion &solucion, int lClientes){
    struct Solucion solPropuesta = solucion;
    // Realocar l clientes entre rutas diferentes
    TRealocar(problemita, solPropuesta, lClientes);
    
//    for(int i = 0; i < solPropuesta.solucionVehiculos.size(); i++){
//        TRealocarVehiculo(problemita.distancias, solPropuesta.solucionVehiculos[i], 1);
//    }
    return solPropuesta;
}

struct Solucion busquedaLocalVehiculosIntercambiar( struct Problema &problemita, 
        struct Solucion &solucion, int lClientes){
    struct Solucion solPropuesta = solucion;
    // Intercambiar l clientes entre rutas diferentes
    TIntercambiar(problemita, solPropuesta, lClientes);
    
//    for(int i = 0; i < solPropuesta.solucionVehiculos.size(); i++){
//        TIntercambiarVehiculo(problemita.distancias, solPropuesta.solucionVehiculos[i], 1);
//    }
    return solPropuesta;
}

void mostraRuta(vector<int> ruta){
    for(int i=0; i<ruta.size(); i++){
        cout<<ruta[i]<<" ";
    }
    cout<<endl;
}