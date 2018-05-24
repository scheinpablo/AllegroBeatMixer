/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   window_manager.c
 * Author: lucas
 * 
 * Created on 16 de mayo de 2018, 14:37
 */

#include "window_manager.h"

/***********************************/
/* Prototipo de funciones privadas */
/***********************************/

/* destroy_textbox 
 * Destruye lo que reservo el textbox
 * 
 * textbox: Puntero al textbox 
 */
static void destroy_textbox(TEXTBOX *textbox);

/* destroy_label
 * Destruye memoria restante del label
 *
 * label: Puntero al label
 */
static void destroy_label(LABEL *label);

/* is_number
 * Devuelve true o false si es numero o no
 *
 * c: caracter
 */
static bool is_number(char c);

/* is_letter
 * Devuelve true o false si es letra o no
 *
 * c: caracter
 */
static bool is_letter(char c);

/* input_textbox 
 * Controla el cambio del buffer string segun el evento
 * de keyboard.
 * 
 * textbox: Puntero al textbox actual
 * event: Evento recibido
 */
static bool input_textbox(TEXTBOX *texbox, ALLEGRO_EVENT event);

/* new_textbox
 * Crea una nueva caja de texto para ingreso
 * 
 * id: Identificador del textbox
 * idName: Identificador string del label
 * x: Posicion x
 * y: Posicion y
 * length: Cantidad de caracteres
 * mode: true/false -> string/int
 * backgroundColor: Color de fondo
 * lineColor: Color de la linea
 */
static TEXTBOX new_textbox(uint16_t id, char *idName, uint16_t x, uint16_t y, uint16_t length, bool mode, ALLEGRO_COLOR backgroundColor, ALLEGRO_COLOR lineColor);

/* new_label
 * Crea un nuevo label
 * 
 * id: Identificador del label en la ventana
 * idName: Identificador string del label
 * text: Texto a contener por el label
 * x: Posicion x de la ubicacion
 * y: Posicion y de la ubicacion
 * colorFont: Color de la fuente
 * fontSize: El tamaño de la fuente
 * fontFile: Nombre del archivo para cargar la fuente
 */
static LABEL new_label(uint16_t id, char *idName, char *text, uint16_t x, uint16_t y, ALLEGRO_COLOR colorFont, uint8_t fontSize, char *fontFile); 

/* update_textbox 
 * Actualiza el bitmpa del textbox
 *
 * textbox: Puntero al textbox 
 */
static void update_textbox(TEXTBOX *textbox);

/* update_label
 * Actualiza los labels
 * 
 * label: Puntero a label
 */
static void update_label(LABEL *label);

/* close_window
 * Oculta la ventana
 * 
 * window: Puntero a la ventana
 */
static void close_window(WINDOW *window);

/* unregister_window_sources 
 * Desregistra las fuentes de eventos para la ventana
 * 
 * window: Puntero a la ventana 
 */
static void unregister_window_sources(WINDOW *window);

/* register_window_sources 
 * Registra las fuentes de eventos para la ventana
 * 
 * window: Puntero a la ventana 
 */
static void register_window_sources(WINDOW *window);

/* window_init 
 * Inicializa la ventana
 */
static bool window_init(WINDOW *window);

/* is_inside_of
 * Devuelve segun si la posicion (x,y) esta ubicada
 * dentro del boton que se pasa como parametro
 * 
 * x:       Coordenada x de la posicion
 * y:       Coordenada y de la posicion
 * button: Instancia del boton a verificar
 */
static bool is_inside_of(POSITION pos, int width, int height, int x, int y);

/* create_button
 * Crea un boton con sus carateristicas basicas
 * 
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
static BUTTON create_button(uint16_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *str, ALLEGRO_COLOR bg, ALLEGRO_COLOR fg, ALLEGRO_COLOR lc, ALLEGRO_COLOR pc, ALLEGRO_COLOR fc);
 
/* destroy_button
 * Destruye el boton, liberando la memoria que lo contenia
 * 
 * button: Puntero a la instancia del boton
 */
