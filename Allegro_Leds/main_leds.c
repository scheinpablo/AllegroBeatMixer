/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "led_lib.h"


/* 
 * File:   main_leds.c
 * Author: pablo
 *
 * Created on 23 de mayo de 2018, 11:05
 */


/*
 * 
 */
int main(int argc, char** argv) {
    
    WINDOW mainWindow;
    LEDS portA;
    
    mainWindow.imagePlate = NULL;
    mainWindow.display = NULL;
    mainWindow.event_queue = NULL;
    mainWindow.imageBackground = NULL;
    mainWindow.timer = NULL;
    mainWindow.titleFont = NULL;
    
    mainWindow.samples[0]="base1.ogg";  //DEFINO LOS SONIDOS QUE REPRODUCIRA CADA LED
    mainWindow.samples[1]="base2.ogg";
    mainWindow.samples[2]="base3.ogg";
    mainWindow.samples[3]="base4.ogg";
    mainWindow.samples[4]="base5.ogg";
    mainWindow.samples[5]="base6.ogg";
    mainWindow.samples[6]="base7.ogg";
    mainWindow.samples[7]="base8.ogg";
    
    bool do_exit = false; 
    bool redraw = false; 
    
    ALLEGRO_COLOR onColor, offColor;
    
    start_allegro(&mainWindow, &portA);
    
    onColor = al_map_rgb(0, 255, 0);
    offColor = al_map_rgb(255,0, 0);
    
    make_led_linear_h_layout (&portA, 310, 10, 100, PLATE_W * 0.19, onColor, offColor);
    draw (&portA, &mainWindow);
    
    while(!do_exit)
	{
            ALLEGRO_EVENT ev;
            if( al_get_next_event(mainWindow.event_queue, &ev) ) //Toma un evento de la cola.
            { 

                if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
                {
                        do_exit = true;
                        
                } else if(ev.type == ALLEGRO_EVENT_KEY_CHAR) 
                {
                    
                    key_pressed (ev.keyboard.unichar, &portA);
                    redraw = true;
                    
                } else if (ev.type == ALLEGRO_EVENT_TIMER)
                {
                    
                    if (portA.toggleFlag){
                        onBlink(&portA);        //GENERA UN PARPADEO EN LOS LEDS CADA UN TIEMPO DETERMINADO
                        redraw = true;
                    }
                    
                } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    mouse_click (ev.mouse.x, ev.mouse.y, &portA);
                    redraw = true;
                }
            }

            if(redraw && al_is_event_queue_empty(mainWindow.event_queue)) 
            {  
                redraw = false;
                al_clear_to_color(al_map_rgb(0,0,0));
                draw (&portA, &mainWindow);
                al_flip_display();
            }
                
                
	}
    destroy (&mainWindow);
    
    return (EXIT_SUCCESS);
    
}















