#include "kronos.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LASERS 100
#define MAX_ROBOTS 10
#define MAX_PERSONAJES 10
#define MAX_PINZAS 100
#define MAX_SUPERTRAJES 10

#define MAX_FILAS 20
#define MAX_COLUMNAS 20
#define MAX_CUADRANTES 4

const int MIN_COLUMNAS = 0;
const int MIN_FILAS = 0;
const int amplitud_matriz = 20;
const int valor_minimo_fila [MAX_CUADRANTES] = {0,10,10,0};
const int valor_minimo_columna [MAX_CUADRANTES] = {0,10,0,10};

const int max_movimientos_pj[MAX_PERSONAJES] = {250, 300, 200, 150};

const char movimiento_arriba = 'W';
const char movimiento_abajo = 'S';
const char movimiento_derecha = 'D';
const char movimiento_izquierda = 'A';
const char movimiento_poder = 'C';

const char salida = 'S';
const char ESPACIO_VACIO = ' ';
const char pj_elastic = 'E';
const char pj_violeta = 'V';
const char pj_dash = 'D';
const char pj_increible = 'I';
const char obst_robot = 'R';
const char obst_pinza = 'P';
const char obst_laser = 'L';
const char obj_supertraje = 'T';
const char convencion_pj[MAX_PERSONAJES] = {pj_elastic, pj_violeta, pj_dash, pj_increible};
const int cant_lasers_robot = 2;

const int juego_en_curso = 0;
const int juego_perdido = -1;
const int juego_ganado = 1;

const int max_pinzas_cuadrante = 4;


//Pre: -
//Pos: Devuelve un numero aleatorio que cumple con: [valor minimo - 10 >= valor minimo <= valor minimo + 10]
int posicion_aleatoria(int valor_minimo){

    return rand () % (amplitud_matriz / 2) + valor_minimo;
}

//Pre: -
//Pos: Asigna posiciones aleatorias a las pinzas, y setea el tope.
void asignar_posicion_pinzas(juego_t* juego){

    int cuadrante_actual = 0;
    for(int pinza_actual = 0; pinza_actual < (max_pinzas_cuadrante * MAX_CUADRANTES); pinza_actual++){

        juego->pinzas[pinza_actual].columna = posicion_aleatoria(valor_minimo_columna[cuadrante_actual]);
        juego->pinzas[pinza_actual].fila = posicion_aleatoria(valor_minimo_fila[cuadrante_actual]);
        juego->tope_pinzas++;

        if((pinza_actual % max_pinzas_cuadrante) == 0){                                                                      // Puede haber 4 pinzas como máximo en cada cuadrante
            cuadrante_actual++;
        }
    }
}

//Pre: -
//Pos: Asigna las posiciones de los laseres, y setea el tope.
void asignar_posicion_laseres(juego_t* juego){

    int cuadrante_actual = 0;
    while(cuadrante_actual < MAX_CUADRANTES){

        for(int long_col_laser = 1; long_col_laser <= juego->longitud_laser; long_col_laser++){
            juego->robots->lasers[juego->robots->tope_lasers].columna = juego->robots[cuadrante_actual].posicion.columna + long_col_laser;
            juego->robots->lasers[juego->robots->tope_lasers].fila = juego->robots[cuadrante_actual].posicion.fila;
            juego->robots->tope_lasers++;
        }
        for(int long_fila_laser = 1; long_fila_laser <= juego->longitud_laser; long_fila_laser++){
            juego->robots->lasers[juego->robots->tope_lasers].fila = juego->robots[cuadrante_actual].posicion.fila + long_fila_laser;
            juego->robots->lasers[juego->robots->tope_lasers].columna = juego->robots[cuadrante_actual].posicion.columna;
            juego->robots->tope_lasers++;
        }
        cuadrante_actual++;
    }
}

//Pre: -
//Pos: Asigna posiciones aleatorias a los objetos unicos (robots, supertrajes y personajes), y setea sus respectivos topes.
void asignar_posicion_unicos(juego_t* juego){

    for(int cuadrante_actual = 0; cuadrante_actual < MAX_CUADRANTES; cuadrante_actual++){

        juego->robots[cuadrante_actual].posicion.fila = posicion_aleatoria(valor_minimo_fila[cuadrante_actual]);
        juego->robots[cuadrante_actual].posicion.columna = posicion_aleatoria(valor_minimo_columna[cuadrante_actual]);
        juego->tope_robots++;

        juego->supertrajes[cuadrante_actual].posicion.fila = posicion_aleatoria(valor_minimo_fila[cuadrante_actual]);
        juego->supertrajes[cuadrante_actual].posicion.columna = posicion_aleatoria(valor_minimo_columna[cuadrante_actual]);
        juego->tope_supertraje++;

        juego->personajes[cuadrante_actual].posicion.fila = posicion_aleatoria(valor_minimo_fila[cuadrante_actual]);
        juego->personajes[cuadrante_actual].posicion.columna = posicion_aleatoria(valor_minimo_columna[cuadrante_actual]);
        juego->tope_personajes++;
    }
}