static void destroy_button(BUTTON *button);

/* update_button
 * Actualiza el boton, sus estados y atributos
 * 
 * button: Puntero a la instancia del boton
 */
static void update_button(BUTTON *button);

/***************************/
/* Definicion de funciones */
/***************************/

/* clear_label_by_id */
void clear_label_by_id(WINDOW *window, uint16_t id){
    uint16_t i;
    
    if( id < window->nextLabelId ){
        for(i = 1;i < window->nextLabelId;i++){
            if(id == window->labels[i].id){
                clear_label(&window->labels[i]);
            }
        }
    }
}

/* clear_label */
void clear_label(LABEL *label){
    strcpy(label->text, "");
}

/* set_cleanup_routine */
void set_cleanup_routine(WINDOW *window, void (*cleanupRoutine)(WINDOW*)){
    window->cleanupRoutine = cleanupRoutine;
    if( !window->cleanupOn ){
        window->cleanupOn = true;
    }
}

/* generate_event */
void generate_event(WINDOW *window, uint16_t eventCode){
    /* Inicializo el evento */
    ALLEGRO_EVENT event;
    
    /* Parametro de evento */
    event.type = eventCode;
    
    /* Creo el evento */
    al_emit_user_event(&window->userEvent, &event, NULL);
}

/* get_label_by_name */
LABEL *get_label_by_name(WINDOW *window, char *idName){
    uint16_t i;
    
    for(i = 1;i < window->nextLabelId;i++){
        if( !(strcmp(window->labels[i].idName, idName)) ){
            return &window->labels[i];
        }
    }
    return NULL;
}

/* get_textbox_by_name*/
TEXTBOX *get_textbox_by_name(WINDOW *window, char *idName){
    uint16_t i;
    
    for(i = 1;i < window->nextBoxId;i++){
        if( !(strcmp(window->textboxes[i].idName, idName)) ){
            return &window->textboxes[i];
        }
    }
    return NULL;
}

void clear_textbox_by_id(WINDOW *window, uint16_t id){
    uint16_t i;
    
    /* Verifico que sea un id valido */
    if( id < window->nextBoxId ){
        for(i = 1;i < window->nextBoxId;i++){
            if( window->textboxes[i].id == id ){
                clear_textbox( &window->textboxes[i]);
            }
        }
    }
}
/* clear_textbox */
void clear_textbox(TEXTBOX *textbox){
    /* Limpio el buffer */
    strcpy(textbox->stringBuffer, "");
}

/* get_label_by_id */
LABEL *get_label_by_id(WINDOW *window, uint16_t id){
    uint16_t i;
    
    if( id < window->nextLabelId ){
        for(i = 1;i < window->nextLabelId;i++){
            if( window->labels[i].id == id ){
                return &window->labels[i]; 
            }
        }
    }
    return NULL;
}

/* get_textbox_by_id */
TEXTBOX *get_textbox_by_id(WINDOW *window, uint16_t id){
    uint16_t i;
    
    if( id < window->nextBoxId ){
        for(i = 1;i < window->nextBoxId;i++){
            if( window->textboxes[i].id == id ){
                return &window->textboxes[i]; 
            }
        }
    }
    return NULL;
}

/* get_button_by_id */
BUTTON *get_button_by_id(WINDOW *window, uint16_t id){
    uint16_t i;
    
    if( id < window->nextButtonId ){
        for(i = 1;i < window->nextButtonId;i++){
            if( window->buttons[i].id == id ){
                return &window->buttons[i]; 
            }
        }
    }
    return NULL;
}

/* set_subprocess */
void set_subprocess(WINDOW *window, void (*subprocess)(WINDOW*)){
    window->subprocess = subprocess;
    if( !window->isSubprocessOn ){
        window->isSubprocessOn = true;
    }
}

