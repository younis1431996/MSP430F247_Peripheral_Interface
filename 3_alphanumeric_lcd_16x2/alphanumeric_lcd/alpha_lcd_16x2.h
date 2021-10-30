/*
 * alpha_lcd_16x2.h
 *
 *  Created on: Oct 21, 2021
 *      Author: Mahmoude Mohamed Younis
 */

#ifndef ALPHA_LCD_16X2_H_
#define ALPHA_LCD_16X2_H_
/* include msp430f247 header file */
#include <msp430.h>

/* include c libraries */
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* include DIO pin definitions */
#include "inc/pin.h"

using namespace std;

/* include alphanumeric lcd commands */
#include "alpha_lcd_16x2_commands.h"

/* alphanumeric lcd 16x2 class */
class alpha_lcd_16x2{
public:
    /* initialization function */
    // control pins array as following ==> RS, RW & EN.
    // data pins array as following ==> D4, D5, D6 & D7.
    // lcd will be initialized to 4-bit modes & 5x8 dots.
    void init(volatile uint8_t *Data_dir, volatile uint8_t *Data_out,
              volatile uint8_t *Control_dir, volatile uint8_t *Control_out,
                       uint8_t data_pins[4], uint8_t control_pins[3]);
    /* clear lcd*/
    void clear(void){
        alpha_lcd_16x2 :: send_command(clear_display);
    }

    /* return to x=0, y=0 position */
    void return_home(void){
        alpha_lcd_16x2 :: send_command(Return_Home);
    }

    /* send command */
    void send_command(uint8_t command);

    /* print character */
    void print_char(uint8_t character);

    /* print character to a custom position */
    void print_char_to(uint8_t character, uint8_t r, uint8_t c);

    /* print string */
    void print_str(const char *str);

    /* print string to a custom position */
    void print_str_to(const char *str, uint8_t r, uint8_t c);

    /* print number */
    void print_num(double num);

    /* print number to a custom position */
    void print_num_to(double num, uint8_t r, uint8_t c);

    /* move cursor to x, y position */
    void move_cursor_to(uint8_t x, uint8_t y);

private:
    /* Hardware configuration */
    volatile uint8_t *data_dir;
    volatile uint8_t *data_out;
    volatile uint8_t *control_dir;
    volatile uint8_t *control_out;
             uint8_t  data_pin[4];
             uint8_t  RS;
             uint8_t  RW;
             uint8_t  EN;

     /* set data pin to logic one */
     void set_data_pin(uint8_t pin){
          *(alpha_lcd_16x2::data_out) |= (1<<pin);
      }

     /* reset data pin to logic zero */
      void clear_data_pin(uint8_t pin){
          *(alpha_lcd_16x2::data_out) &= ~(1<<pin);
      }
};
#endif /* ALPHA_LCD_16X2_H_ */