//Pre: 0 <= laser actual < MAX_LASERS
//Pos: Identifica si algún laser esta en la posicion 1 y posteriormente lo traslada 45 grados en sentido horario con un eje fijo al robot.
void identificar_posi1_laser(juego_t* juego, int laser_actual, int longitud_actual, int cuadrante_actual, int* ya_rotado){

    if(juego->robots->lasers[laser_actual].fila == juego->robots[cuadrante_actual].posicion.fila
    && juego->robots->lasers[laser_actual].columna == juego->robots[cuadrante_actual].posicion.columna + longitud_actual && *ya_rotado == 0){           // posicion 1:     R  L

        *ya_rotado = 1;
        juego->robots->lasers[laser_actual].fila += longitud_actual;
    }
}
//Pre: 0 <= laser actual < MAX_LASERS
//Pos: Identifica si algún laser esta en la posicion 2 y posteriormente lo traslada 45 grados en sentido horario con un eje fijo al robot.
void identificar_posi2_laser(juego_t* juego, int laser_actual, int longitud_actual, int cuadrante_actual, int* ya_rotado){

    if(juego->robots->lasers[laser_actual].fila == juego->robots[cuadrante_actual].posicion.fila + longitud_actual                                      // posicion 2:     R
    && juego->robots->lasers[laser_actual].columna == juego->robots[cuadrante_actual].posicion.columna + longitud_actual && *ya_rotado == 0){           //                    L
                                                                                                                                                        
        *ya_rotado = 1;
        juego->robots->lasers[laser_actual].columna -= longitud_actual;
    }
}
//Pre: 0 <= laser actual < MAX_LASERS
//Pos: Identifica si algún laser esta en la posicion 3 y posteriormente lo traslada 45 grados en sentido horario con un eje fijo al robot.
void identificar_posi3_laser(juego_t* juego, int laser_actual, int longitud_actual, int cuadrante_actual, int* ya_rotado){

    if(juego->robots->lasers[laser_actual].fila == juego->robots[cuadrante_actual].posicion.fila + longitud_actual                                      // posicion 3:       R
    && juego->robots->lasers[laser_actual].columna == juego->robots[cuadrante_actual].posicion.columna && *ya_rotado == 0){                             //                   L

        *ya_rotado = 1;
        juego->robots->lasers[laser_actual].columna -= longitud_actual;
    }
}
//Pre: 0 <= laser actual < MAX_LASERS
//Pos: Identifica si algún laser esta en la posicion 4 y posteriormente lo traslada 45 grados en sentido horario con un eje fijo al robot.
void identificar_posi4_laser(juego_t* juego, int laser_actual, int longitud_actual, int cuadrante_actual, int* ya_rotado){

    if(juego->robots->lasers[laser_actual].fila == juego->robots[cuadrante_actual].posicion.fila + longitud_actual                                      // posicion 4:       R
    && juego->robots->lasers[laser_actual].columna == juego->robots[cuadrante_actual].posicion.columna - longitud_actual && *ya_rotado == 0){           //                 L

        *ya_rotado = 1;
        juego->robots->lasers[laser_actual].fila -= longitud_actual;
    }
}
//Pre: 0 <= laser actual < MAX_LASERS
//Pos: Identifica si algún laser esta en la posicion 5 y posteriormente lo traslada 45 grados en sentido horario con un eje fijo al robot.
void identificar_posi5_laser(juego_t* juego, int laser_actual, int longitud_actual, int cuadrante_actual, int* ya_rotado){

    if(juego->robots->lasers[laser_actual].fila == juego->robots[cuadrante_actual].posicion.fila                                                         // posicion 5:     L  R 
    && juego->robots->lasers[laser_actual].columna == juego->robots[cuadrante_actual].posicion.columna - longitud_actual && *ya_rotado == 0){

        *ya_rotado = 1;
        juego->robots->lasers[laser_actual].fila -= longitud_actual;
    }
}
//Pre: 0 <= laser actual < MAX_LASERS
//Pos: Identifica si algún laser esta en la posicion 6 y posteriormente lo traslada 45 grados en sentido horario con un eje fijo al robot.
void identificar_posi6_laser(juego_t* juego, int laser_actual, int longitud_actual, int cuadrante_actual, int* ya_rotado){

    if(juego->robots->lasers[laser_actual].fila == juego->robots[cuadrante_actual].posicion.fila - longitud_actual                                        //                 L
    && juego->robots->lasers[laser_actual].columna == juego->robots[cuadrante_actual].posicion.columna - longitud_actual && *ya_rotado == 0){             // posicion 6:       R

        *ya_rotado = 1;
        juego->robots->lasers[laser_actual].columna += longitud_actual;
    }
}
//Pre: 0 <= laser actual < MAX_LASERS
//Pos: Identifica si algún laser esta en la posicion 7 y posteriormente lo traslada 45 grados en sentido horario con un eje fijo al robot.
void identificar_posi7_laser(juego_t* juego, int laser_actual, int longitud_actual, int cuadrante_actual, int* ya_rotado){

    if(juego->robots->lasers[laser_actual].fila == juego->robots[cuadrante_actual].posicion.fila - longitud_actual                                          //                 L
    && juego->robots->lasers[laser_actual].columna == juego->robots[cuadrante_actual].posicion.columna && *ya_rotado == 0){                                 // posicion 7:     R

        *ya_rotado = 1;
        juego->robots->lasers[laser_actual].columna += longitud_actual;
    }
}