/* get_textbox_value */
void* get_textbox_value(TEXTBOX *textbox){
    
    if( textbox->intMode ){
        textbox->intValue = atoi(textbox->stringBuffer);
        return &textbox->intValue;
    }else{
        return textbox->stringBuffer;
    }
}

/* get_textbox_value_by_id */
void* get_textbox_value_by(WINDOW *window, uint16_t id){
    
    /* Me fijo como lo configuro al textbox */
    if( window->textboxes[id].intMode ){
        window->textboxes[id].intValue = atoi(window->textboxes[id].stringBuffer);
        return &window->textboxes[id].intValue;
    }else{
        return window->textboxes[id].stringBuffer;
    }
}

/* set_label_text_by_id */
void set_label_text_by_id(WINDOW *window, uint16_t id, const char *text){
    strcpy(window->labels[id].text, text);
}

/* set_label_text */
void set_label_text(LABEL *label, const char *text){
    strcpy(label->text, text);
}

/* add_textbox_action_by_id */
bool add_textbox_action_by_id(WINDOW *window, uint16_t id, void (*action)(void*), void *params){
    /* Verifico que sea un id valido*/
    if( id < window->nextBoxId ){
        /* Configuro accion y parametros */
        window->textboxes[id].onEnter = action;
        window->textboxes[id].params = params;
        /* Establezco que ya se definio, flag de control */
        if( !window->textboxes[id].isActionSet ){
            window->textboxes[id].isActionSet = true;
        }
        
        return true;
    }
    
    return false;
}

/* is_number */
static bool is_number(char c){
    if( c >= '0' && c <= '9' ){
        return true;
    }else{
        return false;
    }
}

/* is_letter */
static bool is_letter(char c){
    if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ){
        return true;
    }else{
        return false;
    }
}

/* input_textbox */
static bool input_textbox(TEXTBOX *textbox, ALLEGRO_EVENT event){
    uint16_t length;
    
    /* Largo del buffer */
    length = strlen(textbox->stringBuffer);
    
    switch( event.keyboard.keycode ){
        case ALLEGRO_KEY_BACKSPACE:
            if( length ){
                textbox->stringBuffer[length-1] = '\0';
            }
            break;
        case ALLEGRO_KEY_ENTER:
            return true;
            break;
        default:
            if( (is_number(event.keyboard.unichar) && textbox->intMode) || (is_letter(event.keyboard.unichar) && textbox->stringMode) ){
                if( length < textbox->length ){
                    textbox->stringBuffer[length] = event.keyboard.unichar;
                    textbox->stringBuffer[length+1] = '\0';
                }
            }
            break;
    }
    return false;
}

/* add_textbox */
uint16_t add_textbox(WINDOW *window, char *idName, uint16_t x, uint16_t y, uint16_t length, bool mode, ALLEGRO_COLOR backgroundColor, ALLEGRO_COLOR lineColor){
    
    /* Incremento el contador de textboxes */
    window->numberOfBoxes++;
    
    /* Asigno mas memoria */
    window->textboxes = realloc(window->textboxes, sizeof(TEXTBOX) * (window->numberOfBoxes+2) );
    if( window->textboxes == NULL ){
        return 0;
    }
    
    /* Guardo nuevo elemento */
    window->textboxes[window->nextBoxId] = new_textbox(window->nextBoxId, idName, x, y, length, mode, backgroundColor, lineColor);
    
    /* Incremento ID */
    window->nextBoxId++;
    
    return window->nextBoxId - 1;
}

