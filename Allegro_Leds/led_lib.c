/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   led_lib.c
 * Author: pablo
 * 
 * Created on 24 de mayo de 2018, 14:10
 */

#include "led_lib.h"


/*draw*/
void draw (LEDS * ref, WINDOW * window){
    BIT * pBit;
    
    al_draw_scaled_bitmap(window->imageBackground,          //Imagen de fondo
        0, 0,                                               // source origin
        al_get_bitmap_width(window->imageBackground),       // source width
        al_get_bitmap_height(window->imageBackground),      // source height
        0, 0 ,                                              // target origin
        SCREEN_W, SCREEN_H,                                 // target dimensions
        0                                                   // flags
        );
    ALLEGRO_COLOR color =al_map_rgb(255, 255,255);
    
    for( pBit= &(ref->bit7); pBit>= &(ref->bit0); pBit--) {
    al_draw_scaled_bitmap(window->imagePlate,               //Imagen de cada "plato"
        0, 0,                                               // source origin
        al_get_bitmap_width(window->imagePlate),            // source width
        al_get_bitmap_height(window->imagePlate),           // source height
        pBit->x - PLATE_W/2, pBit->y - PLATE_H/2,           // target origin
        PLATE_W, PLATE_H,                                   // target dimensions
        0                                                   // flags
        );
        if(pBit->on){
            al_draw_filled_circle (pBit->x,pBit->y, pBit->ratio, pBit->onColor);    //LED
            
            
        }else{
            al_draw_filled_circle (pBit->x,pBit->y, pBit->ratio, pBit->offColor);   //LED
        }
    }
   
    al_draw_text(window->titleFont, color, 440, 40, ALLEGRO_ALIGN_CENTRE, "mix the beat");
    al_draw_text(window->genFont, color, 80, 120, ALLEGRO_ALIGN_LEFT, "Instrucciones:" );
    al_draw_text(window->genFont, color, 100, 135, ALLEGRO_ALIGN_LEFT, "Ingrese numero del plato o cliqueelo para reproducir o parar su sonido." );
    al_draw_text(window->genFont, color, 100, 150, ALLEGRO_ALIGN_LEFT, "ingrese t para invertir estados" );
    al_draw_text(window->genFont, color, 100, 165, ALLEGRO_ALIGN_LEFT, "ingrese b para iniciar parpadeo" );
    al_draw_text(window->genFont, color, 100, 180, ALLEGRO_ALIGN_LEFT, "ingrese c para parar todos los sonidos" );
    al_draw_text(window->genFont, color, 100, 195, ALLEGRO_ALIGN_LEFT, "ingrese s para reproducir todos los sonidos" );
     al_draw_text(window->genFont, color, 100, 210, ALLEGRO_ALIGN_LEFT, "ingrese q para salir." );
    
    
    
    al_flip_display();
    
}

/*key_pressed*/
void key_pressed (int unichar, LEDS * ref)
{
    char key = (char) unichar;
    BIT * pBit = & (ref -> bit0);
    char bit;
    if((key >= 48) && (key <= 57))      //Si se apreto un numero
    {
        bit = (key - 48);               //Pasa el ascii a su valor real
        pBit += bit;
        toggleBit (pBit, ref);          //Invierte el estado del led.
        

    } else
    {
      switch (key) 
      {
        case 't':case 'T':
            
            allToggle (ref);
            break;
            
        case 'c':case 'C':
            
            allOff (ref);
            
            for( pBit= &(ref->bit7); pBit>= &(ref->bit0); pBit--)   //Itera cada led
            {
                pBit->toggle= false;                                //Ningun led en estado de parpadeo
            }
            
            break;
            
        case 's':case 'S':
            
            allOn (ref);
            break;
            
        case 'q':case 'Q':
            
            //endProgram();
            break;
            
        case 'b':case 'B':
            
            if (ref->toggleFlag)            //Si el programa se encuentra en estado de parpadeo
                
            {
                ref->toggleFlag = false;
                onBlink(ref);               //Dejara de parpadear
                
            } else                          //Si el programa no se encuentra en estado de parpadeo
            {
                ref->toggleFlag = true;  
                onBlink(ref);               //Parpadeo de leds
            }
            
            break;
      }
    }
}