//Pre: 0 <= laser actual < MAX_LASERS
//Pos: Identifica si algún laser esta en la posicion 8 y posteriormente lo traslada 45 grados en sentido horario con un eje fijo al robot.
void identificar_posi8_laser(juego_t* juego, int laser_actual, int longitud_actual, int cuadrante_actual, int* ya_rotado){

    if(juego->robots->lasers[laser_actual].fila == juego->robots[cuadrante_actual].posicion.fila - longitud_actual                                          //                    L
    && juego->robots->lasers[laser_actual].columna == juego->robots[cuadrante_actual].posicion.columna + longitud_actual && *ya_rotado == 0){               // posicion 8:      R

        *ya_rotado = 1;
        juego->robots->lasers[laser_actual].fila += longitud_actual;
    }
}
//Pre: -
//Pos: Rota los láseres 45 grados en sentido horario desde un eje fijo al robot
void rotar_laseress(juego_t* juego){

    int ya_rotado = 0;
    int cuadrante_actual = 0;
    int max_laseres_cuadrante = juego->robots->tope_lasers / MAX_CUADRANTES;

    for(int laser_actual = 0; laser_actual < juego->robots->tope_lasers; laser_actual++){
        for(int longitud_actual = 1; longitud_actual <= juego->longitud_laser; longitud_actual++){

            identificar_posi1_laser(juego, laser_actual, longitud_actual, cuadrante_actual, &ya_rotado);
            identificar_posi2_laser(juego, laser_actual, longitud_actual, cuadrante_actual, &ya_rotado);
            identificar_posi3_laser(juego, laser_actual, longitud_actual, cuadrante_actual, &ya_rotado);
            identificar_posi4_laser(juego, laser_actual, longitud_actual, cuadrante_actual, &ya_rotado);
            identificar_posi5_laser(juego, laser_actual, longitud_actual, cuadrante_actual, &ya_rotado);
            identificar_posi6_laser(juego, laser_actual, longitud_actual, cuadrante_actual, &ya_rotado);
            identificar_posi7_laser(juego, laser_actual, longitud_actual, cuadrante_actual, &ya_rotado);
            identificar_posi8_laser(juego, laser_actual, longitud_actual, cuadrante_actual, &ya_rotado);
        }
        if(laser_actual == max_laseres_cuadrante -1 || laser_actual == (max_laseres_cuadrante * 2) -1                                   //Cada robot tiene max_laseres_cuadrante
        || laser_actual == (max_laseres_cuadrante *3) -1|| laser_actual == (max_laseres_cuadrante *4) -1){

            cuadrante_actual++;
        }
    ya_rotado = 0;
    }
}


//Pre: cuadrante actual < MAX_CUADRANTES
//Pos: Devuelve un nuevo valor aleatorio a fila y a columna.
void cambiar_lugar(int* fila, int* columna, int cuadrante_actual){

    *fila = posicion_aleatoria(valor_minimo_fila[cuadrante_actual]);
    *columna = posicion_aleatoria(valor_minimo_columna[cuadrante_actual]);
}

//Pre: -
//Pos: Devuelve true si la fila de algún laser coincide con fila, y simúltaneamente coincide la columna del mismo láser con columna, false en cualquier otro caso.
bool esta_rango_laser(juego_t* juego, int fila, int columna){

    int laser_actual = 0;
    bool superpuesto = false;
    while(superpuesto == false && laser_actual < juego->robots->tope_lasers){

        if(fila == juego->robots->lasers[laser_actual].fila && columna == juego->robots->lasers[laser_actual].columna){

            superpuesto = true;
        }
        laser_actual++;
    }
return superpuesto;
}

//Pre: -
//Pos: Detecta y cambia de lugares aquellos objetos únicos coincidentes en posición con cualquier láser
void separar_unicos_lasers(juego_t* juego){

    int posicion_repetida;
    int cuadrante_actual = 0;
    while(cuadrante_actual < MAX_CUADRANTES){

        posicion_repetida = 0;
        if(esta_rango_laser(juego, juego->personajes[cuadrante_actual].posicion.fila, juego->personajes[cuadrante_actual].posicion.columna)){

            posicion_repetida++;
            cambiar_lugar(&(juego->personajes[cuadrante_actual].posicion.fila), &(juego->personajes[cuadrante_actual].posicion.columna), cuadrante_actual);
        }
        if(esta_rango_laser(juego, juego->supertrajes[cuadrante_actual].posicion.fila, juego->supertrajes[cuadrante_actual].posicion.columna)){

            posicion_repetida++;
            cambiar_lugar(&(juego->supertrajes[cuadrante_actual].posicion.fila), &(juego->supertrajes[cuadrante_actual].posicion.columna), cuadrante_actual);
        }
        if(posicion_repetida == 0){
            cuadrante_actual++;
        }
    }
}

//Pre: -
//Pos: Devuelve true si fila = MIN_FILAS y columna == MAX_COLUMNAS -1.
bool esta_en_salida(int fila, int columna){

    return (fila == MIN_FILAS && columna == MAX_COLUMNAS -1);
}

//Pre: -
//Pos: Les asigna nuevas posiciones a los objetos unicos hasta que esten separados entre si.
void separar_unicos(juego_t* juego){

    int cuadrante_actual = 0;
    int posicion_repetida;

    if(esta_en_salida(juego->personajes[3].posicion.fila, juego->personajes[3].posicion.columna)){

        cambiar_lugar(&(juego->personajes[3].posicion.fila), &(juego->personajes[3].posicion.columna), 3);
    }
    while(cuadrante_actual < MAX_CUADRANTES){
        posicion_repetida = 0;

        if((juego->personajes[cuadrante_actual].posicion.fila == juego->supertrajes[cuadrante_actual].posicion.fila
        && juego->personajes[cuadrante_actual].posicion.columna == juego->supertrajes[cuadrante_actual].posicion.columna)){

            posicion_repetida++;
            cambiar_lugar(&(juego->supertrajes[cuadrante_actual].posicion.fila), &(juego->supertrajes[cuadrante_actual].posicion.columna), cuadrante_actual);

        }
        if(juego->personajes[cuadrante_actual].posicion.fila == juego->robots[cuadrante_actual].posicion.fila
        && juego->personajes[cuadrante_actual].posicion.columna == juego->robots[cuadrante_actual].posicion.columna){

            posicion_repetida++;
            cambiar_lugar(&(juego->personajes[cuadrante_actual].posicion.fila), &(juego->personajes[cuadrante_actual].posicion.columna), cuadrante_actual);

        }
        if(juego->robots[cuadrante_actual].posicion.fila == juego->supertrajes[cuadrante_actual].posicion.fila
        && juego->robots[cuadrante_actual].posicion.columna == juego->supertrajes[cuadrante_actual].posicion.columna){

            posicion_repetida++;
            cambiar_lugar(&(juego->supertrajes[cuadrante_actual].posicion.fila), &(juego->supertrajes[cuadrante_actual].posicion.columna), cuadrante_actual);

        }
        if(posicion_repetida == 0){
            cuadrante_actual++;
        }
    }
    separar_unicos_lasers(juego);
}


