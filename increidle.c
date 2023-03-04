#include "increidle.h"
#include <stdio.h>
#include <stdbool.h>

#define MAX_CONTRASENIA 6
#define CANTIDAD_MAX_NUMEROS_PRIMOS 5
#define CANTIDAD_MAX_CARACTERES_CORRECTOS 6

char const opcion_A = 'A';
char const opcion_B = 'B';
char const opcion_C = 'C';
char const opcion_D = 'D';

char const caracter_respuesta_fallida = 'X';
char const caracter_respuesta_correcta [CANTIDAD_MAX_CARACTERES_CORRECTOS] = {'K','R','O','N','S'};

int const numero_mcqueen = 95;


// Pre:
// Pos: Pide al usuario una respuesta hasta que sea válida.
void validar_respuesta_choice(char* respuesta_usuario){

    while(*respuesta_usuario != opcion_A && *respuesta_usuario != opcion_B
          && *respuesta_usuario != opcion_C && *respuesta_usuario != opcion_D){

        printf("La respuesta es invalida, tiene que ser %c, %c, %c, o %c. Por favor ingresarla de vuelta: ", opcion_A, opcion_B, opcion_C, opcion_D);
        scanf(" %c", respuesta_usuario);

    }
}




// Pre: 
// Pos: Pide al usuario un número hasta que esté dentro del rango valido.
void validar_numero(int* numero_usuario){
    int const numero_minimo_valido = 1;
    int const numero_maximo_valido = 100;

    while(*numero_usuario < numero_minimo_valido || *numero_usuario > numero_maximo_valido){
        printf("El numero es invalido, tiene que estar entre %i y %i. Por favor ingrese otro: ", numero_minimo_valido, numero_maximo_valido);
        scanf(" %i", numero_usuario);
    }

}


// Pre: Las variables del vector deben ser mayores o iguales a 1. El valor del tope recibido tiene que ser como mínimo 1.
// Pos: Devuelve true si todos las variables del vector son números primos, false en cualquier otro caso.
bool son_numeros_primos(int* numeros_primos, int tope_numeros_primos){
    int cantidad_divisores;
    int indice_actual = 0;
    int numero_compuesto = 0;

    while(numero_compuesto == 0 && indice_actual < tope_numeros_primos){
        cantidad_divisores = 0;
        
        for(int i = 1; i <= numeros_primos[indice_actual]; i++){

            if(numeros_primos[indice_actual] == 1){
                numero_compuesto++;
            }
            if(numeros_primos[indice_actual] % i == 0){
                cantidad_divisores++;
            }
            if(cantidad_divisores > 2){                                                 // Si un número tiene más de dos divisores no es un numero primo.
                numero_compuesto++;
            }
        }
    indice_actual++;
    }


return numero_compuesto == 0;
}

