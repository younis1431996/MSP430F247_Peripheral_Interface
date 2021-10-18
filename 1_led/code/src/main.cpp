/*
 * main.cpp
 *
 *  Created on: Oct 19, 2021
 *      Author: Mahmoude Mohamed Younis
 */
/* inclusion */
#include <msp430.h>

/* delay function*/
void delay(volatile unsigned long time){
    for(volatile unsigned long i=time; i>0; i--){
        // do nothing.
    }
}

/* main function */
void main(void){
    /* stop Watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /* configure P1.1 as output */
    P1DIR |= 0x02;

    /* endless loop */
    for(;;){
        P1OUT ^= 0x02;
        delay(50000);
    }
} /* end of main function */