//Pre: -
//Pos: Asigna el resto de valores del registro, es decir todos los demás campos menos las posiciones.
void asignar_campos(juego_t* juego){

    for (int cuadrante_actual = 0; cuadrante_actual < MAX_CUADRANTES; cuadrante_actual++){

        juego->personajes[cuadrante_actual].movimientos = max_movimientos_pj[cuadrante_actual];
        juego->personajes[cuadrante_actual].cuadrante_inicial = cuadrante_actual + 1;
        juego->personajes[cuadrante_actual].movimientos_con_poder = 5;
        juego->personajes[cuadrante_actual].tiene_supertraje = false;
        juego->personajes[cuadrante_actual].poder_activado = false;

        juego->supertrajes[cuadrante_actual].recolectado = false;
        juego->supertrajes[cuadrante_actual].usado = false;
    }
}

//Pre: -
//Pos: Detecta y cambia de lugar a aquellas pinzas que sean coincidentes con cualquier otro elemento en el juego.
void separar_pinzas(juego_t* juego){

    int posicion_repetida, pinza_actual = 0, cuadrante_actual = 0;
    while(pinza_actual < juego->tope_pinzas){
        posicion_repetida = 0;

        if(juego->pinzas[pinza_actual].fila == juego->personajes[cuadrante_actual].posicion.fila
        && juego->pinzas[pinza_actual].columna == juego->personajes[cuadrante_actual].posicion.columna){

            posicion_repetida++;
            cambiar_lugar(&(juego->pinzas[pinza_actual].fila), &(juego->pinzas[pinza_actual].columna), cuadrante_actual);
        }
        if(juego->pinzas[pinza_actual].fila == juego->supertrajes[cuadrante_actual].posicion.fila
        && juego->pinzas[pinza_actual].columna == juego->supertrajes[cuadrante_actual].posicion.columna){

            posicion_repetida++;
            cambiar_lugar(&(juego->pinzas[pinza_actual].fila), &(juego->pinzas[pinza_actual].columna), cuadrante_actual);
        }
        if(juego->pinzas[pinza_actual].fila == juego->robots[cuadrante_actual].posicion.fila
        && juego->pinzas[pinza_actual].columna == juego->robots[cuadrante_actual].posicion.columna){

            posicion_repetida++;
            cambiar_lugar(&(juego->pinzas[pinza_actual].fila), &(juego->pinzas[pinza_actual].columna), cuadrante_actual);
        }
        if(juego->pinzas[pinza_actual].fila == juego->pinzas[pinza_actual +1].fila
        && juego->pinzas[pinza_actual].columna == juego->pinzas[pinza_actual +1].columna && pinza_actual < (juego->tope_pinzas)){

            posicion_repetida++;
            cambiar_lugar(&(juego->pinzas[pinza_actual].fila), &(juego->pinzas[pinza_actual].columna), cuadrante_actual);
        }
        if(esta_rango_laser(juego, juego->pinzas[pinza_actual].fila, juego->pinzas[pinza_actual].columna)){

            posicion_repetida++;
            cambiar_lugar(&(juego->pinzas[pinza_actual].fila), &(juego->pinzas[pinza_actual].columna), cuadrante_actual);
        }
        if(esta_en_salida(juego->pinzas[pinza_actual].fila, juego->pinzas[pinza_actual].columna)){

            posicion_repetida++;
            cambiar_lugar(&(juego->pinzas[pinza_actual].fila), &(juego->pinzas[pinza_actual].columna), cuadrante_actual);
        }
        if(posicion_repetida == 0){
            pinza_actual++;
        }
        if(posicion_repetida == 0 && pinza_actual % 4 == 0){
            cuadrante_actual++;
        }
    }
}

//Pre: -
//Pos: Inicializa ciertos valores necesarios para continuar.
void inicializar_info(juego_t* juego, bool contrasenia_completa){

    juego->id_personaje_actual = 0;

    if(contrasenia_completa == false){
        juego->longitud_laser = 5;
    }
        else{
            juego->longitud_laser = 3;
        }

    juego->tope_personajes = 0;
    juego->tope_pinzas = 0;
    juego->tope_robots = 0;
    juego->tope_supertraje = 0;
    juego->robots->tope_lasers = 0;
    juego->tope_drons = 0;
    juego->dron.destruido = true;

    asignar_campos(juego);
}

void inicializar_juego(juego_t* juego, bool contrasenia_completa){

    inicializar_info(juego, contrasenia_completa);
    asignar_posicion_unicos(juego);
    asignar_posicion_pinzas(juego);
    asignar_posicion_laseres(juego);

    separar_unicos(juego);
    separar_pinzas(juego);
}

//Pre: -
//Pos: Devuelve true si la fila de alguna pinza es igual a fila_actual,
// y simúltaneamente la columna de esa misma pinza es igual a columna_actual, false en cualquier otro caso
bool es_posicion_pinza(juego_t juego, int fila_actual, int columna_actual){

    int pinza_actual = 0;
    bool es_posicion_actual = false;

    while(es_posicion_actual == false && pinza_actual < juego.tope_pinzas){

        if(fila_actual == juego.pinzas[pinza_actual].fila && columna_actual == juego.pinzas[pinza_actual].columna){

            es_posicion_actual = true;
        }
        pinza_actual++;
    }
return es_posicion_actual;
}


