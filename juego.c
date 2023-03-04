#include "kronos.h"
#include "increidle.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int main(){

    srand ((unsigned)time(NULL));

    char contrasenia_adivinada[MAX_CONTRASENIA];
    //adivinar_contrasenia(contrasenia_adivinada);
    bool contrasenia_completa = false;

    if(contrasenia_adivinada[0] == 'K' && contrasenia_adivinada[1] == 'R' && contrasenia_adivinada[2] == 'O' && contrasenia_adivinada[3] == 'N' 
    && contrasenia_adivinada[4] == 'O' && contrasenia_adivinada[5] == 'S'){

        contrasenia_completa = true;
    }
    juego_t juego;

    char movimiento = ' ';

    inicializar_juego(&juego, contrasenia_completa);

    while(estado_juego(juego) == 0){

        system("clear");
        imprimir_terreno(juego);
        realizar_jugada(&juego, movimiento);
    }
    if(estado_juego(juego) == 1){
        printf("ganaste");
    }
    if(estado_juego(juego) == -1){
        printf("Perdisteeeeee KAMADA");
    }

return 0;


}