/* update_textbox */
static void update_textbox(TEXTBOX *textbox){
    ALLEGRO_DISPLAY *display;
    
    /* Guardo el backbuffer original */
    display = al_get_current_display();
    
    /* Verifico loaded */
    if( !textbox->loaded ){
        textbox->font = al_load_ttf_font(textbox->fontFile, textbox->fontSize, 0);
        textbox->bitmap = al_create_bitmap(textbox->width, textbox->height);
        textbox->loaded = true;
    }
    
    /* Muevo al bitmap */
    al_set_target_bitmap(textbox->bitmap);
    
    /* Limpio el bitmap */
    al_clear_to_color(textbox->backgroundColor);
    
    /* Dibujo rectangulo */
    if( textbox->isPressed ){
        al_draw_rectangle(1, 0, textbox->width, textbox->height, textbox->lineColor, 3);
    }else{
        al_draw_rectangle(1, 0, textbox->width, textbox->height, textbox->standardLine, 3);
    }
    
    /* Imprimo contenido actual */
    al_draw_text(textbox->font, textbox->fontColor, 2, 0, 0, textbox->stringBuffer);    
    
    /* Devuelvo control al backbuffer original */
    al_set_target_backbuffer(display);
}

/* new_textbox */
static TEXTBOX new_textbox(uint16_t id, char *idName, uint16_t x, uint16_t y, uint16_t length, bool mode, ALLEGRO_COLOR backgroundColor, ALLEGRO_COLOR lineColor) {
    TEXTBOX textbox = {
        .id = id,
        .isFocus = false,
        .isPressed = false,
        .loaded = false,
        .pos.x = x,
        .pos.y = y,
        .length = length,
        .backgroundColor = backgroundColor,
        .lineColor = lineColor,
        .standardLine = al_map_rgb(120, 120, 120),
        .isActionSet = false
    };
    
    /* Veo que modo de textbox */
    if( mode ){
        textbox.stringMode = true;
        textbox.intMode = false;
    }else{
        textbox.stringMode = false;
        textbox.intMode = true;
    }
    
    /* Guardo el id name */
    strcpy(textbox.idName, idName);
    
    /* Configuro la fuente */
    textbox.fontColor = al_map_rgb(0, 0, 0);
    textbox.fontSize = 20;
    strcpy(textbox.fontFile, "Roboto-Thin.ttf");
    textbox.font = al_load_ttf_font(textbox.fontFile, textbox.fontSize, 0);
    
    /* Calculo tamaño */
    textbox.height = textbox.fontSize + textbox.fontSize / 6;
    textbox.width = textbox.length * textbox.fontSize * 0.65;
    
    /* Contenido vacio */
    strcpy(textbox.stringBuffer, "");
    
    return textbox;
}   


/* add_label */
uint16_t add_label(WINDOW *window, char *idName, char *text, uint16_t x, uint16_t y, ALLEGRO_COLOR colorFont, uint8_t fontSize, char *fontFile){
    
    /* Incremento el contador de labels */
    window->numberOfLabels++;
    
    /* Agrego memoria disponible */
    window->labels = realloc(window->labels, sizeof(LABEL) * (window->numberOfLabels + 2));
    if( window->labels == NULL ){
        return 0;
    }
    
    /* Creo y guardo el label */
    window->labels[window->nextLabelId] = new_label(window->nextLabelId, idName, text, x, y, colorFont, fontSize, fontFile);

    /* Incremento contador de id */
    window->nextLabelId++;
    
    return window->nextLabelId - 1;
}

/* update_label */
static void update_label(LABEL *label){
    /* Inicializo el font */
    if( !label->loaded ){
        label->font = al_load_ttf_font(label->fontFile, label->fontSize, 0);
        label->loaded = true;        
    }
    
}

/* new_label */
static LABEL new_label(uint16_t id, char *idName, char *text, uint16_t x, uint16_t y, ALLEGRO_COLOR colorFont, uint8_t fontSize, char *fontFile){
    /* Configuro parametros basicos */
    LABEL label = {
        .id = id,
        .loaded = false,
        .pos.x = x,
        .pos.y = y,
        .colorFont = colorFont,
        .fontSize = fontSize,
    };
    
    /* Guardo el id name */
    strcpy(label.idName, idName);
    
    /* Configuro el contenido */
    strcpy(label.text, text);
    strcpy(label.fontFile, fontFile);
    
    return label;
}