//Pre: -
//Pos: Devuelve true si la fila de algún supertraje es igual a fila_actual,
// y la columna de ese mismo es igual a columna_actual, simúltaneamente su valor recolectado debe ser false, false en cualquier otro caso
bool es_posicion_traje(juego_t juego, int fila_actual, int columna_actual){

    int traje_actual = 0;
    bool es_traje_actual = false;

    while(es_traje_actual == false && traje_actual < juego.tope_supertraje){

        if(fila_actual == juego.supertrajes[traje_actual].posicion.fila && columna_actual == juego.supertrajes[traje_actual].posicion.columna
        && juego.supertrajes[traje_actual].recolectado == false){

            es_traje_actual = true;
        }
        traje_actual++;
    }
return es_traje_actual;
}

//Pre: -
//Pos: Devuelve true si la fila de algún robot es igual a fila_actual,
// y simúltaneamente la columna de ese mismo robot es igual a columna_actual, false en cualquier otro caso
bool es_posicion_robot(juego_t juego, int fila_actual, int columna_actual){

    int traje_actual = 0;
    bool es_robot_actual = false;

    while(es_robot_actual == false && traje_actual < juego.tope_robots){

        if(fila_actual == juego.robots[traje_actual].posicion.fila && columna_actual == juego.robots[traje_actual].posicion.columna){

            es_robot_actual = true;
        }
        traje_actual++;
    }
return es_robot_actual;
}

//Pre: -
//Pos: Devuelve true si la fila de algún laser es igual a fila_actual,
// y simúltaneamente la columna de ese mismo laser es igual a columna_actual, false en cualquier otro caso
bool es_posicion_laser(juego_t juego, int fila_actual, int columna_actual){

    int laser_actual = 0;
    bool es_laser_actual = false;

    while(es_laser_actual == false && laser_actual < juego.robots->tope_lasers){

        if(fila_actual == juego.robots->lasers[laser_actual].fila && columna_actual == juego.robots->lasers[laser_actual].columna){

            es_laser_actual = true;
        }
        laser_actual++;
    }
return es_laser_actual;
}

//Pre: id_pj_actual < 4.
//Pos: Devuelve true si la fila de personaje en la posición id_pj_actual es igual a fila_actual,
// y simúltaneamente la columna de ese mismo personaje es igual a columna_actual, false en cualquier otro caso
bool es_posicion_personaje(juego_t juego, int id_pj_actual, int fila_actual, int columna_actual){

    bool es_posicion_pj = false;

    if(fila_actual == juego.personajes[id_pj_actual].posicion.fila
    && columna_actual == juego.personajes[id_pj_actual].posicion.columna){

        es_posicion_pj = true;
    }
return es_posicion_pj;
}

bool es_posicion_dron(juego_t juego, int fila_actual, int columna_actual){

    bool es_posicion_dron = false;

    if(fila_actual == juego.dron.posicion.fila && columna_actual == juego.dron.posicion.columna){

        es_posicion_dron = true;
    }
return es_posicion_dron;
}

//Pre: (juego.tope_pinzas, juego.tope_robots, juego.tope_supertrajes, juego.robots.tope_lasers) > 0
//Pos: Asigna todas las posiciones de juego y sus respectivas convenciones dentro de la matriz terreno_juego
void insertar_posiciones(juego_t juego, char terreno_juego[MAX_FILAS][MAX_COLUMNAS]){

    for(int fila_actual = 0; fila_actual < MAX_FILAS; fila_actual++){
        for(int columna_actual = 0; columna_actual < MAX_COLUMNAS; columna_actual++){

            terreno_juego[fila_actual][columna_actual] = ESPACIO_VACIO;

            if(es_posicion_pinza(juego, fila_actual, columna_actual)){

                terreno_juego[fila_actual][columna_actual] = obst_pinza;
            }
            if(es_posicion_robot(juego, fila_actual, columna_actual)){

                terreno_juego[fila_actual][columna_actual] = obst_robot;
            }
            if(es_posicion_traje(juego, fila_actual, columna_actual)){
                
                terreno_juego[fila_actual][columna_actual] = obj_supertraje;
            }
            if(es_posicion_laser(juego, fila_actual, columna_actual)){

                terreno_juego[fila_actual][columna_actual] = obst_laser;
            }
            if(es_posicion_dron(juego, fila_actual, columna_actual) && juego.dron.destruido == false){

                terreno_juego[fila_actual][columna_actual] = 'J';
            }
            if(es_posicion_personaje(juego, juego.id_personaje_actual, fila_actual, columna_actual)){

                terreno_juego[fila_actual][columna_actual] = convencion_pj[juego.id_personaje_actual];
            }
            if(es_posicion_personaje(juego, juego.id_personaje_actual + 1, fila_actual, columna_actual) && juego.id_personaje_actual < 3){

                terreno_juego[fila_actual][columna_actual] = convencion_pj[juego.id_personaje_actual +1];
            }
                else if(juego.id_personaje_actual > 3){

                    terreno_juego[fila_actual][columna_actual] = ESPACIO_VACIO;
                }
        }
    }
    terreno_juego[MIN_FILAS][MAX_COLUMNAS -1] = salida;
}

