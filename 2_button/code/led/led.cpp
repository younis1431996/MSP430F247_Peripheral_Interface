/*
 * led.cpp
 *
 *  Created on: Oct 21, 2021
 *      Author: Mahmoude Mohamed Younis
 */
/* include led header file */
#include "led.h"

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   - init.
 *
 * \ arguments  $ _d  = Data Direction Register.
 *              $ _o  = Output Port.
 *              $ _p  = pin number.
 *              $ _s  = led state.
 *
 * \ details    $ Initialize led according to input state.
 *              $ _s = 0 for pull-up connected led.
 *              $ _s = 1 for pull-down connected led.
 *
 * \ return     $ void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void _led :: init(volatile uint8_t *_d, volatile uint8_t *_o, uint8_t _p, uint8_t _s){
    /* Hardware configuration */
    _led :: dir = _d;       // direction register.
    _led :: out = _o;       // output port.
    _led :: pin = _p;       // pin number.
    _led :: state = _s;     // led state.

    /* configure pin as output pin. */
    *(_led :: dir) |= (1<<pin);

    /* insert nop for synchronization */
    __asm__("nop");

    /* led is initially low. */
    if(_led::state == 0)
        *(_led :: out) |= (1<<_led::pin);
    else if(_led::state == 1)
        *(_led :: out) &= ~(1<<_led::pin);

}

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   - turn_on.
 *
 * \ arguments  - void.
 *
 * \ details    - turn on led.
 *
 * \ return     - void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void _led :: turn_on(void){
    // according to state.
    if(_led::state == 0)
        *(_led::out) &= ~(1<<_led::pin);
    else if(_led::state == 1)
        *(_led::out) |= (1<<_led::pin);
}

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   - toggle.
 *
 * \ arguments  - void.
 *
 * \ details    - toggle led.
 *
 * \ return     - void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void _led :: toggle(void){
    // according to state.
    *(_led::out) ^= (1<<_led::pin);
}

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   - turn_off.
 *
 * \ arguments  - void.
 *
 * \ details    - turn off led.
 *
 * \ return     - void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void _led :: turn_off(void){
    // according to state.
    if(_led::state == 0)
        *(_led::out) |=  (1<<_led::pin);
    else if(_led::state == 1)
        *(_led::out) &= ~(1<<_led::pin);
}