//Pre: tope_contrasenia_sindrome >= 0
//Pos: Llena el vector contraseña_sindrome en la posicion tope con su respectiva letra
void validar_respuesta_1(char* respuesta_pregunta_1, char contraseña_sindrome[MAX_CONTRASENIA], int* tope_contrasenia_sindrome){

    printf("Pregunta 1:  ¿Cuál es el código de emergencia utilizado en Monsters Inc."
            "cuando uno de los personajes tiene una media humana pegada a su cuerpo?\n\n");

    printf("(A) 3312\n(B) 3317\n(C) 2312\n(D) 2031\nIngrese su respuesta: ");
    scanf(" %c", respuesta_pregunta_1);

    validar_respuesta_choice(respuesta_pregunta_1);

    if (*respuesta_pregunta_1 == opcion_A){

        contraseña_sindrome[*tope_contrasenia_sindrome] = caracter_respuesta_correcta [0];
    }
        else{
            contraseña_sindrome [*tope_contrasenia_sindrome] = caracter_respuesta_fallida;
        }
    
    *tope_contrasenia_sindrome += 1;

}
//Pre: tope_contrasenia_sindrome >= 0
//Pos: Llena el vector contraseña_sindrome en la posicion tope con su respectiva letra
void validar_respuesta_2(char* respuesta_pregunta_2, char contraseña_sindrome[MAX_CONTRASENIA], int* tope_contrasenia_sindrome){


    printf("Pregunta 2: ¿Cuál era el nombre del pez mascota de Lilo?\n\n");
    printf("(A) Stich\n(B) Pez\n(C) Pato\n(D) Aurelio\nIngrese su respuesta: ");
    scanf(" %c", respuesta_pregunta_2);

    validar_respuesta_choice(respuesta_pregunta_2);

    if (*respuesta_pregunta_2 == opcion_C){
        contraseña_sindrome [*tope_contrasenia_sindrome] = caracter_respuesta_correcta [1];
        
    }
        else{
            contraseña_sindrome [*tope_contrasenia_sindrome] = caracter_respuesta_fallida;
        }

    *tope_contrasenia_sindrome += 1;


}
//Pre: tope_contrasenia_sindrome >= 0
//Pos: Llena el vector contraseña_sindrome en la posicion tope con su respectiva letra
void validar_respuesta_3(int numeros_primos [CANTIDAD_MAX_NUMEROS_PRIMOS], int tope_numeros_primos, char contraseña_sindrome[MAX_CONTRASENIA],
                                                                                                                    int* tope_contrasenia_sindrome){

    printf("Pregunta 3:  Tadashi necesita que lo ayudes con unos cálculos.Ingrese 5 números, todos deben ser primos.\n");

    while(tope_numeros_primos < CANTIDAD_MAX_NUMEROS_PRIMOS){
        printf("Ingresa un numero primo: ");
        scanf(" %i", &numeros_primos[tope_numeros_primos]);
        validar_numero(&numeros_primos[tope_numeros_primos]);
        tope_numeros_primos++;

    }

    if(son_numeros_primos(numeros_primos, tope_numeros_primos) == true){

        contraseña_sindrome[*tope_contrasenia_sindrome] = caracter_respuesta_correcta [2];                
        *tope_contrasenia_sindrome += 1;
        *tope_contrasenia_sindrome += 1;
                                                                                                                   
        contraseña_sindrome[*tope_contrasenia_sindrome] = caracter_respuesta_correcta [2];                                
    }
        else{
            contraseña_sindrome[*tope_contrasenia_sindrome] = caracter_respuesta_fallida;
            *tope_contrasenia_sindrome += 1;
            *tope_contrasenia_sindrome += 1;
            contraseña_sindrome[*tope_contrasenia_sindrome] = caracter_respuesta_fallida;
    }
}
//Pre: tope_contrasenia_sindrome >= 0
//Pos: Llena el vector contraseña_sindrome en la posicion tope con su respectiva letra
void validar_respuesta_4(int* respuesta_pregunta_4, char contraseña_sindrome[MAX_CONTRASENIA], int* tope_contrasenia_sindrome){

    printf("Pregunta 4:  ¿Qué número lleva el Rayo McQueen?\nIngrese su respuesta: ");
    scanf(" %i",respuesta_pregunta_4);
    validar_numero(respuesta_pregunta_4);

    if(*respuesta_pregunta_4 == numero_mcqueen){
        contraseña_sindrome [*tope_contrasenia_sindrome - 1] = caracter_respuesta_correcta [3];            
    }
        else{
            contraseña_sindrome [*tope_contrasenia_sindrome - 1] = caracter_respuesta_fallida;
    }

    *tope_contrasenia_sindrome += 1;

}

//Pre: tope_contrasenia_sindrome >= 0
//Pos: Llena el vector contraseña_sindrome en la posicion tope con su respectiva letra
void validar_respuesta_5(char* respuesta_pregunta_5, char contraseña_sindrome[MAX_CONTRASENIA], int* tope_contrasenia_sindrome){

    printf("Pregunta 5:  ¿Cómo se llama el tigre de la princesa Jazmín?\n\n");
    printf("(A) Zafiro\n(B) Abú\n(C) Rajah\n(D) Jafar\nIngrese su respuesta: ");
    scanf(" %c", respuesta_pregunta_5);
    validar_respuesta_choice(respuesta_pregunta_5);

    if(*respuesta_pregunta_5 == opcion_C){
        contraseña_sindrome [*tope_contrasenia_sindrome] = caracter_respuesta_correcta [4];

    }
        else{
            contraseña_sindrome [*tope_contrasenia_sindrome] = caracter_respuesta_fallida;
    }

    *tope_contrasenia_sindrome += 1;
}

//Pre: tope_contrasenia_sindrome >= 0
//Pos: Imprime el vector contraseña_sindrome
void imprimir_contraseña(char* contraseña_sindrome, int tope_contrasenia_sindrome){

    printf("-");
    for (int i = 0; i < tope_contrasenia_sindrome; i++){
        printf("%c", contraseña_sindrome[i]);
    }
    printf("-");
}

void adivinar_contrasenia(char contrasenia_adivinada[MAX_CONTRASENIA]){

    char contrasenia_sindrome[MAX_CONTRASENIA];
    int tope_contrasenia_sindrome = 0;
    char respuesta_pregunta_1 = ' ';

    validar_respuesta_1(&respuesta_pregunta_1, contrasenia_sindrome, &tope_contrasenia_sindrome);

    char respuesta_pregunta_2 = ' ';
    
    validar_respuesta_2(&respuesta_pregunta_2, contrasenia_sindrome, &tope_contrasenia_sindrome);

    int numeros_primos[CANTIDAD_MAX_NUMEROS_PRIMOS];
    int tope_numeros_primos = 0;

    validar_respuesta_3(numeros_primos, tope_numeros_primos, contrasenia_sindrome, &tope_contrasenia_sindrome);

    int respuesta_pregunta_4 = 0;

    validar_respuesta_4(&respuesta_pregunta_4, contrasenia_sindrome, &tope_contrasenia_sindrome);

    char respuesta_pregunta_5 = ' ';

    validar_respuesta_5(&respuesta_pregunta_5, contrasenia_sindrome, &tope_contrasenia_sindrome);

    for(int i = 0; i < tope_contrasenia_sindrome; i++){

        contrasenia_adivinada[i] = contrasenia_sindrome[i];
    }

}