//Pre: -
//Pos: Muestra información potencialemente útil al usuario
void mostrar_informacion(juego_t juego){

    printf("Cantidad de movimientos restantes: %i\n", juego.personajes[juego.id_personaje_actual].movimientos);
    
    if(juego.id_personaje_actual == 0 || juego.id_personaje_actual == 1 || juego.id_personaje_actual == 2){

        printf("El personaje que estas controlando es %c y tenes que buscar a %c\n",convencion_pj[juego.id_personaje_actual],convencion_pj[juego.id_personaje_actual +1]);
    }
        else if(juego.id_personaje_actual >= 3){

            printf("Rápido, dirigite a %c, es la salida\n", salida);
        }
    if(juego.personajes[juego.id_personaje_actual].tiene_supertraje == true && juego.supertrajes[juego.id_personaje_actual].usado == false){

        printf("Tenes en tu poder el supertraje listo para usarse\n");
    }
         else if(juego.personajes[juego.id_personaje_actual].tiene_supertraje == false){

            printf("No tenes el supertraje\n");
        }
    if(juego.supertrajes[juego.id_personaje_actual].usado && juego.personajes[juego.id_personaje_actual].movimientos_con_poder < 0){

        printf("Ya usaste el supertraje de este personaje :(\n");
    }
    if(juego.personajes[juego.id_personaje_actual].poder_activado == true){

        printf("Tu poder se acabará en %i turnos\n", juego.personajes[juego.id_personaje_actual].movimientos_con_poder +1);
    }
}


void imprimir_terreno(juego_t juego){

    char terreno_juego [MAX_FILAS][MAX_COLUMNAS];

    insertar_posiciones(juego, terreno_juego);

    for(int fila_actual = 0; fila_actual < MAX_FILAS; fila_actual++){
      for(int columna_actual = 0; columna_actual < MAX_COLUMNAS; columna_actual++){

        printf("_%c_|", terreno_juego[fila_actual][columna_actual]);     
        }
    printf("\n");
    }
    
    mostrar_informacion(juego);
}

//Pre: -
//Pos: Devuelve true si el movimiento es valido (W,A,S,D o C), false en cualquier otro caso
bool es_movimiento_valido(char movimiento){

    bool movimiento_valido = true;
    
    if(movimiento != movimiento_arriba && movimiento != movimiento_abajo && movimiento != movimiento_derecha
        && movimiento != movimiento_izquierda && movimiento != movimiento_poder){

            movimiento_valido = false;
    }

return movimiento_valido;

}

//Pre: -
//Pos: Realiza las acciones de movimiento del personaje.
void movimiento_pj(juego_t* juego, char* movimiento){

        if(*movimiento == movimiento_arriba){

            juego->personajes[juego->id_personaje_actual].posicion.fila -= 1;
            juego->personajes[juego->id_personaje_actual].movimientos--;
        }
        if(*movimiento == movimiento_abajo){

            juego->personajes[juego->id_personaje_actual].posicion.fila += 1;
            juego->personajes[juego->id_personaje_actual].movimientos--;
        }
        if(*movimiento == movimiento_izquierda){

            juego->personajes[juego->id_personaje_actual].posicion.columna -= 1;
            juego->personajes[juego->id_personaje_actual].movimientos--;
        }
        if(*movimiento == movimiento_derecha){

            juego->personajes[juego->id_personaje_actual].posicion.columna += 1;
            juego->personajes[juego->id_personaje_actual].movimientos--;
        }
        if(*movimiento == movimiento_poder && juego->supertrajes[juego->id_personaje_actual].recolectado == true
        && juego->supertrajes[juego->id_personaje_actual].usado == false){
            
            juego->supertrajes[juego->id_personaje_actual].usado = true;
            juego->personajes[juego->id_personaje_actual].poder_activado = true;
            
        }
        if(*movimiento == movimiento_poder && juego->supertrajes[juego->id_personaje_actual].recolectado == false){

            printf("Primero tienes que recoger el supertraje\n");
        }
        if(*movimiento == movimiento_poder && juego->supertrajes[juego->id_personaje_actual].usado == true
        && juego->personajes[juego->id_personaje_actual].movimientos_con_poder < 0){

            printf("Ya usaste el poder del supertraje de este personaje\n");
        }
}

//Pre: 0 <= juego.id_personaje_actual <= 3
//Pos: Asigna al personaje actual una posicion aleatoria en alguno de los 8 casilleros adyacentes al robot de su cuadrante inicial
void posicionar_post_pinza(juego_t* juego){

    int posicion_fila_aleatoria = 0;
    int posicion_columna_aleatoria = 0;

    while(posicion_fila_aleatoria == 0 && posicion_columna_aleatoria == 0){                                                                                     //Para evitar que sean 0 ambas.

        posicion_fila_aleatoria = rand() % 2;
        posicion_columna_aleatoria = rand() % 2;
    }

    juego->personajes[juego->id_personaje_actual].posicion.fila = juego->robots[juego->id_personaje_actual].posicion.fila - posicion_fila_aleatoria;
    juego->personajes[juego->id_personaje_actual].posicion.columna = juego->robots[juego->id_personaje_actual].posicion.columna - posicion_columna_aleatoria;

    if(juego->robots[juego->id_personaje_actual].posicion.fila == MAX_FILAS -1){

        juego->personajes[juego->id_personaje_actual].posicion.fila = juego->robots[juego->id_personaje_actual].posicion.fila - 1;
    }
    if(juego->robots[juego->id_personaje_actual].posicion.fila == MIN_FILAS){

        juego->personajes[juego->id_personaje_actual].posicion.fila = juego->robots[juego->id_personaje_actual].posicion.fila + 1;
    }
    if(juego->robots[juego->id_personaje_actual].posicion.columna == MAX_COLUMNAS -1){

        juego->personajes[juego->id_personaje_actual].posicion.columna = juego->robots[juego->id_personaje_actual].posicion.columna - 1;
    }
    if(juego->robots[juego->id_personaje_actual].posicion.columna == MIN_COLUMNAS){

        juego->personajes[juego->id_personaje_actual].posicion.columna = juego->robots[juego->id_personaje_actual].posicion.columna + 1;
    }
}

