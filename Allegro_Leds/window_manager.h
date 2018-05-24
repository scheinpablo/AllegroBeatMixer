/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   window_manager.h
 * Author: lucas
 *
 * Created on 16 de mayo de 2018, 14:37
 */

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

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

/**************/
/* Constantes */
/**************/

#define MAX_LONG_STRING 256
#define MAX_STRING 20
#define FPS 60.0
#define MANAGER_EVENT_CLOSE 2000

/************************/
/* Estructuras de datos */
/************************/

/* STRING
 * Estructura para strings
 */
typedef char STRING[MAX_STRING];
typedef char LONG_STRING[MAX_LONG_STRING];

/* POSITION
 * Estructura para almacenar ubicacion (x,y) en display
 */
typedef struct{
    uint16_t x;
    uint16_t y;
} POSITION;

/* BUTTON
 * Estructura para control de botones en un display/ventana
 */
typedef struct{
    /* Position option */
    POSITION pos;
    
    /* Size options */
    uint16_t width;
    uint16_t height;
    
    /* Identifier options */
    uint16_t id;
    STRING text;
    
    /* Functionality options */
    bool isPressed;
    bool isFocus;
    bool loaded;
    
    /* Color options */
    ALLEGRO_COLOR backgroundColor;
    ALLEGRO_COLOR foregroundColor;
    ALLEGRO_COLOR lineColor;
    ALLEGRO_COLOR pressedColor;
    ALLEGRO_COLOR focusColor;
    
    ALLEGRO_FONT *font;
    STRING fontFile;
    uint16_t fontSize;
    
    /* Bitmap layout */
    ALLEGRO_BITMAP *bitmap;
    
    /* OnClick action */
    bool isClickSet;
    void (*onClick)(void*);
    void *params;
    
} BUTTON;

/*
 * LABEL
 * Estructura para el manejo de textos
 */
typedef struct{
    
    STRING idName;
    uint16_t id;
    bool loaded;
    
    /* Contenido del label */
    LONG_STRING text;
    
    /* Posicion dentro de parent */
    POSITION pos;
    
    /* Parametro de colores */
    ALLEGRO_COLOR colorFont;
    
    /* Parametros de fuente */
    ALLEGRO_FONT *font;
    uint8_t fontSize;
    STRING fontFile;
    
} LABEL;

/* TEXTBOX
 * Estructura para el ingreso de datos
 */
typedef struct{
    
    STRING idName;
    uint16_t id;
    bool loaded;
    
    /* Posicion */
    POSITION pos;
    
    /* Tamaño */
    uint16_t length;
    uint16_t width;
    uint16_t height;
    
    /* Tipo de dato */
    bool stringMode;
    bool intMode;
    
    bool isFocus;
    bool isPressed;
    
    /* Buffers */
    STRING stringBuffer;
    uint32_t intValue;
    
    /* Colores */
    ALLEGRO_COLOR backgroundColor;
    ALLEGRO_COLOR lineColor;
    ALLEGRO_COLOR standardLine;
    
    /* Bitmap */
    ALLEGRO_BITMAP *bitmap;
    
    /* Fuente */
    ALLEGRO_COLOR fontColor;
    ALLEGRO_FONT *font;
    STRING fontFile;
    uint16_t fontSize;
    
    /* onEnter */
    bool isActionSet;
    void (*onEnter)(void*);
    void *params;
    
} TEXTBOX;

/* WINDOW
 * Estructura para controlar una ventana
 */
typedef struct WINDOW_STRUCT WINDOW;

struct WINDOW_STRUCT{
    
    /* Parametros de la ventana */
    uint16_t width;
    uint16_t height;
    ALLEGRO_COLOR backgroundColor;
    
    /* Displays de la ventana */
    ALLEGRO_DISPLAY *display;
    
    /* Botones de la ventana */
    BUTTON *buttons;
    uint16_t numberOfButtons;
    uint16_t nextButtonId;
    
    /* Labels de la ventana */
    LABEL *labels;
    uint16_t numberOfLabels;
    uint16_t nextLabelId;
    
    /* Textboxes de la ventana */
    TEXTBOX *textboxes;
    uint16_t numberOfBoxes;
    uint16_t nextBoxId;
    
    /* Cola de eventos */
    ALLEGRO_EVENT_SOURCE userEvent;
    ALLEGRO_EVENT_QUEUE *eventQueue;
    ALLEGRO_EVENT event;
    
