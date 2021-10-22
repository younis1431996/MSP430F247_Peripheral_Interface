/*
 * main.cpp
 *
 *  Created on: Oct 21, 2021
 *      Author: Mahmoude Mohamed Younis
 */

/* include msp430f247 register file definition */
#include <msp430.h>

/* include msp430f247 GPIO pin definitions */
#include "inc/pin.h"

/* include button header file */
#include "button/button.h"

/* include led header file */
#include "led/led.h"

void main(void){
    /* stop watchdog timer */
	WDTCTL = WDTPW | WDTHOLD;

	/* create button object */
	_btn btn1;

	/* create led object */
	_led led1;

	/* init btn1 */
	btn1.init(&P6DIR, &P6IN, P6_5);

	/* init led1 */
	led1.init(&P2DIR, &P2OUT, P2_0, 1);

	/* endless loop */
	for(;;){
	    /* if condition satisfied, toggle led */
	    if(btn1.is_pressed()){
	        /* stability loop */
	        while(btn1.is_pressed());
	            // do nothing.

            /* toggle led1 */
	        led1.toggle();

	        /* insert nop for synchronization */
	        __asm__(" nop");
	    }
	}
}
