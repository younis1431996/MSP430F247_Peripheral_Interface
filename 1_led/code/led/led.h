/*
 * led.h
 *
 *  Created on: Oct 21, 2021
 *      Author: Mahmoude Mohamed Younis
 */

#ifndef LED_LED_H_
#define LED_LED_H_

/* include msp430f247 register file definition */
#include <msp430.h>

/* include some helpful c files */
#include <stdint.h>

/* include pins definition file */
#include "inc/pin.h"

/* led class */
class _led{
public:
    // init.
    void init(volatile uint8_t *_d, volatile uint8_t *_o, uint8_t _p, uint8_t _s);

    // turn led on.
    void turn_on(void);

    // turn led off.
    void turn_off(void);

    // toggle led.
    void toggle(void);

private:
    volatile uint8_t *dir;
    volatile uint8_t *out;
             uint8_t pin;
             uint8_t state;
};



#endif /* LED_LED_H_ */
