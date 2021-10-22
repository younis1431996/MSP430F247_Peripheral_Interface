/*
 * button.h
 *
 *  Created on: Sep 23, 2021
 *      Author: Mahmoude Mohamed Younis
 */

#ifndef BUTTON_BUTTON_H_
#define BUTTON_BUTTON_H_

/* include msp430f247 register file definition */
#include <msp430.h>

/* include msp430f247 GPIO pin definitions */
#include "inc/pin.h"

/* include c library */
#include <stdint.h>

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \class		- button
 *
 * \details		- Class controls buttons.
 * 				- Can control button by initialization, is-pressed, turn-off, state functions.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
class _btn{
public:
    // init.
	void init(volatile uint8_t *_d, volatile uint8_t *_in, uint8_t _p);

	// check if button is pressed.
	bool is_pressed(void);

private:
	volatile uint8_t  *dir;
	volatile uint8_t  *in;
			 uint8_t   pin;

	// read inserted value.
	bool read_pin(void);
};



#endif /* BUTTON_BUTTON_H_ */