/*allToggle*/
void allToggle (LEDS * ref) {
    
    BIT * pBit;
    for( pBit= &(ref->bit7); pBit>= &(ref->bit0); pBit--)       //Itera cada led
    {     
        toggleBit (pBit, ref);
    }
    
}

/*allOn*/
void allOn (LEDS * ref) {
    
    BIT * pBit;
    for( pBit= &(ref->bit7); pBit>= &(ref->bit0); pBit--)                                           //Itera cada led
    {
        //al_play_sample(pBit->sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &(pBit->id_sample));     //Reproduce el sonido asociado al led
        pBit -> on = true;
    }
    
}

/*allOff*/
void allOff (LEDS * ref) {
    
    BIT * pBit;
    for( pBit= &(ref->bit7); pBit>= &(ref->bit0); pBit--)    //Itera cada led
    {
        //al_stop_sample(&(pBit->id_sample));               //Stop sonido del led
        pBit -> on = false;
        
    }    
    
}

/*toggleBit*/
void toggleBit (BIT * pBit, LEDS * ref)
{
    if( pBit->on )                                                //Si el led esta encendido
        {
            //al_stop_sample(&(pBit->id_sample));

            if(ref->toggleFlag)                                  //Si el programa se encuentra en estado de parpadeo
            {
                pBit->toggle = false;                            //El led deja de parpadear.
                allOff (ref);                                    //Apagara todos (se volveran a encender los que se encontraban parpadeando)
                                                                 //Para que sigan parpadeando sincronizadamente.
                
            }else                                                //Si no hay parpadeo
            {
               pBit->on= false;                                  //Apaga el led
            }
            
            
        } else                                                  //Si esta apagado
        {
            //al_play_sample(pBit->sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &(pBit->id_sample));
        
            if(ref->toggleFlag)                                //Si el programa se encuentra en estado de parpadeo 
            {
                if(pBit->toggle)                               //Si el bit esta apagado debido al parpadeo
                {
                    allOff (ref);                              
                    pBit->toggle = false;                      //Se apagaran todos (se volveran a encender los que se encontraban parpadeando excepto el seleccionado)
                                                               //Para que sigan parpadeando sincronizadamente.
                    
                } else                                         //Si el bit esta apagado sin parpadeo mientras el programa esta en estado de parpadeo.
                {
                    allOff (ref);                              
                    pBit->toggle = true;                       //Se apagaran todos (se volveran a encender los que se encontraban parpadeando mas el seleccionado)
                                                               //Para que sigan parpadeando sincronizadamente.
                }
                
                
            }else                                              //Si esta apagado y el programa sin parpadeo.
            {
               pBit->on= true;                                 //Se enciende
            }   
            
        }
}

/*onBlink*/
void onBlink (LEDS * ref) {
    BIT * pBit;
    
    for( pBit= &(ref->bit7); pBit>= &(ref->bit0); pBit--)           //Itera cada led
    {
        if (pBit->on)                           
        {
            if (ref->toggleFlag == true)                            //Si hay estado de parpadeo y el led esta encendido
            {
                pBit-> on= false;                                   //Se apaga el led
               // al_stop_sample(&(pBit->id_sample));               //Stop a todo el sonido
                pBit-> toggle = true;
            } else 
            {
                pBit->toggle = false;                               //Si no hay estado de parpadeo, ningun led parpadeara.
            }
        } else
        {
            if(pBit->toggle)                                        //Si el led tiene estado de parpadeo y el led apagado.
            {
                //al_play_sample(pBit->sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP, &(pBit->id_sample));
                pBit->on=true;                                      //Se enciende el led
                if (ref->toggleFlag == false)                       //Si no hay estado de parpadeo en el programa
                {
                    pBit->toggle=false;                             //El led dejara de tener estado de parpado.
                }
            }
        }
    }   
}

