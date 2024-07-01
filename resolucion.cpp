
/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       resolucion.cpp
 >> Author:     Grupo 7
[/] */

#include "resolucion.h"
#define L_MIN 1                         // Minimo de Elementos Tomados para una Modificacion
#define L_MAX 2                         // Maximo de Elementos Tomados para una Modificacion
#define K_MIN 3                         // Minimo Numero de Agitaciones a Solucion
#define K_MAX 5                         // Maximo Numero de Agitaciones a Solucion
#define T_MAX 10                        // Delimitador de Tiempo de Ejecucion [Tiempo Promedio: <T_MAX 10 == 30s> <T_MAX 100 == 120s>] // Entre mas tiempo sea, la solucion tiende a ser mejor.
#define MAX_INTENTOS 10                 // Maxima Cantidad de Intentos para la Validacion de Una Modificacion Randonomica en los Vehiculos
#define NO_ENCONTRADO -1                // Marcador de Fallo en Busqueda
#define PEOR_FITNESS 9999               // Fitness Descartable por Solucion Aberrante


                /*  - / > [ Definicion de Funciones ] < / -  */
                  /*  - / > [ Funciones en main() ] < / -  */

// General Variable Neighborhood Search
void GVNS(struct Problema &problemita){
    // Declaracion de Variables
    struct Solucion solucionAux,x_best;
    int t = 0,t_best = 0;
    // Inicializacion de Elementos
    srand(time(NULL));
    cout<<fixed<<setprecision(2);
    // Solucion 'Heuristica'
    solucionInicial(problemita,solucionAux);
    imprimirSolucion(problemita,solucionAux,"SolucionIni.txt");
    // Solucion Optimizada por 'Variable Neighborhood Descent'
    cout<<"// La primera optimizacion puede tardar debido a que"<<endl;
    cout<<"// la solucion inicial es altamente reducible.."<<endl;
    VND(problemita,solucionAux);
    imprimirSolucion(problemita,solucionAux,"SolucionVND.txt");
    cout<<setw(18)<<"Nuevo mejor!"<<endl;
    cout<<setw(9)<<'['<<solucionAux.fitness<<']'<<endl;
    // Proceso de Mejora por GNVS
    x_best = solucionAux;
    auto start = chrono::high_resolution_clock::now();                          // Inicializacion del Cronometro
    do {                                                                        // Iterativa del Proceso Randonomico Delimitado por el Tiempo Transcurrido
        int k = K_MIN;
        solucionAux = x_best;
        while (k <= K_MAX && t < T_MAX){                                        // Realizacion de Optimizaciones a Soluciones Aleatorias desde Agitaciones de Solucion Optima
            struct Solucion x_prima;
            while(true){                                                        // Realizacion de Agitaciones Aleatorias Continuas hasta una Posible Solucion
                x_prima = solucionAux;
                shaking(x_prima,k,problemita);
                cout<<"Validando.. "<<endl;
                if(posibleSolucion(x_prima)) break;                             // Validacion de Hallazgo de Posible Solucion
                else cout<<setw(18)<<"[ABERRACION]"<<endl;
            }
            cout<<setw(20)<<"[POSIBLE MEJORA]"<<endl;
            struct Solucion x_prima_doble = x_prima;
            VND(problemita,x_prima_doble);                                      // Optimizacion de Posible Nueva Solucion
            auto end = chrono::high_resolution_clock::now();                    // Marca del Cronometro
            auto duracion = chrono::duration_cast<chrono::seconds>(end - start);
            t = duracion.count();
            // Neighborhood Change // Validacion de Cambio de Vecindario por Hallazgo de Mejor Solucion
            cambiarVecindario(problemita,solucionAux,x_prima_doble,x_best,k,t,
                              t_best);
        }
    } while (t < T_MAX);
    // Solucion del Problema [Optimizada por 'General Variable Neighborhood Search']
    problemita.solucion = x_best;
    imprimirSolucion(problemita,x_best,"SolucionGVNS.txt");
    cout<<endl<<"MEJOR SOLUCION OBTENIDA: "<<problemita.solucion.fitness<<endl;
}

              /*  - / > [ Funciones de Orden Principal ] < / -  */

