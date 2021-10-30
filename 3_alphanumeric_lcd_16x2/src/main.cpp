/*
 * main.cpp
 *
 *  Created on: Oct 21, 2021
 *      Author: Mahmoude Mohamed Younis
 */
/* include msp430f247 header file*/
#include <msp430.h>

/* include alphanumeric lcd header file */
#include "alphanumeric_lcd/alpha_lcd_16x2.h"

/* include DIO pin definitions */
#include "inc/pin.h"

// delay function.
static void wait(void){
    volatile uint32_t t;
    for(t = 150000; t>0; t--)
        asm(" nop");
}

/* main function */
void main(void)
{
    /* stop watchdog timer*/
    WDTCTL = WDTPW | WDTHOLD;

    /* alphanumeric lcd 16x2 object */
    alpha_lcd_16x2 lcd;

    /* lcd init */
    uint8_t data_pins[4] = {P4_0, P4_1, P4_2, P4_3};
    uint8_t control_pins[3] = {P5_0, P5_1, P5_2};
    lcd.init(&P4DIR, &P4OUT, &P5DIR, &P5OUT, data_pins, control_pins);

    /* stream scenario */
    lcd.print_str("$deriver by.");
    lcd.print_str_to("Mahmoude Mohamed", 1, 0);
    wait();

    lcd.clear();
    lcd.return_home();
    lcd.print_str("Now print char");
    lcd.print_char_to('x', 1, 0);
    wait();

    lcd.clear();
    lcd.return_home();
    lcd.print_str("Now print str");
    lcd.print_str_to("Hello world", 1, 0);
    wait();

    lcd.clear();
    lcd.return_home();
    lcd.print_str("Now print number");
    lcd.print_num_to(898, 1, 0);
    wait();

    lcd.clear();
    lcd.return_home();
    lcd.print_str("Thank you ^_^");

    /* endless loop */
    for(;;);
}