    /* Subproceso a ejecutar */
    bool isSubprocessOn;
    void (*subprocess)(WINDOW*);
    
    /* Clean up routine */
    bool cleanupOn;
    void (*cleanupRoutine)(WINDOW*);
    
    /* Timer de actualizacion de ventana */
    ALLEGRO_TIMER *refreshTimer;
    ALLEGRO_TIMER *subprocessTimer;
    
    /* Timer settings */
    double fps;
    double subprocessTime;
};

/* OPEN_WINDOW_PARAMS
 * Estructura con parametros para open window action
 */
typedef struct{
    WINDOW *fromWindow;
    WINDOW *toWindow;
} OPEN_WINDOW_PARAMS;

/***************************/
/* Prototipos de funciones */
/***************************/

/* clear_label
 * Limpia el contenido de un label segun el puntero
 *
 * label: Puntero al label
 */
void clear_label(LABEL *label);

/* clear_label_by_id
 * Limpia un label, segun su identificador
 *
 * window: Puntero a ventana
 * id: Identificador del label
 */
void clear_label_by_id(WINDOW *window, uint16_t id);

/* set_cleanup_routine 
 * Permite configurar una rutina para ejecutarla al momento
 * del cierre de la ventana
 *
 * window: Puntero a la ventana
 * cleanupRoutine: Puntero a la funcion
 */
void set_cleanup_routine(WINDOW *window, void (*cleanupRoutine)(WINDOW*));

/* generate_event 
 * Genera un evento
 * 
 * window: Puntero a ventana
 * eventCode: Codigo del evento
 */
void generate_event(WINDOW *window, uint16_t eventCode);

/* get_label_by_name 
 * Devuelve el label segun el id name
 *
 * window: Puntero a la ventana
 * idName: Identificador nombre del label
 */
LABEL *get_label_by_name(WINDOW *window, char *idName);

/* get_textbox_by_name 
 * Devuelve el label segun el id name
 *
 * window: Puntero a la ventana
 * idName: Identificador nombre del label
 */
TEXTBOX *get_textbox_by_name(WINDOW *window, char *idName);

/* clear_textbox 
 * Limpia el contenido de un textbox
 *
 * textbox: Punteor al textbox
 */
void clear_textbox(TEXTBOX *textbox);

/* clear_textbox_by_id
 * Limpia el contenido de un textbox
 *
 * window: Puntero a la ventana donde esta el textbox
 * id: Identificador del textbox
 */
void clear_textbox_by_id(WINDOW *window, uint16_t id);

/* get_button_by_id
 * Recupero un puntero al button dentro de la ventana
 *
 * window: Puntero a la ventana donde busco el id 
 * id: Identificador del button en la ventana
 */
BUTTON *get_button_by_id(WINDOW *window, uint16_t id);

/* get_label_by_id
 * Recupero un puntero al label dentro de la ventana
 *
 * window: Puntero a la ventana donde busco el id 
 * id: Identificador del label en la ventana
 */
LABEL *get_label_by_id(WINDOW *window, uint16_t id);

/* get_textbox_by_id 
 * Recupero un puntero al textbox dentro de la ventana
 *
 * window: Puntero a la ventana donde busco el id 
 * id: Identificador del textbox en la ventana
 */
TEXTBOX *get_textbox_by_id(WINDOW *window, uint16_t id);

/* set_subprocess
 * Establece la rutina de subprocesos
 *
 * window: Puntero a la ventana
 * subprocess: Funcion a ejecutar
 */
void set_subprocess(WINDOW *window, void (*subprocess)(WINDOW*));

/* get_textbox_value_by_id
 * Devuelve un puntero al dato contenido en el textbox
 * Requiere casteo, sabiendo si se lo pidio en string o int
 *
 * window: Puntero a la ventana
 * id: Identificador del textobx
 */
void* get_textbox_value_by_id(WINDOW *window, uint16_t id);

/* get_textbox_value
 * Devuelve un puntero al dato contenido en el textbox
 * Requiere casteo, sabiendo si se lo pidio en string o int
 */
void* get_textbox_value(TEXTBOX *textbox);

/* add_textbox_action_by_id
 * Configura la action al apretar enter en
 * una entrada de texto
 *
 * window: Puntero a la ventana
 * id: Identificar del textbox en la ventana
 * action: Puntero a la funcion callback
 * params: Puntero generico a parametro
 */