// Modulo de Generacion de Solucion Heuristica del Problema
void solucionInicial(struct Problema &problemita,struct Solucion &solucionAux){
    cout<<"Inicializando solucion base.."<<endl;
    // Declaracion & Inicializacion de Variables
    int cantVehi = problemita.vehiculos.size(),posCercana,partida = POSINICIO;
    int i,j; vector<struct Cliente> clientesAux = problemita.clientes;
    solucionAux.vehiculos = problemita.vehiculos;
    // Eliminación del Punto de Partida
    clientesAux.erase(clientesAux.begin() + POSINICIO);
    // Selección de los Nodos Atendidos por Vehiculo
    for(i = 0,j = -1;true;i++){
        if(solucionAux.vehiculos[i].ruta.size() == 0) partida = POSINICIO;
        else partida = solucionAux.vehiculos[i].ruta[j];
        posCercana = masCercanoNodo(problemita.distancias,clientesAux,partida);
        if(posCercana == NO_ENCONTRADO) break;
        solucionAux.vehiculos[i].ruta.push_back(posCercana);
        if(i + 1 == cantVehi){
            i = -1;
            j++;
        }
    }
    // Insercion de Fitness de Solucion
    solucionAux.fitness = hallarFitness(problemita,solucionAux);
    cout<<setw(12)<<'['<<solucionAux.fitness<<']'<<endl;
}
// VND - Variable Neighborhood Descent  // Modulo de Optimizacion de Solucion por Modificaciones Validadas por Ruta
void VND(struct Problema &problemita,struct Solucion &solucion){
    cout<<"Optimizando.."<<endl;
    // Declaracion de Variables
    struct Solucion solucionPropuesta;
    bool huboMejora;
    int i = 1;
    // Iterativa del Proceso de Seleccion Secuencial de Mejora
    while(i <= 3){
        solucionPropuesta = solucion;
        switch(i){
            case 1:
                huboMejora = LSInsertar(problemita,solucion,
                                        solucionPropuesta,1);
                break;
            case 2:
                huboMejora = LSIntercambiar(problemita,solucion,
                                            solucionPropuesta,1);
                break;
            case 3:
                huboMejora = LSRealocar(problemita,solucion,
                                        solucionPropuesta,1);
                break;
        }
        // Validacion de Existencia de Mejora
        if(huboMejora){
            solucion = solucionPropuesta;
            i = 1;
            // cout<<solucion.fitness<<endl; // Descomentar para ver el historial de mejoras
        } else i++;
    }
    cout<<setw(9)<<'['<<solucion.fitness<<']'<<endl;
}
// Modulo de Modificacion Randonomica de Solucion por Ruta [Estas Modificaciones se validan antes de Realizarse]
void shaking(Solucion &solucion, int k,struct Problema &problemita){
    cout<<"Shaking.."<<endl;
    // Perturbar la solución actual para diversificación
    for (int i = 0; i < k; ++i) {
        int neighborhood = rand() % 3;                                          // Selección aleaotoria de los tipos de vecindario para insertar, realocar e intercambiar
        int ele = L_MIN + rand() % (L_MAX - L_MIN + 1);                         // Seleccionar el tamaño de vecindario de manera aleatoria
        switch (neighborhood) {
            case 0: // Insertar l clientes en la misma ruta
                TInsertar(problemita,solucion,ele);
                break;
            case 1: // Realocar l clientes entre rutas diferentes
                TRealocar(problemita,solucion,ele);
                break;
            case 2: // Intercambiar l clientes entre rutas diferentes
                TIntercambiar(problemita,solucion,ele);
                break;
        }
        solucion.fitness = hallarFitness(problemita,solucion);
        cout<<solucion.fitness<<endl; // Descomentar para ver el historial de Agitaciones
    }
    cout<<setw(9)<<'['<<solucion.fitness<<']'<<endl;
}
// Neighborhood Change  // Modulo de Validacion de Cambio de Vecindad por Hallazgo de una Mejor Solucion
void cambiarVecindario(struct Problema &problemita,struct Solucion &solucionAux,
                       struct Solucion &x_prima_doble,struct Solucion &x_best,
                       int &k, int t, int &mejorT){
    cout<<"Validando.. "<<endl;
    if(x_prima_doble.fitness < x_best.fitness){
        x_best = x_prima_doble;
        solucionAux = x_prima_doble;
        k = K_MIN;
        mejorT = t;
        cout<<setw(18)<<"Nuevo mejor!"<<endl;
        cout<<setw(9)<<'['<<x_best.fitness<<']'<<endl;
    } else{
        cout<<setw(19)<<"No es mejor.."<<endl;
        k++;
    }
}
             /*  - / > [ Funciones de Orden Secundario ] < / -  */

