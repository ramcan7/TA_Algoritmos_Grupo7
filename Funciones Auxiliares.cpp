
/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       Funciones Auxiliares.cpp
 >> Author:     Grupo 7
[/] */

#include "Funciones Auxiliares.h"
#define DIMLINEA 68

                /*  - / > [ Definicion de Funciones ] < / -  */

// Modulo de Carga de Distancias
void cargarMatriz(vector<vector<double>> &distancias,const char *nombArch){
    // Apertura de Archivos & Declaracion de Variables
    ifstream arch = abrirArchivo_IFS(nombArch);
    double dist; int dim = 0;
    // Descarte Inicial de Parametros de Archivo
    arch>>ws;
    while(arch.get() != '\n');
    arch>>ws;
    while(arch.get() != '\n');
    // Proceso de Carga de Datos
    while(1){
        arch>>ws;
        if(arch.eof()) break;                                                   // Validacion de Fin de Archivo
        // Carga de Datos de Fila
        vector<double> vDists;
        while(1){
            arch>>dist;
            vDists.push_back(dist);
            if(arch.get() == '\n') break;                                       // Validacion de Fin de Datos de Fila
        }
        // Validación de dimension regular de matriz
        if(dim != vDists.size()){
            if(dim != 0){
                cout<<"La matriz a cargar debe ser de una dimension 'n x n'";
                cout<<endl;
                exit(1); 
            } else dim = vDists.size();
        }
        // Insercion de Nueva Fila
        distancias.push_back(vDists);
    }
}
// Modulo de Carga de Datos de 'Cliente'
void cargarClientes(vector<struct Cliente> &clientes,const char *nombArch){
    // Apertura de Arhivo & Declaracion de Variables
    ifstream arch = abrirArchivo_IFS(nombArch);
    int dni;
    // Proceso de Carga de Datos
    for(int i = 0;1;i++){
        arch>>dni;
        if(arch.eof()) break;                                                   // Validacion de Fin de Arhivo
        struct Cliente cliente{};
        if(i == POSINICIO){                                                     // Validacion por Posicion de Inicio
            cliente.id = -1;  // i
            clientes.push_back(cliente);
            i++;
        }
        cliente.id = i;
        arch>>cliente.demanda>>cliente.tiempo_servicio;
        clientes.push_back(cliente);
    }
}
// Modulo de Carga de Datos De 'Vehiculo'
void cargarVehiculos(vector <struct Vehiculo> &vehiculos){
    // Proceso de Carga de Datos
    for(int i = 0;i < CANT_VECHICULOS;i++){
        struct Vehiculo vehiculo;
        vehiculo.id = i;
        vehiculo.velocidad = VEL_PROM;
        vehiculo.capacidad_max = MAX_CARGA;
        vehiculo.capacidad_actual = 0;
        vehiculo.tiempo_total = 0;
        vehiculo.distancia_total = 0;
        vehiculos.push_back(vehiculo);
    }
}
// Modulo de Impresion de 'Solucion'
void imprimirSolucion(struct Problema &problemita, struct Solucion& solucion,
                      const char *nombArch){
    // Apertura de Archivo
    ofstream arch = abrirArchivo_OFS(nombArch);
    // Inicializacion de Elementos
    arch<<fixed<<setprecision(2);
    // Proceso de Impresion de la Solucion
    imprimirLinea(arch,DIMLINEA,'=');
    arch<<setw((DIMLINEA + 22)/2)<<"FITNESS DE LA SOLUCION"<<endl;
    arch<<setw((DIMLINEA + 7)/2)<<solucion.fitness<<endl;
    imprimirLinea(arch,DIMLINEA,'=');
    for(int i = 0;i < solucion.vehiculos.size();i++){
        struct Vehiculo vehiculo = solucion.vehiculos[i];
        arch<<setw((DIMLINEA + 8)/2)<<"Vehiculo #";
        arch<<setfill('0')<<setw(2)<<vehiculo.id+1<<setfill(' ')<<endl;
        imprimirLinea(arch,DIMLINEA,'-');
        imprimirEncabezado(arch,DIMLINEA,'A');
        arch<<setw(13)<<hallarCarga_Ruta(problemita.clientes,vehiculo.ruta);
        arch<<setw(15)<<vehiculo.capacidad_max<<setw(12)<<vehiculo.velocidad;
        arch<<setw(15)<<vehiculo.ruta.size()<<endl;
        imprimirRuta(arch,problemita,vehiculo);
        imprimirEstadisticasDeRuta(arch,problemita,vehiculo);
        imprimirLinea(arch,DIMLINEA,'=');
    }
}
// Modulo de Impresion de Ruta Seguida por Vehiculo
void imprimirRuta(ofstream &arch,struct Problema &problemita,
                  struct Vehiculo &vehiculo){
    // Declaracion de Variables
    int i = 0;
    // Proceso de Impresion
    imprimirEncabezado(arch,DIMLINEA,'B');
    arch<<setw(18)<<"P. INICIO   -->"<<setw(9)<<vehiculo.ruta[0];
    arch<<setw(17)<<hallarDistancia(problemita.distancias,POSINICIO,
                                    vehiculo.ruta[0]);
    arch<<setw(16)<<problemita.clientes[vehiculo.ruta[0]].demanda<<endl;
    while(i < vehiculo.ruta.size() - 1){
        arch<<setw(9)<<vehiculo.ruta[i]<<setw(9)<<"-->";
        arch<<setw(9)<<vehiculo.ruta[i+1];
        arch<<setw(17)<<hallarDistancia(problemita.distancias,vehiculo.ruta[i],
                                        vehiculo.ruta[i+1]);
        arch<<setw(16)<<problemita.clientes[vehiculo.ruta[i+1]].demanda<<endl;
        i++;
    }
    arch<<setw(9)<<vehiculo.ruta[i]<<setw(9)<<"-->"<<setw(12)<<"P. INICIO";
    arch<<setw(14)<<hallarDistancia(problemita.distancias,vehiculo.ruta[i],
                                    POSINICIO);
    arch<<setw(16)<<"- - -"<<endl;
}
// Modulo de Impresion de Resumen de Estadisticas de Ruta Seguida por Vehiculo
void imprimirEstadisticasDeRuta(ofstream &arch,struct Problema &problemita,
                                struct Vehiculo &vehiculo){
    imprimirLinea(arch,DIMLINEA,'-');
    arch<<setw(31)<<"Distancia total a recorrer:";
    arch<<setw(13)<<hallarDistancia_Ruta(problemita.distancias,vehiculo.ruta);
    arch<<endl<<setw(30)<<"Tiempo por Servicio total:";
    arch<<setw(14)<<hallarTiempoPorServicio_Ruta(problemita.clientes,
                                                 vehiculo.ruta);
    arch<<endl<<setw(36)<<"Tiempo total requerido por ruta:";
    arch<<setw(8)<<hallarTiempoTotal_Ruta(problemita.distancias,
                                           problemita.clientes,vehiculo.ruta);
    arch<<endl;
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
// Modulo de Impresion de Encabezados de Reporte Solucion
void imprimirEncabezado(ofstream &arch,int dim,char seleccion){
    if(seleccion == 'A'){
        arch<<setw(15)<<"Cap. Actual"<<setw(15)<<"Cap. Maxima";
        arch<<setw(13)<<"Velocidad"<<setw(18)<<"Num. Atendidos"<<endl;
    } else if(seleccion == 'B'){
        imprimirLinea(arch,DIMLINEA,'-');
        arch<<setw((DIMLINEA + 11)/2)<<"> R U T A <"<<endl;
        imprimirLinea(arch,DIMLINEA,'-');
        arch<<setw(29)<<"PARTIDA    -->    LLEGADA";
        arch<<setw(17)<<"DISTANCIA"<<setw(18)<<"CAP. DEMANDADA"<<endl;
        imprimirLinea(arch,DIMLINEA,'-');
    } else {
        
    }
}
