
/* [/]
 >> Project:    ReposteameEstaPablo
 >> File:       Funciones Auxiliares.cpp
 >> Author:     Grupo 7
[/] */

#include "Funciones Auxiliares.h"
#define DIMLINEA 55

                /*  - / > [ Definicion de Funciones ] < / -  */

// Moddulo de Carga de Datos de Matriz
void cargarMatriz(vector<vector<double>> &distancias,const char *nombArch){
    // Apertura de Archivos & Declaracion de Variables
    ifstream arch = abrirArchivo_IFS(nombArch);
    double dist; int tamanho=0;
    // Descarte Inicial de Parametros de Archivo
    arch>>ws;
    while(arch.get() != '\n');
    arch>>ws;
    while(arch.get() != '\n');
    // Proceso de Carga de Datos
    while(1){
        arch>>ws;
        if(arch.eof()) break;                                                   // Validacion de Fin de Archivo
        vector<double> vDists;
        while(1){
            arch>>dist;
            vDists.push_back(dist);
            if(arch.get() == '\n') break;                                       // Validacion de Fin de Datos de Linea
        }
        // Validación para cada nodo
        if(tamanho != vDists.size()){
            if(tamanho == 0){
                tamanho = vDists.size();
            }
            else{
                cout<<"La matriz a cargar debe ser de una dimension 'n x n'"<<endl;
                exit(1);
            }
        }
        
        distancias.push_back(vDists);
    }
    // Validacion de Archivo Correcto
    if(distancias[0].size() != distancias.size()){
        cout<<"La matriz a cargar debe ser de una dimension 'n x n'"<<endl;
        exit(1);
    }
}
// Modulo de Carga de Datos de Cliente
void cargarClientes(vector<struct Cliente> &clientes,const char *nombArch){
    // Apertura de Arhivo & Declaracion de Variables
    ifstream arch = abrirArchivo_IFS(nombArch);
    int dni;
    // Proceso de Carga de Datos
    for(int i = 0;1;i++){
        arch>>dni;
        if(arch.eof()) break;                                                   // Validacion de Fin de Arhivo
        struct Cliente cli{};
        if(i == POSINICIO){                                                     // Validacion por Posicion de Inicio
            cli.id = -1;  // i
            clientes.push_back(cli);
            i++;
        }
        cli.id = i;
        arch>>cli.demanda>>cli.tiempo_servicio;
        cli.atendido = false;
        clientes.push_back(cli);
    }
}
// Modulo de Carga de Datos De Vehiculo
void cargarVehiculos(vector <struct DatosVehiculo> &vehiculos){
    // Proceso de Carga de Datos
    for(int i = 0;i < CANT_VECHICULOS;i++){
        struct DatosVehiculo vehiculo;
        vehiculo.capacidad_max = CARGAMAX;
        vehiculo.id = i;
        vehiculo.velocidad = VEL_PROM;
        vehiculos.push_back(vehiculo);
    }
}
// Modulo de Impresion de Datos de Solucion
void imprimirSolucion(const struct Solucion &solucion,
                      const char *nombArch){
    // Apertura de Archivo
    ofstream arch = abrirArchivo_OFS(nombArch);
    // Iterativa del Proceso de Impresion de Solucion
    for(int i = 0;i < solucion.solucionVehiculos.size();i++){
        struct Vehiculo vehiculo = solucion.solucionVehiculos[i];
        arch<<setw((DIMLINEA + 10)/2)<<"Vehiculo "<<vehiculo.datosVehiculo.id+1<<endl;
        imprimirLinea(arch,DIMLINEA,'-');
        arch<<setw(15)<<"Cap. Actual"<<setw(19)<<"Cap. Maxima";
        arch<<setw(17)<<"Velocidad"<<endl;
        arch<<setw(10)<<vehiculo.capacidad_actual;
        arch<<setw(20)<<vehiculo.datosVehiculo.capacidad_max<<setw(17)<<vehiculo.datosVehiculo.velocidad;
        arch<<endl<<endl<<setw((DIMLINEA + 16)/2)<<"RUTA DE CLIENTES"<<endl;
        imprimirLinea(arch,DIMLINEA,'-');
        // Iterativa del Proeso de Impresion de Ruta de Vehiculo
        for(int j = 0;j < vehiculo.ruta.size();j++){
            arch<<setw(24)<<j+1<<") "<<vehiculo.ruta[j]<<endl;
        }
        imprimirLinea(arch,DIMLINEA,'-');
        arch<<setw(28)<<"Tiempo total requerido: "<<vehiculo.tiempo_total<<endl;
        imprimirLinea(arch,DIMLINEA,'=');
    }
}
// Modulo de Apertura de Archivos IFSTREAM
ifstream abrirArchivo_IFS(const char *nombArch){
    ifstream arch(nombArch,ios::in);
    if(arch.is_open()) return arch;
    cout<<"ERROR DE APERTURA: Archivo "<<nombArch<<endl;
    exit(1);
}
// Modulo de Apertura de Archivos OFSTREAM
ofstream abrirArchivo_OFS(const char *nombArch){
    ofstream arch(nombArch,ios::out);
    if(arch.is_open()) return arch;
    cout<<"ERROR DE APERTURA: Archivo "<<nombArch<<endl;
    exit(1);
}
// Modulo de Impresion de Datos en Linea
void imprimirLinea(ofstream &arch,int dim,char simbolo){
    for(int i = 0;i < dim;i++) arch.put(simbolo);
    arch<<endl;
}