bool add_textbox_action_by_id(WINDOW *window, uint16_t id, void (*action)(void*), void *params);

/* add_textbox
 * Agrega un textbox a una ventana dada
 * Devuelve el id
 * 
 * window: Puntero de la ventana
 * idName: Identificador string del label
 * x: Posicion x
 * y: Posicion y
 * length: Cantidad de caracteres
 * mode: true/false -> string/int
 * backgroundColor: Color de fondo
 * lineColor: Color de la linea
 */
uint16_t add_textbox(WINDOW *window, char *idName, uint16_t x, uint16_t y, uint16_t length, bool mode, ALLEGRO_COLOR backgroundColor, ALLEGRO_COLOR lineColor);

/* add_label
 * Agrega un label a la ventana dada
 * Devuelve el id
 * 
 * window: Puntero a la ventana
 * idName: Identificador string del label
 * text: Texto a contener por el label
 * x: Posicion x de la ubicacion
 * y: Posicion y de la ubicacion
 * colorFont: Color de la fuente
 * fontSize: El tamaño de la fuente
 * fontFile: Nombre del archivo para cargar la fuente
 */
uint16_t add_label(WINDOW *window, char *idName, char *text, uint16_t x, uint16_t y, ALLEGRO_COLOR colorFont, uint8_t fontSize, char *fontFile);

/* open_window
 * Pasa de una ventana a otra, permitiendo volver al 
 * momento de cierre volver a la original
 *
 * fromWindow: Puntero a la ventana de donde se sale
 * toWindow: Puntero a la ventana a la que se llega
 */
void open_window(void *params);

/* run_window
 * Funcion de uso principal.
 * Luego de configurar COMO es la ventana, solamente
 * hay que llamar a esta funcion para que pase el control
 * a la nueva ventana y vuelve cuando termino
 *
 * window: Puntero a la ventana
 */
bool run_window(WINDOW *window);

/* add_button
 * Agrega un boton a una ventana
 * Devuelve el id
 *
 * window: Puntero a la ventana
 * x:       Coordenada x de la posicion del boton
 * y:       Coordenada y de la posicion del boton
 * width:   Ancho del boton
 * height:  Alto del boton
 * str:     Texto del boton
 * bg:      Color de fondo en el boton
 * fg:      Color del texto en el boton
 * lc:      Color de la linea borde del boton
 * pc:      Color de presion del boton
 */
uint16_t add_button(WINDOW *window, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *str, ALLEGRO_COLOR bg, ALLEGRO_COLOR fg, ALLEGRO_COLOR lc, ALLEGRO_COLOR pc, ALLEGRO_COLOR fc);

/* add_button_action_by_text
 * Agrega la accion on click de un boton segun su label
 *
 * window: Puntero a la ventana donde pertenece el boton
 * text: Puntero al label a buscar
 * action: Puntero a la funcion para agregar
 * params: Puntero a estructura con parametros
 */
bool add_button_action_by_text(WINDOW *window, const char *text, void (*action)(void*), void *params);

/* destroy_window
 * Libera la memoria ocupada por las partes integrantes
 * de esta ventana
 * 
 * window: Puntero a la ventana 
 */
void destroy_window(WINDOW *window);

/* refresh_window 
 * Actualiza el contenido en el display en pantalla
 *
 * window: Puntero a la ventana a actualizar
 */
void refresh_window(WINDOW *window);

/* new_window
 * Crea una nueva ventana
 * 
 * window: Puntero a la ventana a inicializar
 * width: Ancho de la ventana
 * height: Alto de la ventana
 * backgroundColor: Color de fondo de la ventana
 */
bool new_window(WINDOW *window, uint16_t width, uint16_t height, ALLEGRO_COLOR backgroundColor);

/* manager_init
 * Inicializa todos los modulos y parametros necesarios
 * para el correcto funcionamiento de la libreria
 */
bool manager_init(void);

/* set_label_text_by_id
 * Establece el texto de un label
 * 
 * window: Puntero a la ventana
 * id: Identificador de la ventana
 * text: Texto para configurar en el label
 */
void set_label_text_by_id(WINDOW *window, uint16_t id, const char *text);

/* set_label_text
 * Establece el texto de un label
 * 
 * label: Puntero al label
 */
void set_label_text(LABEL *label, const char *text);
        
#endif /* WINDOW_MANAGER_H */
