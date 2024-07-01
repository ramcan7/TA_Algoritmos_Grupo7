
/* [/]
 >> Project:    TA_Algoritmos_Avanzados_G7
 >> File:       Cliente.h
 >> Author:     Grupo 7
[/] */

#ifndef CLIENTE_H
#define CLIENTE_H

// Definicion de 'Cliente'
struct Cliente {
    int id;                 // Identificador del Cliente
    double demanda;         // Demanda o Carga que da el Cliente al Vehiculo
    double tiempo_servicio; // Tiempo de Atencion que toma el Cliente despues de llegar al Nodo [Esto es como un atributo a la realidad, pues los clientes no siempre se atienden al instante.]
    bool atendido;          // Indica si se un Vehiculo ha sido asignado al Cliente o no
};

#endif /* CLIENTE_H */