//Pre: -
//Pos: Realiza las acciones necesarias en caso de chocar con un supertraje.
void colision_supertraje(juego_t* juego){

    if(juego->personajes[juego->id_personaje_actual].posicion.fila == juego->supertrajes[juego->id_personaje_actual].posicion.fila
    && juego->personajes[juego->id_personaje_actual].posicion.columna == juego->supertrajes[juego->id_personaje_actual].posicion.columna
    && juego->supertrajes[juego->id_personaje_actual].recolectado == false){

        juego->supertrajes[juego->id_personaje_actual].recolectado = true;
        juego->personajes[juego->id_personaje_actual].tiene_supertraje = true;
        printf("Ahora tienes el supertraje :)\n");
    }
}


//Pre: -
//Pos: Realiza las acciones necesarias en caso de chocar con una pinza.
void colision_pinzas(juego_t* juego){

    for(int pinza_actual = 0; pinza_actual < juego->tope_pinzas; pinza_actual++){

        if(juego->personajes[juego->id_personaje_actual].posicion.fila == juego->pinzas[pinza_actual].fila
        && juego->personajes[juego->id_personaje_actual].posicion.columna == juego->pinzas[pinza_actual].columna
        && juego->personajes[juego->id_personaje_actual].poder_activado == false){

            posicionar_post_pinza(juego);
            printf("Oh no, una pinza te capturo\n");
        }
        if(juego->personajes[juego->id_personaje_actual].posicion.fila == juego->pinzas[pinza_actual].fila
        && juego->personajes[juego->id_personaje_actual].posicion.columna == juego->pinzas[pinza_actual].columna
        && juego->id_personaje_actual == 3 && juego->personajes[juego->id_personaje_actual].poder_activado == true){

            printf("Tu supertraje te salvo de ser arrastrado por la pinza\n");
        }
    }
}

//Pre: -
//Pos: Realiza las acciones necesarias en caso de chocar con un robot.
void colision_robot(juego_t* juego){

    for(int robot_actual = 0; robot_actual < juego->tope_robots; robot_actual++){

        if(juego->personajes[juego->id_personaje_actual].posicion.fila == juego->robots[robot_actual].posicion.fila
        && juego->personajes[juego->id_personaje_actual].posicion.columna == juego->robots[robot_actual].posicion.columna){

            juego->personajes[juego->id_personaje_actual].movimientos = 0;
        } 
    }
}

//Pre: -
//Pos: Realiza las acciones necesarias en caso de chocar con un laser.
void colision_laser(juego_t* juego){

    for(int laser_actual = 0; laser_actual < juego->robots->tope_lasers; laser_actual++){

        if(juego->personajes[juego->id_personaje_actual].posicion.fila == juego->robots->lasers[laser_actual].fila
        && juego->personajes[juego->id_personaje_actual].posicion.columna == juego->robots->lasers[laser_actual].columna
        && juego->personajes[juego->id_personaje_actual].poder_activado == false){

            juego->personajes[juego->id_personaje_actual].movimientos = 0;

        }
        if(juego->id_personaje_actual == 1 && juego->personajes[1].poder_activado == true && juego->personajes[1].posicion.fila == juego->robots->lasers[laser_actual].fila
        && juego->personajes[1].posicion.columna == juego->robots->lasers[laser_actual].columna){

            printf("Tu supertraje te salvo de ser desintegrado");
        
        }
    }   
}

//Pre: -
//Pos: Cambia al personaje con el que acabas de chocar.
void colision_pj(juego_t* juego){

    if(juego->personajes[juego->id_personaje_actual].posicion.fila == juego->personajes[juego->id_personaje_actual +1].posicion.fila
    && juego->personajes[juego->id_personaje_actual].posicion.columna == juego->personajes[juego->id_personaje_actual +1].posicion.columna){

        juego->id_personaje_actual++;
        printf("Has cambiado de personaje a %c\n", convencion_pj[juego->id_personaje_actual]);
    }
}


//Pre: -
//Pos: Devuelve false si el personaje esta en los límites del terreno, true en cualquier otro caso
bool es_destino_valido(juego_t juego, char movimiento){

    bool posicion_valida = true;

    if(juego.personajes[juego.id_personaje_actual].posicion.fila == MAX_FILAS -1 && movimiento == movimiento_abajo){

        posicion_valida = false;
    }
    if(juego.personajes[juego.id_personaje_actual].posicion.fila == MIN_FILAS && movimiento == movimiento_arriba){

        posicion_valida = false;
    }
    if(juego.personajes[juego.id_personaje_actual].posicion.columna == MAX_COLUMNAS -1 && movimiento == movimiento_derecha){

        posicion_valida = false;
    }
    if(juego.personajes[juego.id_personaje_actual].posicion.columna == MIN_COLUMNAS && movimiento == movimiento_izquierda){

        posicion_valida = false;
    }

return posicion_valida;
}


//Pre: -
//Pos: Activa el poder de elastic girl.
void activar_poder_elastic(juego_t* juego, char* movimiento){

    for(int cant_mov_actual = 0; cant_mov_actual < 2; cant_mov_actual++){
        if(es_destino_valido(*juego, *movimiento) == true){

            movimiento_pj(juego, movimiento);
        }
        else{
            printf("No podes avanzar más que esto\n");
        }
    }       
}

