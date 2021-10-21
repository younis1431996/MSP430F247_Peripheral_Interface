/*
 * main.cpp
 *
 *  Created on: Oct 21, 2021
 *      Author: Mahmoude Mohamed Younis
 */
/* include msp430f247 register file definition */
#include <msp430.h>

/* include pins definition file */
#include "inc/pin.h"

/* include led header file*/
#include "led/led.h"

/* function generates delay time */
void delay(void){
    unsigned long i=100000;

    /* delay loop */
    while(i>0){
        __asm__(" nop");
        i--;
    }

    /* exit */
    return;
}
/* main function */
void main(void){
    /*  stop watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /* pull-up led object */
    _led led1;

    /* pull-down led object */
    _led led2;

    /* init led1 */
    led1.init(&P1DIR, &P1OUT, P1_1, 0);

    /* init led2 */
    led2.init(&P3DIR, &P3OUT, P3_2, 1);

    /* endless loop*/
    for(;;){
        led1.toggle();
        led2.toggle();
        delay();
    }
}



