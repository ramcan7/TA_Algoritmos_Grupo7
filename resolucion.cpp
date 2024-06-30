
/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       resolucion.cpp
 >> Author:     Grupo 7
[/] */

#include "resolucion.h"
#define L_MIN 1
#define L_MAX 2
#define K_MIN 3
#define K_MAX 5
#define T_MAX 100
#define CLI_POR_VEH 100
#define NO_ENCONTRADO -1
#define PEOR_FITNESS 9999
#define MAX_INTENTOS 10

//Puntos faltantes con **





                /*  - / > [ Definicion de Funciones ] < / -  */

//
void GVNS(struct Problema &problemita){
    //
    struct Solucion solucionAux,x_best;
    int t = 0, t_best=0;
    srand(time(NULL));
    //
    inicializarSolucion(problemita.vehiculos,solucionAux);
    solucionInicial1(problemita,solucionAux);
    imprimirSolucion(problemita, solucionAux,"SolucionIni.txt");
    //
    VND(problemita,solucionAux);
    imprimirSolucion(problemita, solucionAux,"SolucionVND1.txt");
    
    x_best = solucionAux;
    cout<<solucionAux.fitnessSolucion<<endl;
    //
    do {
        int k = K_MIN;
        solucionAux = x_best;
        while (k <= K_MAX && t < T_MAX){
            struct Solucion x_prima;
            do{
                x_prima = solucionAux;
                shaking(x_prima,k,problemita);
                cout<<"shake"<<endl;
                cout<<x_prima.fitnessSolucion<<endl;
            } while (!posibleSolucion(x_prima));
            cout<<"Salio shake"<<endl;
            struct Solucion x_prima_doble = x_prima;
            VND(problemita,x_prima_doble);
            t++;
            // NeighborhoodChange
            cambiarVecindario(problemita, solucionAux, x_prima_doble, x_best, k, t, t_best);
                
        }
    } while (t < T_MAX);
    imprimirSolucion(problemita, x_best,"SolucionGNV.txt");
    
    problemita.solucion = x_best;
}
//
void solucionInicial1(struct Problema &problemita, struct Solucion& solucionActual){
    vector<struct Cliente> copiaClientes = problemita.clientes;
    //Eliminación del punto de partida
    copiaClientes.erase(copiaClientes.begin()+POSINICIO);
    
    int cantVehi = problemita.vehiculos.size();
    
    
    int posCercana;
    //Selección de los primeros pedidos atendidos
    for(int i=0; i<cantVehi; i++){
        posCercana = masCercanoNodo1(problemita.distancias, copiaClientes, POSINICIO);
        if(posCercana==NO_ENCONTRADO) break;
        solucionActual.vehiculos[i].ruta.push_back(posCercana);
    }
    //Selección de los siguientes nodos atendidos
    int i=0;
    while(true){
        int papa = solucionActual.vehiculos[i].ruta[solucionActual.vehiculos[i].ruta.size()-1];
        posCercana = masCercanoNodo1(problemita.distancias, copiaClientes, papa);
        if(posCercana==NO_ENCONTRADO) break;
        solucionActual.vehiculos[i].ruta.push_back(posCercana);
        i++;
        if(i==cantVehi){
            i=0;
        }
    }
    solucionActual.fitnessSolucion = hallarFitness1(problemita, solucionActual.vehiculos);
    
}
//
int masCercanoNodo1(vector<vector<double>> &distancias, vector<struct Cliente> &copiaClientes, int partida){
    double distanciaMin;
    int posI;
    
    int i=0;
    if(i < copiaClientes.size()){
        distanciaMin = hallarDistancia(distancias, partida, i);
        posI = i;
        for(i=1; i < copiaClientes.size(); i++){
            double dist = hallarDistancia(distancias, partida, i);
            if(dist < distanciaMin){
                distanciaMin = dist;
                posI = i;
            }
        }
        int posMin = copiaClientes[posI].id;
        copiaClientes.erase(copiaClientes.begin()+posI);
        return posMin;
    }
    
    return NO_ENCONTRADO;
    
}
//
void inicializarSolucion(const vector<struct Vehiculo> &vehiculos,
                         struct Solucion &solucionAux){
    for(int i = 0; i < vehiculos.size(); i++){
        struct Vehiculo vehiculo;
        vehiculo = vehiculos[i];
        solucionAux.vehiculos.push_back(vehiculo);
    }
}
//
void solucionInicial(struct Problema &problemita,struct Solucion &solucionAux){
    //
    bool hayVehiculosDisponibles = false;
    int posCercana,cantVehi = problemita.vehiculos.size();
    // Selección de los primeros pedidos atendidos
    for(int i = 0; i < cantVehi;i++){
        posCercana = masCercanoNodo(problemita.distancias,problemita.clientes,POSINICIO);
        if(posCercana == NO_ENCONTRADO) break;
        solucionAux.vehiculos[i].capacidad_actual += problemita.clientes[posCercana].demanda;
        solucionAux.vehiculos[i].ruta.push_back(posCercana);
    }
    //Selección de los siguientes nodos atendidos
    for(int i = 0,posInicial;1;){
        posInicial = solucionAux.vehiculos[i].ruta[solucionAux.vehiculos[i].ruta.size()-1];
        posCercana = masCercanoNodo(problemita.distancias,problemita.clientes,
                                    posInicial);
        if(posCercana == NO_ENCONTRADO) break;
        if(solucionAux.vehiculos[i].capacidad_max >=
                solucionAux.vehiculos[i].capacidad_actual +
                problemita.clientes[posCercana].demanda ){
            hayVehiculosDisponibles = true;
            solucionAux.vehiculos[i].capacidad_actual += problemita.clientes[posCercana].demanda;
            solucionAux.vehiculos[i].ruta.push_back(posCercana);
        } else problemita.clientes[posCercana].atendido = false;
        i++;
        if(i == cantVehi){
            if(!hayVehiculosDisponibles) break;
            else hayVehiculosDisponibles = false;
            i = 0;
        }
    }
    //
    actualizarRegistrosDeSolucion(problemita.distancias,problemita.clientes,
                                  solucionAux);
}
void actualizarRegistrosDeSolucion(vector<vector<double>> &distancias,
                                   vector<struct Cliente> &clientes,
                                   struct Solucion &solucionAux){
    double tiempo,distancia;
    for(int i = 0;i < solucionAux.vehiculos.size();i++){
        tiempo = hallarTiempoRuta(distancias,clientes,solucionAux.vehiculos[i].ruta);
        solucionAux.vehiculos[i].tiempo_total = tiempo;
        distancia = hallarDistanciaRuta(distancias,solucionAux.vehiculos[i].ruta);
        solucionAux.vehiculos[i].distancia_total = distancia;
    }
}
//
int masCercanoNodo(vector<vector<double>> &distancias,
                   vector<struct Cliente> &clientes,int partida){
    //
    int i = 0,posI = NO_ENCONTRADO,posMin;
    double distanciaMin = 9999,dist;
    //
    for(int i = 0; i < clientes.size(); i++){
        dist = hallarDistancia(distancias,partida,i);
        if(i != partida and dist < distanciaMin and not clientes[i].atendido){
            distanciaMin = dist;
            if(posI >= 0) clientes[posI].atendido = false;
            clientes[i].atendido = true;
            posI = i;
        }
    }
    return posI;
}
//
double hallarDistancia(const vector<vector<double>> &distancias,int partida,
                       int llegada){
    return distancias[partida][llegada];
}
// VND - Variable Neighborhood Descent
void VND(struct Problema &problemita,struct Solucion &solucion){
    struct Solucion solMejoradaPropuesta;
    bool encontrado;
    int i = 1;
    while(i <= 3){
        solMejoradaPropuesta = solucion;
        
        switch(i){
            case 1:
//                imprimirCasos(solMejoradaPropuesta.vehiculos[1].ruta);
                encontrado = LSInsertar(problemita, solucion, solMejoradaPropuesta, 1);
//                imprimirCasos(solMejoradaPropuesta.vehiculos[1].ruta);
                break;
            case 2:
                encontrado = LSIntercambiar(problemita, solucion, solMejoradaPropuesta, 1);
                break;
            case 3:
                encontrado = LSRealocar(problemita, solucion, solMejoradaPropuesta, 1);
                break;
        }
        if(encontrado){
            solucion = solMejoradaPropuesta;
            i = 1;
        } else i++;
    }
}
//
bool LSInsertar(struct Problema& problemita, struct Solucion& solucionActual, struct Solucion &solMejoradaPropuesta, int lClientes){
    //Retorna si se encontró una mejor solución en las vecindades
    
//    //Extra
//    int pop=0;
//    char nombre[] = "nombre      .txt";
//    double fit, fat;
//    //
    
    bool encontrado = false;
    vector <vector<double>>& distancias = problemita.distancias;
    for(int i=0; i < solucionActual.vehiculos.size(); i++){
        struct Vehiculo & vehiculoActual = solucionActual.vehiculos[i];
        int cantAdmisibles = vehiculoActual.ruta.size() - lClientes + 1;
        int cantAdmisibles2 = vehiculoActual.ruta.size() - lClientes;
        if(cantAdmisibles2 < 1) continue;
        for(int pos=0; pos < cantAdmisibles; pos++){
            for(int nuevaPos=0; nuevaPos < cantAdmisibles2; nuevaPos++){
                struct Solucion solucion = solucionActual;
                struct Vehiculo& vehiculo = solucion.vehiculos[i];
                int nuevaPos2 = nuevaPos;
                
                
                //Sacar l clientes
                vector<int> porcion(vehiculo.ruta.begin()+pos, vehiculo.ruta.begin()+pos+lClientes);
                vehiculo.ruta.erase(vehiculo.ruta.begin()+pos, vehiculo.ruta.begin()+pos+lClientes);
                
                if(nuevaPos >= pos) nuevaPos2 = nuevaPos + 1;
                
                //Insertar l clientes
                vehiculo.ruta.insert(vehiculo.ruta.begin()+nuevaPos2, porcion.begin(), porcion.end());
                
                
                
                solucion.fitnessSolucion = hallarFitness1(problemita, solucion.vehiculos);
                if(compararFitness(solucion, solMejoradaPropuesta) > 0){
                    
                    
                    
                    
                    solMejoradaPropuesta = solucion;
                    encontrado = true;
                }

            }
        }
    }
    
    return encontrado;
}
//
void imprimirCasos(struct vector<int> ruta){
    for(int i=0; i<ruta.size(); i++){
        cout<<ruta[i]<<" ";
    }
    cout<<endl;
}
//
bool LSIntercambiar(struct Problema& problemita, struct Solucion& solucionActual, struct Solucion &solMejoradaPropuesta, int lClientes){
    //Retorna si se encontró una mejor solución en las vecindades
    bool encontrado = false;
    vector <vector<double>>& distancias = problemita.distancias;
    vector <struct Cliente>& clientes = problemita.clientes;
    for(int i=0; i < solucionActual.vehiculos.size() - 1; i++){
//        cout<<"Caso 1"<<endl<<endl;
        for(int j=i+1; j < solucionActual.vehiculos.size(); j++){
//            cout<<"Caso"<<i<<" "<<j<<endl<<endl;
            struct Vehiculo & vehiculoActual1 = solucionActual.vehiculos[i];
            struct Vehiculo & vehiculoActual2 = solucionActual.vehiculos[j];
            
            int cantAdmisibles = vehiculoActual1.ruta.size() - lClientes + 1;
            int cantAdmisibles2 = vehiculoActual2.ruta.size() - lClientes + 1;
            
            if(cantAdmisibles <= 0 or cantAdmisibles2 <= 0) continue;
            
            for(int pos1=0; pos1 < cantAdmisibles; pos1++){
//                cout<<"Vehiculo1"<<endl<<endl;
                for(int pos2=0; pos2 < cantAdmisibles2; pos2++){
                    struct Solucion solucion = solucionActual;
                    struct Vehiculo& vehiculo1 = solucion.vehiculos[i];
                    struct Vehiculo& vehiculo2 = solucion.vehiculos[j];
                    
//                    imprimirCasos(vehiculo1.ruta);
//                    imprimirCasos(vehiculo2.ruta);
//                    cout<<endl;
                    
                    //Sacar l clientes de vehículo 1
                    vector<int> porcion1(vehiculo1.ruta.begin()+pos1, vehiculo1.ruta.begin()+pos1+lClientes);
                    vehiculo1.ruta.erase(vehiculo1.ruta.begin()+pos1, vehiculo1.ruta.begin()+pos1+lClientes);
                    
                    //Sacar l clientes de vehículo 2
                    vector<int> porcion2(vehiculo2.ruta.begin()+pos2, vehiculo2.ruta.begin()+pos2+lClientes);
                    vehiculo2.ruta.erase(vehiculo2.ruta.begin()+pos2, vehiculo2.ruta.begin()+pos2+lClientes);



                    //Insertar l clientes en vehículo 1
                    vehiculo1.ruta.insert(vehiculo1.ruta.begin()+pos1, porcion2.begin(), porcion2.end());



                    //Insertar l clientes en vehículo 2
                    vehiculo2.ruta.insert(vehiculo2.ruta.begin()+pos2, porcion1.begin(), porcion1.end());


                    solucion.fitnessSolucion = hallarFitness1(problemita, solucion.vehiculos);
                    
//                    imprimirCasos(vehiculo1.ruta);
//                    imprimirCasos(vehiculo2.ruta);
//                    cout<<endl;
                    if(compararFitness(solucion, solMejoradaPropuesta) > 0){
                        //Prueba
//                        cout<<solucion.fitnessSolucion<<" "<<solMejoradaPropuesta.fitnessSolucion<<endl;
                        
                        solMejoradaPropuesta = solucion;
                        encontrado = true;
                    }
                }
            }
        }
    }
    
    return encontrado;
}
//
bool LSRealocar(struct Problema& problemita, struct Solucion& solucionActual, struct Solucion &solMejoradaPropuesta, int lClientes){
    //Retorna si se encontró una mejor solución en las vecindades
    bool encontrado = false;
    vector <vector<double>>& distancias = problemita.distancias;
    vector <struct Cliente>& clientes = problemita.clientes;
    for(int i=0; i < solucionActual.vehiculos.size(); i++){
        for(int j=0; j < solucionActual.vehiculos.size(); j++){
            if(i==j) continue;
            struct Vehiculo & vehiculoActual1 = solucionActual.vehiculos[i];
            struct Vehiculo & vehiculoActual2 = solucionActual.vehiculos[j];
            
            int cantAdmisibles = vehiculoActual1.ruta.size() - lClientes + 1;
            int cantAdmisibles2 = vehiculoActual2.ruta.size() + 1;
            
            if(cantAdmisibles <= 0 or cantAdmisibles2 <= 0) continue;
            
            for(int pos=0; pos < cantAdmisibles; pos++){
                for(int nuevaPos=0; nuevaPos < cantAdmisibles2; nuevaPos++){
                    struct Solucion solucion = solucionActual;
                    struct Vehiculo & vehiculo1 = solucion.vehiculos[i];
                    struct Vehiculo & vehiculo2 = solucion.vehiculos[j];

                    
                    //Sacar l clientes de vehículo 1
                    vector<int> porcion(vehiculo1.ruta.begin()+pos, vehiculo1.ruta.begin()+pos+lClientes);
                    vehiculo1.ruta.erase(vehiculo1.ruta.begin()+pos, vehiculo1.ruta.begin()+pos+lClientes);


                    //Insertar l clientes en vehículo 2
                    vehiculo2.ruta.insert(vehiculo2.ruta.begin()+nuevaPos, porcion.begin(), porcion.end());


                    
                    solucion.fitnessSolucion = hallarFitness1(problemita, solucion.vehiculos);
                    
                    if(compararFitness(solucion, solMejoradaPropuesta) > 0){
                        solMejoradaPropuesta = solucion;
                        encontrado = true;
                    }
                }
            }
        }
    }
    
    return encontrado;
}
//
void TInsertar(struct Problema &problemita, struct Solucion& solucion, int lClientes){
    int pos;
    for(int i=0; i<MAX_INTENTOS; i++){
        
        pos = rand()%solucion.vehiculos.size();
        if(solucion.vehiculos[pos].ruta.size() - lClientes > 0){
            TInsertarVehiculo(problemita.distancias, solucion.vehiculos[pos], lClientes);
            break;
        }
    }
}
//
void TInsertarVehiculo(vector <vector <double>>& distancias, struct Vehiculo &vehiculo, int lClientes){
    //Para ejecutar esta parte se debe corroborar que vehiculo.ruta.size() - lClientes > 0

    int cantAdmisibles = vehiculo.ruta.size() - lClientes + 1;
    int pos = rand()%cantAdmisibles;
    
    //Sacar l clientes
    vector<int> porcion(vehiculo.ruta.begin()+pos, vehiculo.ruta.begin()+pos+lClientes);
    vehiculo.ruta.erase(vehiculo.ruta.begin()+pos, vehiculo.ruta.begin()+pos+lClientes);
  
    cantAdmisibles = vehiculo.ruta.size();
    
    int nuevaPos= rand()%cantAdmisibles;
    if(nuevaPos >= pos) nuevaPos++;
    
    //Insertar l clientes
    vehiculo.ruta.insert(vehiculo.ruta.begin()+nuevaPos, porcion.begin(), porcion.end());
}
//
int compararFitness(struct Solucion& solucion1, struct Solucion& solucion2){
    //Retorna 1 para mejor
    //0 para igual
    //-1 para peor
    if(solucion1.fitnessSolucion == PEOR_FITNESS){
        if(solucion2.fitnessSolucion == PEOR_FITNESS){
            return 0;
        }
        else{
            return -1;
        }
    }
    else{
        if(solucion1.fitnessSolucion == PEOR_FITNESS){
            return 1;
        }
        else{
            if(solucion1.fitnessSolucion < solucion2.fitnessSolucion){
                return 1;
            }
            else if(solucion1.fitnessSolucion == solucion2.fitnessSolucion){
                return 0;
            }
            else{
                return -1;
            }
        }
    }
}
//
double hallarFitness1(struct Problema &problemita,
                     vector<struct Vehiculo> &solucion){
    int i;
    double tiempoAcumulado = 0;
    double cargaAcumulada = 0;
    double tiempo, distancia, tiempoReal;
    double carga;
    int cliPorVeh;
    for(i = 0; i < solucion.size();i++){
        tiempoReal =  hallarTiempoRuta(problemita.distancias,problemita.clientes,solucion[i].ruta);
        carga = hallarCargaTotalRuta(problemita.clientes, solucion[i].ruta);
        cargaAcumulada += carga;
        tiempoAcumulado += tiempoReal;
        cliPorVeh = solucion[i].ruta.size();
        if(carga > CARGAMAX or cliPorVeh > CLI_POR_VEH or  tiempo > TIEMPOMAX){
            return PEOR_FITNESS;
        }
    }
    return (tiempoAcumulado)/i;
}
//
double hallarTiempoRuta(vector<vector<double>> &distancias,
                        vector<struct Cliente> &clientes,
                        const vector<int> ruta){
    double distancia = hallarDistanciaRuta(distancias,ruta);
    double tiempoClientes = hallarTiempoPorClienteRuta(clientes,ruta);
    return tiempoClientes + distancia/VEL_PROM; //Falta definir la escala del tiempo
}
//
double hallarDistanciaRuta(vector<vector<double>> &distancias,
                           const vector<int> ruta){
    double distancia = 0;
    int cantClientes = ruta.size();
    if(cantClientes > 0){
        distancia = hallarDistancia(distancias,POSINICIO,ruta[0]);
        for(int i=1; i<cantClientes; i++){
            distancia += hallarDistancia(distancias, ruta[i], ruta[i-1]);
        }
        distancia += hallarDistancia(distancias,ruta[cantClientes-1],POSINICIO);
    }
    return distancia;
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
void shaking(Solucion &solucion, int k, Problema &problemita){
    srand(time(NULL));
      // Perturbar la solución actual para diversificación
    for (int i = 0; i < k; ++i) {
        // Selección aleaotoria de los tipos de vecindario para insertar, realocar e intercambiar
        int neighborhood = rand() % 3;
        // Seleccionar el tamaño de vecindario de manera aleatoria
        int ele = L_MIN + rand() % (L_MAX - L_MIN + 1);
        switch (neighborhood) {
            case 0:
                // Insertar l clientes en la misma ruta
                TInsertar(problemita,solucion,ele);
                solucion.fitnessSolucion = hallarFitness1(problemita, solucion.vehiculos);
                cout<<solucion.fitnessSolucion<<endl;
                break;
            case 1: {
                // Realocar l clientes entre rutas diferentes
                TRealocar(problemita, solucion,ele);
                solucion.fitnessSolucion = hallarFitness1(problemita, solucion.vehiculos);
                cout<<solucion.fitnessSolucion<<endl;
                break;
            }
            case 2: {
                // Intercambiar l clientes entre rutas diferentes
                TIntercambiar(problemita,solucion,ele);
                solucion.fitnessSolucion = hallarFitness1(problemita, solucion.vehiculos);
                cout<<solucion.fitnessSolucion<<endl;
                break;
            }
        }
    }
    // Recalcular fitness después del shaking
    solucion.fitnessSolucion = hallarFitness1(problemita, solucion.vehiculos);
}
//
bool posibleSolucion(const struct Solucion &solucion){
    if(solucion.fitnessSolucion == PEOR_FITNESS ) return false;
    return true;
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
void TRealocar(struct Problema& problemita, struct Solucion& solucion, int lClientes){
    int v1, v2; //Por random
    for(int i=0; i<MAX_INTENTOS; i++){
        v1 = rand() % solucion.vehiculos.size();
        v2 = rand() % solucion.vehiculos.size();
        if(v1!=v2 and solucion.vehiculos[v1].ruta.size() - lClientes >= 0){
            TRealocarVehiculo(problemita.distancias, problemita.clientes, 
                              solucion.vehiculos[v1], solucion.vehiculos[v2], lClientes);
            solucion.fitnessSolucion = hallarFitness1(problemita, solucion.vehiculos);
            break;
        }
    }
}
//
void TRealocarVehiculo(vector <vector <double>>& distancias, vector<struct Cliente> &clientes, 
        struct Vehiculo &vehiculo1, struct Vehiculo& vehiculo2, int lClientes){
    //Para ejecutar esta parte se debe corroborar que vehiculo1.ruta.size() - lClientes > 0
    int cantAdmisibles = vehiculo1.ruta.size() - lClientes + 1;
    int pos = rand()%cantAdmisibles;
    
    
    //Sacar l clientes de vehículo 1
    vector<int> porcion(vehiculo1.ruta.begin()+pos, vehiculo1.ruta.begin()+pos+lClientes);
    vehiculo1.ruta.erase(vehiculo1.ruta.begin()+pos, vehiculo1.ruta.begin()+pos+lClientes);
    
    
    cantAdmisibles = vehiculo2.ruta.size() + 1;
    int nuevaPos= rand()%cantAdmisibles;
    
    //Insertar l clientes en vehículo 2
    vehiculo2.ruta.insert(vehiculo2.ruta.begin()+nuevaPos, porcion.begin(), porcion.end());
    
    
}
//
void TIntercambiar(struct Problema& problemita, struct Solucion& solucion, int lClientes){
    int v1, v2; //Por random
    for(int i=0; i<MAX_INTENTOS; i++){
        v1 = rand() % solucion.vehiculos.size();
        v2 = rand() % solucion.vehiculos.size();
        if(v1!=v2 and solucion.vehiculos[v1].ruta.size() - lClientes >= 0 and solucion.vehiculos[v2].ruta.size() - lClientes >= 0){
            TIntercambiarVehiculo(problemita.distancias, problemita.clientes, 
                              solucion.vehiculos[v1], solucion.vehiculos[v2], lClientes);
            solucion.fitnessSolucion = hallarFitness1(problemita, solucion.vehiculos);
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
    
    //Sacar l clientes de vehículo 1
    vector<int> porcion1(vehiculo1.ruta.begin()+pos1, vehiculo1.ruta.begin()+pos1+lClientes);
    vehiculo1.ruta.erase(vehiculo1.ruta.begin()+pos1, vehiculo1.ruta.begin()+pos1+lClientes);
    
    int cantAdmisibles2 = vehiculo2.ruta.size() - lClientes + 1;
    int pos2 = rand()%cantAdmisibles2;
    
    //Sacar l clientes de vehículo 2
    vector<int> porcion2(vehiculo2.ruta.begin()+pos2, vehiculo2.ruta.begin()+pos2+lClientes);
    vehiculo2.ruta.erase(vehiculo2.ruta.begin()+pos2, vehiculo2.ruta.begin()+pos2+lClientes);
    
    
    
    
    //Insertar l clientes en vehículo 1
    vehiculo1.ruta.insert(vehiculo1.ruta.begin()+pos1, porcion2.begin(), porcion2.end());
    
    
    
    //Insertar l clientes en vehículo 2
    vehiculo2.ruta.insert(vehiculo2.ruta.begin()+pos2, porcion1.begin(), porcion1.end());
    
    
    
}

//NeighborhoodChange
void cambiarVecindario(struct Problema &problemita,
        struct Solucion &solucionAux, struct Solucion &x_prima_doble, 
        struct Solucion &x_best, int &k, int &t, int mejorT){
    x_prima_doble.fitnessSolucion = hallarFitness1(problemita, x_prima_doble.vehiculos);
    x_best.fitnessSolucion = hallarFitness1(problemita, x_best.vehiculos);
    if(x_prima_doble.fitnessSolucion < x_best.fitnessSolucion){
        x_best = x_prima_doble;
        solucionAux = x_prima_doble;
        k = K_MIN;
        t = mejorT;
    }
    else{
        k++;
    }
}
//
struct Solucion busquedaLocalVehiculosRealocar( struct Problema &problemita, 
        struct Solucion &solucion, int lClientes){
    struct Solucion solPropuesta = solucion;
    // Realocar l clientes entre rutas diferentes
    TRealocar(problemita, solPropuesta, lClientes);
    
//    for(int i = 0; i < solPropuesta.vehiculos.size(); i++){
//        TRealocarVehiculo(problemita.distancias, solPropuesta.vehiculos[i], 1);
//    }
    return solPropuesta;
}
//
struct Solucion busquedaLocalVehiculosIntercambiar( struct Problema &problemita, 
        struct Solucion &solucion, int lClientes){
    struct Solucion solPropuesta = solucion;
    // Intercambiar l clientes entre rutas diferentes
    TIntercambiar(problemita, solPropuesta, lClientes);
    
//    for(int i = 0; i < solPropuesta.vehiculos.size(); i++){
//        TIntercambiarVehiculo(problemita.distancias, solPropuesta.vehiculos[i], 1);
//    }
    return solPropuesta;
}