/* close_window */
static void close_window(WINDOW *window){
    uint16_t i;
    
    /* Destruyo todos los botones */
    if( window->numberOfButtons ){
        for(i = 1;i < window->nextButtonId;i++){
            destroy_button( &window->buttons[i] );
        }
    }
    
    /* Destruyo todos los labels */
    if( window->numberOfLabels ){
        for(i = 1;i < window->nextLabelId;i++){
            destroy_label( &window->labels[i] );
        }
    }
    
    /* Destruyo todos los textboxes */
    if( window->numberOfBoxes ){
        for(i = 1;i < window->nextBoxId;i++){
            destroy_textbox( &window->textboxes[i] );
        }
    }
    
    /* Unregister */
    unregister_window_sources(window);
    
    /* Destruyo el user event source */
    al_destroy_user_event_source(&window->userEvent);
    
    /* Destruyo la cola de eventos */
    al_destroy_event_queue(window->eventQueue);
    
    /* Destruyo el timer */
    al_destroy_timer(window->refreshTimer);
    al_destroy_timer(window->subprocessTimer);
    
    /* Destruyo el backbuffer */
    al_destroy_display(window->display);
}

/* open_window */
void open_window(void *params){
    
    OPEN_WINDOW_PARAMS *args = params;
    
    /* Cierro la ventana anterior */
    close_window(args->fromWindow);
    
    /* Llamo a la nueva ventana */
    run_window(args->toWindow);
    
    /* Abro la ventana anterior */
    window_init(args->fromWindow);
}

/* register_window_sources*/
static void register_window_sources(WINDOW *window){
    al_register_event_source(window->eventQueue, &window->userEvent);
    al_register_event_source(window->eventQueue, al_get_timer_event_source(window->refreshTimer));  
    al_register_event_source(window->eventQueue, al_get_timer_event_source(window->subprocessTimer));  
    al_register_event_source(window->eventQueue, al_get_mouse_event_source());
    al_register_event_source(window->eventQueue, al_get_keyboard_event_source());
    al_register_event_source(window->eventQueue, al_get_display_event_source(window->display));
}
    
/* register_window_sources*/
static void unregister_window_sources(WINDOW *window){
    al_unregister_event_source(window->eventQueue, &window->userEvent);
    al_unregister_event_source(window->eventQueue, al_get_timer_event_source(window->refreshTimer));  
    al_unregister_event_source(window->eventQueue, al_get_timer_event_source(window->subprocessTimer));
    al_unregister_event_source(window->eventQueue, al_get_mouse_event_source());
    al_unregister_event_source(window->eventQueue, al_get_keyboard_event_source());
    al_unregister_event_source(window->eventQueue, al_get_display_event_source(window->display));
}