//Pre: -
//Pos: Chequea si elasticgirl tiene el poder activado y de ser afirmativo realiza los movimientos propios de ese poder
void chequear_poder_elastic(juego_t* juego, char* movimiento){

    if(juego->personajes[0].poder_activado == true && juego->id_personaje_actual == 0){
            
        printf("Activaste el poder de Elastic Girl, así que ahora serás capaz de estirarte por %i turnos más\n",juego->personajes[0].movimientos_con_poder);
                                                                                        
        activar_poder_elastic(juego, movimiento);
    }
}

//Pre: -
//Pos: Chequea si dash tiene el poder activado, de ser afirmativo lo transporta a una posicion aleatoria del cuadrante 3
void chequear_poder_dash(juego_t* juego){

    if(juego->personajes[2].poder_activado == true && juego->id_personaje_actual == 2){

        juego->personajes[2].posicion.fila = posicion_aleatoria (valor_minimo_fila[3]);
        juego->personajes[2].posicion.columna = posicion_aleatoria (valor_minimo_columna[3]);
        juego->personajes[2].poder_activado = false;
        juego->personajes[2].movimientos_con_poder = -1;

    }
}

//Pre: -
//Pos: Resta 1 a los movimientos de poder si el personaje tiene el poder activado, y lo desactiva cuando se cumplan las condiciones
void regular_poderes(juego_t* juego, char* movimiento){

    chequear_poder_elastic(juego, movimiento);
    chequear_poder_dash(juego);

    if((juego->personajes[juego->id_personaje_actual].poder_activado == true && juego->personajes[juego->id_personaje_actual].movimientos_con_poder >= 0)){

            juego->personajes[juego->id_personaje_actual].movimientos_con_poder--;
        }
        if(juego->personajes[juego->id_personaje_actual].poder_activado == true && juego->personajes[juego->id_personaje_actual].movimientos_con_poder < 0){
            
            juego->personajes[juego->id_personaje_actual].movimientos_con_poder --;
            printf("El poder de tu supertraje ha mermado completamente\n");
            juego->personajes[juego->id_personaje_actual].poder_activado = false;
        }
}

void mover_dron(juego_t* juego){

    for(int movimiento_dron = 0; movimiento_dron < juego->dron.velocidad; movimiento_dron++){

        bool ya_se_movio = false;
        if(juego->dron.posicion.fila > juego->personajes[juego->id_personaje_actual].posicion.fila 
        && juego->dron.posicion.columna == juego->personajes[juego->id_personaje_actual].posicion.columna && ya_se_movio == false){
        
            ya_se_movio = true;
            juego->dron.posicion.fila--;
    
        }
        if(juego->dron.posicion.fila < juego->personajes[juego->id_personaje_actual].posicion.fila 
        && juego->dron.posicion.columna == juego->personajes[juego->id_personaje_actual].posicion.columna && ya_se_movio == false){

            ya_se_movio = true;
            juego->dron.posicion.fila++;
    
        }
        if(juego->dron.posicion.columna > juego->personajes[juego->id_personaje_actual].posicion.columna 
        && juego->dron.posicion.fila == juego->personajes[juego->id_personaje_actual].posicion.fila && ya_se_movio == false){

            ya_se_movio = true;
            juego->dron.posicion.columna--;
    
        }
        if(juego->dron.posicion.columna < juego->personajes[juego->id_personaje_actual].posicion.columna
        && juego->dron.posicion.fila == juego->personajes[juego->id_personaje_actual].posicion.fila && ya_se_movio == false){

            ya_se_movio = true;
            juego->dron.posicion.columna++;
    
        }
        if(juego->dron.posicion.fila > juego->personajes[juego->id_personaje_actual].posicion.fila 
        && juego->dron.posicion.columna < juego->personajes[juego->id_personaje_actual].posicion.columna && ya_se_movio == false){
        
            ya_se_movio = true;
            juego->dron.posicion.fila--;
    
        }
        juego->dron.bateria--;
    }
}

void regular_dron(juego_t* juego){

    mover_dron(juego);

    if(juego->dron.bateria <= 0){

        juego->dron.destruido = true;
    }
}

void crear_dron(juego_t* juego){

    juego->dron.posicion.fila = posicion_aleatoria(10);
    juego->dron.posicion.columna = posicion_aleatoria(10);
    juego->dron.destruido = false;
    juego->dron.bateria = 200;
}

void realizar_jugada(juego_t* juego, char movimiento){

    int turnos_totales = 0;

    printf("Indique su movimiento: \n");
    scanf(" %c", &movimiento);

    while(es_movimiento_valido(movimiento) == false || es_destino_valido(*juego, movimiento) == false){

        printf("Tu movimiento no es valido, porfavor ingresa otro: \n");
        scanf(" %c", &movimiento);
    }

    movimiento_pj(juego, &movimiento);
    
    colision_supertraje(juego);
    colision_pinzas(juego);
    colision_robot(juego);
    colision_laser(juego);
    colision_pj(juego);

    regular_poderes(juego, &movimiento);

    if(turnos_totales == 0){

        crear_dron(juego);
    }
    //if(juego->dron.destruido == false){
    //    regular_dron(juego);
    //}
    if(movimiento != movimiento_poder){
        rotar_laseress(juego);
    }
    turnos_totales++;
    printf("Turnso totales: %i", turnos_totales);
}

int estado_juego(juego_t juego){

    int estado = juego_en_curso;

    if(juego.personajes[juego.id_personaje_actual].movimientos <= 0){

        estado = juego_perdido;
    }
    if(juego.id_personaje_actual == 3 && (juego).personajes[3].posicion.fila == MIN_FILAS && (juego).personajes[3].posicion.columna == MAX_COLUMNAS -1){

        estado = juego_ganado;
    }
    
return estado;

}