/*make_led_linear_h_layout*/
void make_led_linear_h_layout (LEDS * ref, int y, int dist, int first_x, int ratio, ALLEGRO_COLOR onColor, ALLEGRO_COLOR offColor){
    
    BIT * pBit;

    int x= first_x;
    int distance = dist + PLATE_W;
    
    for( pBit= &(ref->bit7); pBit>= &(ref->bit0); pBit--) {
        pBit->y=y;
        pBit->ratio = ratio;
        pBit->onColor = onColor;
        pBit->offColor = offColor;
        pBit->on=false;
        pBit->x=x;
        pBit->toggle=false;
        pBit->id_sample=NULL;
        x+=distance; 
       
    }
    ref->toggleFlag=false;
}

/*start_allegro*/
bool start_allegro (WINDOW * window, LEDS * ref ){
        BIT * pBit;
        char i = 0;
        if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return false;
	}
        al_init_font_addon();
        al_init_primitives_addon();
        al_init_ttf_addon();
        al_init_image_addon();
        al_init_acodec_addon();
        
        
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return false;
	}
        
        if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        return false;
        }
        
      
 
        if (!al_reserve_samples(8)){
        fprintf(stderr, "failed to reserve samples!\n");
        return false;
        }
        
        for( pBit= &(ref->bit0); pBit<= &(ref->bit7); pBit++) {
        
            pBit->sample = al_load_sample ( (window->samples[i++]) );
            if (!pBit->sample){
            printf( "Audio clip sample not loaded!\n" ); 
            return false;
            }
        }

	window->event_queue = al_create_event_queue();
	if(!window->event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		return false;
	}
        window->timer = al_create_timer(15.0 / FPS);
	if(!window->timer) {
		fprintf(stderr, "failed to create timer!\n");
		return false;
	}
        
        if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize the mouse!\n");
		return false;
	}
 
	window->display = al_create_display(SCREEN_W, SCREEN_H);
	if(!window->display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_event_queue(window->event_queue);
		return false;
	}
        window->imagePlate = al_load_bitmap("plate.png");
	if(!window->imagePlate) {
            fprintf(stderr, "failed to load image !\n");
            return false;
	}
        
        window->imageBackground = al_load_bitmap("fondo2.jpg");
	if(!window->imageBackground) {
            fprintf(stderr, "failed to load image !\n");
            return false;
	}
        
        window->titleFont= al_load_ttf_font ("fontdj.ttf", 55 , 0);
        if(!window->titleFont) {
            fprintf(stderr, "failed to load font !\n");
            return false;
	}
        
        window->genFont= al_load_ttf_font ("soloist.ttf", 9, 0);
        if(!window->genFont) {
            fprintf(stderr, "failed to load font !\n");
            return false;
	}
        
        al_set_target_bitmap(al_get_backbuffer(window->display));
  
	al_register_event_source(window->event_queue, al_get_display_event_source(window->display));
        al_register_event_source(window->event_queue, al_get_timer_event_source(window->timer));
	al_register_event_source(window->event_queue, al_get_keyboard_event_source()); 
        al_register_event_source(window->event_queue, al_get_mouse_event_source());
 
	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();
        al_start_timer(window->timer);
}

/*destroy*/
void destroy (WINDOW * window)
{
    BIT * pBit;

    al_destroy_timer(window->timer);
    al_destroy_display(window->display);
    al_destroy_bitmap(window->imagePlate);
    al_destroy_event_queue(window->event_queue);
    al_uninstall_audio();
    
}

/*mouse_click*/
void mouse_click (int x, int y, LEDS *ref)
{
    BIT * pBit;
    for( pBit= &(ref->bit7); pBit>= &(ref->bit0); pBit--) 
    {
        if(((pBit->x)-(PLATE_W/2) <= x) && ((pBit->x)+(PLATE_W/2) >= x) && ((pBit->y)-(PLATE_H/2) <= y) && ((pBit->y)+(PLATE_H/2) >= y))
        {
            toggleBit (pBit, ref);
        }
    }
}