/* run_window */
bool run_window(WINDOW *window){
    bool close = false;
    bool refresh = false;
    uint16_t i;
    
    /* Inicializo la ventana */
    window_init(window);
    
    /* Window loop */
    while( !close ){
        
        /* Busco un nuevo evento */
        if( al_get_next_event(window->eventQueue, &window->event) ){
        
            /* Manejo los eventos que van llegando */
            switch( window->event.type ){
                case ALLEGRO_EVENT_KEY_CHAR:
                    if( window->numberOfBoxes ){
                        for(i = 1;i < window->nextBoxId;i++){
                            if( window->textboxes[i].isPressed ){
                                if( input_textbox(&window->textboxes[i], window->event) ){
                                    if( window->textboxes[i].isActionSet ){
                                        window->textboxes[i].onEnter(window->textboxes[i].params);
                                    }
                                }
                            }
                        }
                    }
                    break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE: case MANAGER_EVENT_CLOSE:
                    if( window->cleanupOn ){
                        window->cleanupRoutine(window);
                    }
                    close = true;
                    break;
                case ALLEGRO_EVENT_TIMER:
                    if( window->event.timer.source == window->refreshTimer ){
                        refresh = true;
                    }else if( window->event.timer.source == window->subprocessTimer ){
                        if( window->isSubprocessOn ){
                            window->subprocess(window);
                        }
                    }
                    break;
                case ALLEGRO_EVENT_MOUSE_AXES:
                    if( window->numberOfButtons ){
                        for(i = 1;i < window->nextButtonId;i++){
                            window->buttons[i].isFocus = is_inside_of(window->buttons[i].pos, window->buttons[i].width, window->buttons[i].height, window->event.mouse.x, window->event.mouse.y);
                        }
                    }
                    if( window->numberOfBoxes ){
                        for(i = 1;i < window->nextBoxId;i++){
                            window->textboxes[i].isFocus = is_inside_of(window->textboxes[i].pos, window->textboxes[i].width, window->textboxes[i].height, window->event.mouse.x, window->event.mouse.y);
                        }
                    }
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    if( window->numberOfButtons ){
                        for(i = 1;i < window->nextButtonId;i++){
                            window->buttons[i].isPressed = is_inside_of(window->buttons[i].pos, window->buttons[i].width, window->buttons[i].height, window->event.mouse.x, window->event.mouse.y);
                        }
                    }
                    if( window->numberOfBoxes ){
                        for(i = 1;i < window->nextBoxId;i++){
                            if( is_inside_of(window->textboxes[i].pos, window->textboxes[i].width, window->textboxes[i].height, window->event.mouse.x, window->event.mouse.y) ){
                                if( window->textboxes[i].isPressed ){
                                    window->textboxes[i].isPressed = false;
                                }else{
                                    window->textboxes[i].isPressed = true;                                    
                                }
                            }else{
                                window->textboxes[i].isPressed = false;
                            }
                        }
                    }
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    if( window->numberOfButtons ){
                        for(i = 1;i < window->nextButtonId;i++){
                            if( window->buttons[i].isPressed ){
                                window->buttons[i].isPressed = false;
                                if( window->buttons[i].isClickSet ){
                                    window->buttons[i].onClick(window->buttons[i].params);
                                }
                            }
                        }
                    }
                    break;
            }
        }
        
        /* Finalmente actualizo todo */
        if( refresh && al_is_event_queue_empty(window->eventQueue) ){
            refresh_window(window);
            refresh = false;
        }
    }
    
    /* Cierro la ventana */
    close_window(window);
    
    return true;
}

/* refresh_window */
void refresh_window(WINDOW *window){
    uint16_t i;
    
    /* Limpio el display */
    al_clear_to_color(window->backgroundColor);
    
    /* Imprimo cada boton */
    if( window->numberOfButtons ){
        for(i = 1;i < window->nextButtonId;i++){
            update_button(&window->buttons[i]);
            al_draw_bitmap( window->buttons[i].bitmap, window->buttons[i].pos.x, window->buttons[i].pos.y, 0);
        }
    }
    
    /* Imprimo cada label */
    if( window->numberOfLabels ){
        for(i = 1;i < window->nextLabelId;i++){
            update_label(&window->labels[i]);
            al_draw_text(window->labels[i].font, window->labels[i].colorFont, window->labels[i].pos.x, window->labels[i].pos.y, 0, window->labels[i].text);
        }
    }
    
    /* Imprimo cada textbox */
    if( window->numberOfBoxes ){
        for(i = 1;i < window->nextBoxId;i++){
            update_textbox(&window->textboxes[i]);
            al_draw_bitmap(window->textboxes[i].bitmap, window->textboxes[i].pos.x, window->textboxes[i].pos.y, 0);
        }
    }
    
    /* Mando backbuffer a pantalla */
    al_flip_display();
}