// Modulo de Busqueda de Identificador de Nodo Mas Cercano
int masCercanoNodo(vector<vector<double>> &distancias,
                   vector<struct Cliente> &clientesAux,int partida){
    // Declaracion de Variables
    int posMin,posI = 0,i = 0;
    double distMin;
    // Validacion de Existencia de Clientes
    if(i < clientesAux.size()){
        distMin = hallarDistancia(distancias,partida,i);
        for(i = 1; i < clientesAux.size(); i++){
            double dist = hallarDistancia(distancias,partida,i);
            if(dist < distMin){
                distMin = dist;
                posI = i;
            }
        }
        posMin = clientesAux[posI].id;
        clientesAux.erase(clientesAux.begin() + posI);
        return posMin;
    } else return NO_ENCONTRADO;
}
// Modulo de Insercion de lClientes [Se Valida el hallazgo de una mejor solución en las vecindades]
bool LSInsertar(struct Problema &problemita, struct Solucion &solucion,
                struct Solucion &solucionPropuesta, int lClientes){
    for(int i=0; i < solucion.vehiculos.size(); i++){
        struct Vehiculo &vehiculoActual = solucion.vehiculos[i];
        int cantAdmisibles = vehiculoActual.ruta.size() - lClientes + 1;
        if(cantAdmisibles <= 1) continue;
        for(int pos = 0;pos < cantAdmisibles;pos++){
            for(int nuevaPos = 0;nuevaPos < cantAdmisibles - 1;nuevaPos++){
                struct Solucion solucionAux = solucion;
                struct Vehiculo &vehiculo = solucionAux.vehiculos[i];
                int nuevaPos2 = nuevaPos;
                // Se sacan 'l' clientes del vehiculo
                vector<int> porcion(vehiculo.ruta.begin()+pos,
                                    vehiculo.ruta.begin()+pos+lClientes);
                vehiculo.ruta.erase(vehiculo.ruta.begin()+pos,
                                    vehiculo.ruta.begin()+pos+lClientes);
                // Validacion para Descarte de Misma Solucion
                if(nuevaPos >= pos) nuevaPos2 = nuevaPos + 1;
                // Se Insertan 'l' clientes en otra posicion del vehiculo
                vehiculo.ruta.insert(vehiculo.ruta.begin() + nuevaPos2,
                                     porcion.begin(),porcion.end());
                // Validacion de Mejora
                solucion.fitness = hallarFitness(problemita,solucion);
                if(compararFitness(solucion,solucionPropuesta) > 0){
                    solucionPropuesta = solucion;
                    return true;
                }

            }
        }
    }
    return false;
}
// Modulo de Intercambio de lClientes [Se Valida el hallazgo de una mejor solución en las vecindades]
bool LSIntercambiar(struct Problema &problemita,struct Solucion &solucion,
                    struct Solucion &solucionPropuesta,int lClientes){
    for(int i = 0;i < solucion.vehiculos.size() - 1; i++){
        for(int j = i + 1; j < solucion.vehiculos.size();j++){
            struct Vehiculo &vehiculoActual1 = solucion.vehiculos[i];
            struct Vehiculo &vehiculoActual2 = solucion.vehiculos[j];
            int cantAdmisibles = vehiculoActual1.ruta.size() - lClientes + 1;
            int cantAdmisibles2 = vehiculoActual2.ruta.size() - lClientes + 1;
            if(cantAdmisibles <= 0 or cantAdmisibles2 <= 0) continue;
            for(int pos1=0; pos1 < cantAdmisibles; pos1++){
                for(int pos2=0; pos2 < cantAdmisibles2; pos2++){
                    struct Solucion solucionAux = solucion;
                    struct Vehiculo &vehiculo1 = solucionAux.vehiculos[i];
                    struct Vehiculo &vehiculo2 = solucionAux.vehiculos[j];
                    // Se Sacan 'l' clientes del vehículo '1'
                    vector<int> porcion1(vehiculo1.ruta.begin()+pos1,
                                         vehiculo1.ruta.begin()+pos1+lClientes);
                    vehiculo1.ruta.erase(vehiculo1.ruta.begin()+pos1,
                                         vehiculo1.ruta.begin()+pos1+lClientes);
                    // Se Sacan 'l' clientes del vehículo '2'
                    vector<int> porcion2(vehiculo2.ruta.begin()+pos2,
                                         vehiculo2.ruta.begin()+pos2+lClientes);
                    vehiculo2.ruta.erase(vehiculo2.ruta.begin()+pos2,
                                         vehiculo2.ruta.begin()+pos2+lClientes);
                    // Se Insertan 'l' clientes del vehículo '1' [En la posicion 1]
                    vehiculo1.ruta.insert(vehiculo1.ruta.begin()+pos1,
                                          porcion2.begin(), porcion2.end());
                    // Se Insertan 'l' clientes del vehículo '2' [En la posicion 2]
                    vehiculo2.ruta.insert(vehiculo2.ruta.begin()+pos2,
                                          porcion1.begin(), porcion1.end());
                    // Validacion de Mejora
                    solucionAux.fitness = hallarFitness(problemita,solucionAux);
                    if(compararFitness(solucionAux,solucionPropuesta) > 0){
                        solucionPropuesta = solucionAux;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
// Modulo de Realocacion de lClientes [Se Valida el hallazgo de una mejor solución en las vecindades]
bool LSRealocar(struct Problema &problemita,struct Solucion &solucion,
                struct Solucion &solucionPropuesta,int lClientes){
    for(int i = 0;i < solucion.vehiculos.size();i++){
        for(int j = 0;j < solucion.vehiculos.size();j++){
            if(i == j) continue;
            struct Vehiculo &vehiculoActual1 = solucion.vehiculos[i];
            struct Vehiculo &vehiculoActual2 = solucion.vehiculos[j];
            int cantAdmisibles = vehiculoActual1.ruta.size() - lClientes + 1;
            int cantAdmisibles2 = vehiculoActual2.ruta.size() + 1;
            if(cantAdmisibles <= 0 or cantAdmisibles2 <= 0) continue;
            for(int pos=0; pos < cantAdmisibles; pos++){
                for(int nuevaPos=0; nuevaPos < cantAdmisibles2; nuevaPos++){
                    struct Solucion solucionAux = solucion;
                    struct Vehiculo &vehiculo1 = solucionAux.vehiculos[i];
                    struct Vehiculo &vehiculo2 = solucionAux.vehiculos[j];
                    // Se Sacan 'l' clientes del vehículo '1'
                    vector<int> porcion(vehiculo1.ruta.begin()+pos,
                                        vehiculo1.ruta.begin()+pos+lClientes);
                    vehiculo1.ruta.erase(vehiculo1.ruta.begin()+pos,
                                         vehiculo1.ruta.begin()+pos+lClientes);
                    // Se Insertan los 'l' en el behiculo '2' 
                    vehiculo2.ruta.insert(vehiculo2.ruta.begin()+nuevaPos,
                                          porcion.begin(), porcion.end());
                    // Validacion de Mejora
                    solucionAux.fitness = hallarFitness(problemita,solucionAux);
                    if(compararFitness(solucionAux,solucionPropuesta) > 0){
                        solucionPropuesta = solucionAux;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
// Modulo de Insercion Randonomica de lClientes en Vehiculo Solucion [Parte Validacion]
void TInsertar(struct Problema &problemita,struct Solucion &solucion,
               int lClientes){
    int pos;
    for(int i=0; i<MAX_INTENTOS; i++){
        pos = rand()%solucion.vehiculos.size();
        if(solucion.vehiculos[pos].ruta.size() - lClientes > 0){                // Validacion de Insercion Permitida 
            TInsertarVehiculo(problemita.distancias, solucion.vehiculos[pos],
                              lClientes);
            break;
        }
    }
}
// Modulo de Insercion Randonomica de lClientes en Vehiculo Solucion [Parte Ejecucion]
void TInsertarVehiculo(vector <vector <double>> &distancias,
                       struct Vehiculo &vehiculo,int lClientes){
    // Declaracion de Variables
    int cantAdmisibles = vehiculo.ruta.size() - lClientes + 1;
    int pos = rand()%cantAdmisibles,nuevaPos;
    // Se sacan 'l' clientes del vehiculo
    vector<int> porcion(vehiculo.ruta.begin() + pos,
                        vehiculo.ruta.begin() + pos + lClientes);
    vehiculo.ruta.erase(vehiculo.ruta.begin() + pos,
                        vehiculo.ruta.begin() + pos + lClientes);
    cantAdmisibles = vehiculo.ruta.size();
    nuevaPos = rand()%cantAdmisibles;
    if(nuevaPos >= pos) nuevaPos++;         // Validacion de Insercion en Diferente Posicion
    // Se insertan los 'l' clientes en el vehiculo en la nueva posicion al azar
    vehiculo.ruta.insert(vehiculo.ruta.begin()+nuevaPos, porcion.begin(),
                         porcion.end());
}
// Modulo de Realocacion Randonomica de lClientes en Vehiculos Al Azar de Solucion [Parte Validacion]
void TIntercambiar(struct Problema &problemita,struct Solucion &solucion,
                   int lClientes){
    // Declaracion de Variables
    int v1, v2;
    for(int i=0; i<MAX_INTENTOS; i++){
        v1 = rand() % solucion.vehiculos.size();
        v2 = rand() % solucion.vehiculos.size();
        if(v1!=v2 and solucion.vehiculos[v1].ruta.size() - lClientes >= 0 and
           solucion.vehiculos[v2].ruta.size() - lClientes >= 0){        // Validacion de Intercambio Permitido 
            TIntercambiarVehiculo(problemita.distancias,problemita.clientes, 
                                  solucion.vehiculos[v1],solucion.vehiculos[v2],
                                  lClientes);
            break;
        }
    }
}
// Modulo de Realocacion Randonomica de lClientes en Vehiculos Al Azar de Solucion [Parte Ejecucion]
void TIntercambiarVehiculo(vector <vector <double>> &distancias,
                           vector<struct Cliente> &clientes,
                           struct Vehiculo &vehiculo1,
                           struct Vehiculo &vehiculo2,int lClientes){
    // Declaracion de Variables
    int cantAdmisibles = vehiculo1.ruta.size() - lClientes + 1,pos2;
    int pos1 = rand()%cantAdmisibles,cantAdmisibles2;
    // Se Sacan 'l' clientes de vehículo 1 [Se Maneja una Posicion Aleatoria para el Intercambio Randonomico]
    vector<int> porcion1(vehiculo1.ruta.begin()+pos1, vehiculo1.ruta.begin()+pos1+lClientes);
    vehiculo1.ruta.erase(vehiculo1.ruta.begin()+pos1, vehiculo1.ruta.begin()+pos1+lClientes);
    // Se Sacan 'l' clientes de vehículo 2 [Se Maneja una Posicion Aleatoria para el Intercambio Randonomico]
    cantAdmisibles2 = vehiculo2.ruta.size() - lClientes + 1;
    pos2 = rand()%cantAdmisibles2;
    vector<int> porcion2(vehiculo2.ruta.begin()+pos2,
                         vehiculo2.ruta.begin()+pos2+lClientes);
    vehiculo2.ruta.erase(vehiculo2.ruta.begin()+pos2,
                         vehiculo2.ruta.begin()+pos2+lClientes);
    // Se Insertan los 'l' clientes en vehículo 1
    vehiculo1.ruta.insert(vehiculo1.ruta.begin()+pos1,
                          porcion2.begin(), porcion2.end());
    // Se Insertan los 'l' clientes en vehículo 2
    vehiculo2.ruta.insert(vehiculo2.ruta.begin()+pos2,
                          porcion1.begin(), porcion1.end());
}
// Modulo de Realocacion Randonomica de lClientes en Vehiculos Al Azar de Solucion [Parte Validacion]
void TRealocar(struct Problema &problemita,struct Solucion &solucion,
               int lClientes){
    // Declaracion de Variables
    int v1, v2;
    for(int i=0;i < MAX_INTENTOS; i++){
        v1 = rand() % solucion.vehiculos.size();       // Seleccion al Azar de Vehiculo
        v2 = rand() % solucion.vehiculos.size();       // Seleccion al Azar de Vehiculo
        if(v1!=v2 and solucion.vehiculos[v1].ruta.size() - lClientes >= 0){     // Validacion de Realocacion Permitida
            TRealocarVehiculo(problemita.distancias, problemita.clientes, 
                              solucion.vehiculos[v1], solucion.vehiculos[v2],
                              lClientes);
            break;
        }
    }
}
// Modulo de Realocacion Randonomica de lClientes en Vehiculos Al Azar de Solucion [Parte Ejecucion]
void TRealocarVehiculo(vector <vector <double>>& distancias,
                       vector<struct Cliente> &clientes,
                       struct Vehiculo &vehiculo1,struct Vehiculo &vehiculo2,
                       int lClientes){
    // Declaracion de Variables
    int cantAdmisibles = vehiculo1.ruta.size() - lClientes + 1;
    int pos = rand()%cantAdmisibles,nuevaPos;
    // Se Sacan 'l' clientes de vehículo 1
    vector<int> porcion(vehiculo1.ruta.begin()+pos,
                        vehiculo1.ruta.begin()+pos+lClientes);
    vehiculo1.ruta.erase(vehiculo1.ruta.begin()+pos,
                         vehiculo1.ruta.begin()+pos+lClientes);
    // Se Saca una Nueva Posicion Aleatoria de Realocacion
    cantAdmisibles = vehiculo2.ruta.size() + 1;
    nuevaPos = rand()%cantAdmisibles;
    // Se Insertan los 'l' clientes en el vehículo 2
    vehiculo2.ruta.insert(vehiculo2.ruta.begin()+nuevaPos,porcion.begin(),
                          porcion.end());
}

              /*  - / > [ Funciones de Orden Derivado ] < / -  */

// Modulo de Validacion de Posible Solucion
bool posibleSolucion(const struct Solucion &solucion){
    if(solucion.fitness == PEOR_FITNESS) return false;
    return true;
}
// Modulo de Retorno de Distancia entre Punto de Partida y Llegada
double hallarDistancia(const vector<vector<double>> &distancias,int partida,
                       int llegada){
    return distancias[partida][llegada];
}
// Modulo de Calculo de Fitness de Solucion
double hallarFitness(struct Problema &problemita,struct Solucion &solucion){
    // Declaracion de Variables
    int i,numAtenciones;
    double tiempoAcumulado = 0,carga,tiempo;
    // Proceso de Calculo
    for(i = 0; i < solucion.vehiculos.size();i++){
        carga = hallarCarga_Ruta(problemita.clientes,
                                 solucion.vehiculos[i].ruta);
        tiempo = hallarTiempoTotal_Ruta(problemita.distancias,
                                        problemita.clientes,
                                        solucion.vehiculos[i].ruta);
        numAtenciones = solucion.vehiculos[i].ruta.size();
        if(carga > MAX_CARGA or numAtenciones > MAX_ATENCIONES or
           tiempo > MAX_TIEMPO) return PEOR_FITNESS;
        else tiempoAcumulado += tiempo;
    }
    return (tiempoAcumulado)/i;
}
// Modulo de Calculo de Carga Total en Ruta
double hallarCarga_Ruta(vector<struct Cliente> &clientes, const vector<int> ruta){
    int demanda=0;
    for(int i=0; i<ruta.size(); i++){
        demanda += clientes[ruta[i]].demanda;
    }
    return demanda;
}
// Modulo de Calculo de Tiempo Total Requerido por Ruta
double hallarTiempoTotal_Ruta(vector<vector<double>> &distancias,
                        vector<struct Cliente> &clientes,
                        const vector<int> ruta){
    double distancia = hallarDistancia_Ruta(distancias,ruta);
    double tiempoClientes = hallarTiempoPorServicio_Ruta(clientes,ruta);
    return tiempoClientes + distancia/VEL_PROM;
}
// Modulo de Calculo de Distancia Total de Ruta
double hallarDistancia_Ruta(vector<vector<double>> &distancias,
                           const vector<int> ruta){
    // Declaracion de Variables
    int cantClientes = ruta.size();
    double distancia = 0;
    // Validacion de Existencia de Clientes
    if(cantClientes > 0){
        distancia = hallarDistancia(distancias,POSINICIO,ruta[0]);
        for(int i = 0;i < cantClientes - 1;i++){
            distancia += hallarDistancia(distancias,ruta[i],ruta[i+1]);
        }
        distancia += hallarDistancia(distancias,ruta[cantClientes-1],POSINICIO);
    }
    return distancia;
}
// Modulo de Calculo de Tiempo Total por Servicio de Ruta
double hallarTiempoPorServicio_Ruta(vector<struct Cliente> &clientes,
                                    const vector<int> ruta){
    int tiempo = 0;
    for(int i = 0; i<ruta.size(); i++)
        tiempo += clientes[ruta[i]].tiempo_servicio;
    return tiempo;
}
// Modulo de Comparacion de Fitness entre Soluciones [<'1'> == MEJOR || <'0'> == IGUAL || <'-1'> == PEOR]
int compararFitness(struct Solucion &solActual,struct Solucion &solPropuesta){
    double diff = solActual.fitness - solPropuesta.fitness;
    if(diff == 0) return 0;         // Ambas Soluciones Son Igual de Eficientes o Ineficientes
    else if(diff > 0) return -1;    // La Solucion Propuesta es Peor que la Solucion Actual
    else return 1;                  // La Solucion Propuesta es Mejor que la Solucion Actual
}
