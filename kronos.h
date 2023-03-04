#ifndef __KRONOS_H__
#define __KRONOS_H__

#include <stdbool.h>

#define MAX_LASERS 100
#define MAX_ROBOTS 10
#define MAX_PERSONAJES 10
#define MAX_PINZAS 100
#define MAX_SUPERTRAJES 10
#define MAX_DRONS 10

typedef struct coordenada{
    int fila;
    int columna;
} coordenada_t;

typedef struct dron{

    coordenada_t posicion;
    int velocidad;
    bool destruido;
    int bateria;
} dron_t;

typedef struct robot{
    coordenada_t posicion;
    coordenada_t lasers[MAX_LASERS];
    int tope_lasers;
} robot_t;

typedef struct supertraje{
    coordenada_t posicion;
    int cuadrante;
    bool recolectado;
    bool usado;
} supertraje_t;

typedef struct personaje{
    bool poder_activado;
    bool tiene_supertraje;
    coordenada_t posicion;
    int movimientos;
    int movimientos_con_poder;
    int cuadrante_inicial;
} personaje_t;

typedef struct juego{
    personaje_t personajes [MAX_PERSONAJES];
    int tope_personajes;
    robot_t robots [MAX_ROBOTS];
    int tope_robots;
    coordenada_t pinzas [MAX_PINZAS];
    int tope_pinzas;
    supertraje_t supertrajes [MAX_SUPERTRAJES];
    int tope_supertraje;
    int longitud_laser;
    int id_personaje_actual;
    dron_t dron;
    int tope_drons;
} juego_t;


/*
 * Inicializará el juego, cargando toda la información inicial de los robots, los supertrajes, el personaje, los lásers y las pinzas.
 * El campo id_personaje_actual comienza en 1.
 */ 
void inicializar_juego(juego_t* juego, bool contrasenia_completa);

/*
 * Moverá el personaje y se realizarán todas las acciones necesarias en caso de chocas con algún elemento.
 */ 
void realizar_jugada(juego_t* juego, char movimiento);

/*
 * Imprime el juego por pantalla.
 */
void imprimir_terreno(juego_t juego);

/*
 * El juego se dará por ganado si se está sobre el casillero de salida siendo el personaje Mr. Increíble y perdido si el personaje (cualquiera sea) se queda sin movimientos.
 * Devuelve:
 * -> 1 si se ganó el juego.
 * -> 0 si el juego aún se está jugando.
 * -> -1 se se perdió el juego.
 */
int estado_juego(juego_t juego);

#endif /* __KRONOS_H__ */