/* add_button */
uint16_t add_button(WINDOW *window, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *str, ALLEGRO_COLOR bg, ALLEGRO_COLOR fg, ALLEGRO_COLOR lc, ALLEGRO_COLOR pc, ALLEGRO_COLOR fc){
    
    /* Aumento contador para numero de botones */
    window->numberOfButtons++;
    
    /* Agrego un bloque mas de memoria a la lista de botones */
    window->buttons = realloc(window->buttons, sizeof(BUTTON) * (window->numberOfButtons+2));
    if( window->buttons == NULL ){
        return 0;
    }
    
    /* Voy al final y creo un boton */
    window->buttons[window->nextButtonId] = create_button(window->nextButtonId, x, y, width, height, str, bg, fg, lc, pc, fc);
    
    /* Aumento contador para ID */
    window->nextButtonId++;
    
    return window->nextButtonId - 1;
}

/* add_button_action_by_text */
bool add_button_action_by_text(WINDOW *window, const char *text, void (*action)(void*), void *params){
    uint16_t i;
    
    /* Verifico que hayan botones */
    if( window->numberOfButtons ){
        /* Itero por todos los botones de window */
        for(i = 1;i < window->nextButtonId;i++){
            /* Me fijo cual tiene ese nombre */
            if( !strcmp(text, window->buttons[i].text) ){
                /* Le establezco la accion */
                window->buttons[i].onClick = action;
                window->buttons[i].params = params;
                window->buttons[i].isClickSet = true;
                return true;
            }
        }
    }
    
    return false;
}

/* destroy_window */
void destroy_window(WINDOW *window){
    
    /* Libero memoria de los botones */
    free(window->buttons);
    
    /* Libero memoria de los labels */
    free(window->labels);
    
    /* Libero memoria de los textboxes */
    free(window->textboxes);
}

/* new_window */
bool new_window(WINDOW *window, uint16_t width, uint16_t height, ALLEGRO_COLOR backgroundColor){
        
    /* Inicializo parametros generales */
    window->numberOfButtons = 0;
    window->nextButtonId = 1;
    window->numberOfLabels = 0;
    window->nextLabelId = 1;
    window->numberOfBoxes = 0;
    window->nextBoxId = 1;
    window->width = width;
    window->height = height;
    window->backgroundColor = backgroundColor;
    window->fps = FPS;
    window->subprocessTime = 0.01;
    window->isSubprocessOn = false;
    window->cleanupOn = false;
    
    /* Reservo memoria inicial para lista de botones */
    window->buttons = malloc( sizeof(BUTTON) * 2 );
    if( window->buttons == NULL ){
        return false;
    }
    
    /* Reservo memoria inicial para lista de labels */
    window->labels = malloc( sizeof(LABEL) * 2 );
    if( window->labels == NULL ){
        return false;
    }
    
    /* Reservo memoria inicial para lista de textboxes */
    window->textboxes = malloc( sizeof(TEXTBOX) * 2 );
    if( window->textboxes == NULL ){
        return false;
    }
    
    return true;
}

/* window_init */
static bool window_init(WINDOW *window){
    
    /* Creo el backbuffer de esta ventana */
    window->display = al_create_display(window->width, window->height);
    if( window->display == NULL ){
        return false;
    }
    
    /* Creo la cola de eventos */
    window->eventQueue = al_create_event_queue();
    if( window->eventQueue == NULL ){
        al_destroy_display(window->display);
        return false;
    }
    
    /* Creo el refresh timer */
    window->refreshTimer = al_create_timer(1.0 / window->fps);
    if( window->refreshTimer == NULL ){
        al_destroy_display(window->display);
        al_destroy_event_queue(window->eventQueue);
        return false;
    }
    
    /* Creo el subprocess timer */
    window->subprocessTimer = al_create_timer(window->subprocessTime);
    if( window->subprocessTimer == NULL ){
        al_destroy_timer(window->refreshTimer);
        al_destroy_display(window->display);
        al_destroy_event_queue(window->eventQueue);
        return false;
    }
    
    /* Creo user event sources */
    al_init_user_event_source(&window->userEvent);
    
    /* Registro fuentes de eventos */
    register_window_sources(window);
    
    /* Limpio la queue */
    al_flush_event_queue(window->eventQueue);
    
    /* Paso el control a la nueva ventana */
    al_set_target_backbuffer(window->display);
    
    /* Inicio el timer */
    al_start_timer( window->refreshTimer );
    al_start_timer( window->subprocessTimer );
    
    return true;
}

