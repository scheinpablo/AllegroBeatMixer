/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   led_lib.h
 * Author: pablo
 *
 * Created on 24 de mayo de 2018, 14:10
 */

#ifndef LED_LIB_H
#define LED_LIB_H

#endif /* LED_LIB_H */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h> 


/**************/
/* Constantes */
/**************/

#define SCREEN_W		900
#define SCREEN_H		400

#define PLATE_W                 90
#define PLATE_H                 90

#define FPS 60.0

/************************/
/* Estructuras de datos */
/************************/

/*BIT: Estructura con las propiedades de cada led*/
typedef struct {
    ALLEGRO_SAMPLE * sample;
    ALLEGRO_SAMPLE_ID *id_sample;
    
    ALLEGRO_COLOR onColor;
    ALLEGRO_COLOR offColor;
    
    int x;
    int y;
    
    bool toggle;
    
    int ratio;
    
    bool on;
} BIT;

/*LEDS: Estructura con las propiedades y contenido de un puerto.*/
typedef struct {
    BIT bit0;
    BIT bit1;
    BIT bit2;
    BIT bit3;
    BIT bit4;
    BIT bit5;
    BIT bit6;
    BIT bit7;
    
    bool toggleFlag;
  
} LEDS;


/*WINDOW: Estructura con las propiedades y contenido de una ventana*/
typedef struct {
    ALLEGRO_DISPLAY * display;
    ALLEGRO_EVENT_QUEUE * event_queue;
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP * imagePlate;
    ALLEGRO_BITMAP *imageBackground;
    ALLEGRO_FONT * titleFont;
    ALLEGRO_FONT * genFont;
    
    char * samples[8];
} WINDOW;

/***************************/
/* Prototipos de funciones */
/***************************/


/* make_led_linear_h_layout
 * Se encarga de asignarle valores a los campos de cada led.
 * Todos los leds se posicionaran a la misma altura.
 * 
 * ref: Puntero a la estructura de todos los leds.
 * y: Altura a asignarles a los leds.
 * dist: Distancia entre los leds
 * first_x: Posicion en x del primer led
 * ratio: Radio de cada led.
 * onColor: Color del led en caso de encontrarse encendido.
 * offColor: Color del led en caso de encontrarse apagado.
 */
void make_led_linear_h_layout (LEDS * ref, int y, int dist, int first_x, int ratio, ALLEGRO_COLOR onColor, ALLEGRO_COLOR offColor);

/* draw
 * Dibuja en pantalla todos los elementos de la ventana.
 * 
 * ref: Puntero a la estructura de todos los leds.
 * window: Puntero a la ventana a mostrar.
 */
void draw (LEDS * ref, WINDOW * window);

/*start_allegro
 * Inicializa todos los aspectos relacionados a la libreria ALLEGRO.
 * Devuelve false si fallo alguna inicializacion.
 * Le asigna los sonidos a cada led.
 * 
 * window: Puntero a la ventana a iniciar.
 * ref: Puntero a la estructura de todos los leds.
 */
bool start_allegro (WINDOW * window, LEDS * ref);

/*key_pressed
 *Accion realizada cuando el usuario aprieta una tecla del teclado
 * 
 * unichar: Unichar de la tecla apretada.
 * ref: Puntero a la estructura de todos los leds.
 */
void key_pressed (int unichar, LEDS * ref);

/*allOff
 * Apaga todos los leds.
 * 
 * ref: Puntero a la estructura de todos los leds.
 */
void allOff (LEDS * ref);

/*allOn
 * Enciende todos los leds.
 * 
 * ref: Puntero a la estructura de todos los leds.
 */
void allOn (LEDS * ref);

/*allToggle
 * Invierte el estado de todos los leds.
 * 
 * ref: Puntero a la estructura de todos los leds.
 */
void allToggle (LEDS * ref);

/*onBlink
 * Genera un parpadeo en los leds encendidos.
 * 
 * ref: Puntero a la estructura de todos los leds.
 */
void onBlink (LEDS * ref);

/*allToggle
 * Invierte el estado de un led.
 * 
 * pBit: Puntero al bit a invertir.
 * ref: Puntero a la estructura de todos los leds.
 */
void toggleBit (BIT * pBit, LEDS * ref);

/*destroy
 * Destruye todos los campos relacionados a Allegro.
 * 
 * window: Puntero a la ventana a destruir
 */
void destroy (WINDOW * window);

/*mouse_click
 *Accion realizada cuando el usuario clickea el mouse
 * 
 * x: Coordenada x del mouse.
 * y: COordenada y del mouse.
 * ref: Puntero a la estructura de todos los leds.
 */
void mouse_click (int x, int y, LEDS *ref);