/* is_inside_of */
static bool is_inside_of(POSITION pos, int width, int height, int x, int y){
    if( pos.x < x && x < (pos.x + width) && pos.y < y && y < (pos.y + height) ){
        return true;      
    }else{
        return false;
    }
}

/* manager_init */
bool manager_init(){
    /* Inicializo libreria general de Allegro */
    if( !al_init() ){
        return false;
    }
    
    /* Inicializo addons de Allegro */
    al_init_font_addon();
    al_init_primitives_addon();
    al_init_ttf_addon();
    
    /* Instala el mouse */
    if( !al_install_mouse() ){
        return false;
    }
    
    /* Instala el keyboard */
    if( !al_install_keyboard() ){
        return false;
    }
    
    return true;
}

/* destroy_button */
static void destroy_button(BUTTON *button){
    al_destroy_font(button->font);
    al_destroy_bitmap(button->bitmap);
    button->loaded = false;
}

/* destroy_textbox */
static void destroy_textbox(TEXTBOX *textbox){
    al_destroy_font(textbox->font);
    al_destroy_bitmap(textbox->bitmap);
    textbox->loaded = false;
}

/* destroy_labels */
static void destroy_label(LABEL *label){
    al_destroy_font(label->font);
    label->loaded = false;
}

/* create_button */
static BUTTON create_button(uint16_t id, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *str, ALLEGRO_COLOR bg, ALLEGRO_COLOR fg, ALLEGRO_COLOR lc, ALLEGRO_COLOR pc, ALLEGRO_COLOR fc){
    /* Inicializo la instancia del boton */
    BUTTON boton = {
        .id = id,
        .pos.x = x,
        .pos.y = y,
        .loaded = false,
        .width = width,
        .height = height,
        .backgroundColor = bg,
        .foregroundColor = fg,
        .lineColor = lc,
        .pressedColor = pc,
        .focusColor = fc,
        .isPressed = 0,
        .isFocus = 0,
        .isClickSet = false
    };
    
    /* Inicializo el texto del boton */
    strcpy(boton.text, str);
    
    /* Font */
    boton.fontSize = 18;
    strcpy(boton.fontFile, "chapter.ttf");
    
    return boton;
}

/* update_button */
static void update_button(BUTTON *button){
    ALLEGRO_DISPLAY *display;
    
    /* Guardo el buffer actual */
    display = al_get_current_display();
    
    /* Verifico si se creo */
    if( !button->loaded ){
        button->font = al_load_ttf_font(button->fontFile, button->fontSize, 0);
        button->bitmap = al_create_bitmap(button->width, button->height);
        button->loaded = true;
    }
    
    /* Defino el nuevo buffer o target */
    al_set_target_bitmap(button->bitmap);
    
    /* Fondo del bitmap */
    if( button->isPressed ){
        al_clear_to_color(button->focusColor);        
    }else if( button->isFocus ){
        al_clear_to_color(button->pressedColor);      
    }else{
        al_clear_to_color(button->backgroundColor);
    }
    
    /* Imprimo los bordes */
    al_draw_rectangle(1, 0, button->width, button->height-1, button->lineColor, 1);

    /* Imprimo el texto */
    al_draw_text(button->font, button->foregroundColor, (button->width - strlen(button->text)*8)/2, (button->height - button->fontSize)/2, 0, button->text);
    
    /* Devuelvo el buffer al target */
    al_set_target_backbuffer(display);
}
