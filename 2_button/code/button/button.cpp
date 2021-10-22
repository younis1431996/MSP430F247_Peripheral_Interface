/*
 * button.cpp
 *
 *  Created on: Sep 23, 2021
 *      Author: Mahmoude Mohamed Younis
 */
/* inclusion */
#include "button.h"

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function	$ init.
 *
 * \ arguments	$ _d   = Data Direction Register.
 * 				$ _in  = Input Port.
 * 				$ _p  = connected pin's number.
 *
 * \ details	$ initialize button.
 *
 * \ return		$ void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void _btn :: init(volatile uint8_t *_d, volatile uint8_t *_in, uint8_t _p){
	// Save button hardware configuration.
    _btn::dir = _d;
    _btn::in  = _in;
    _btn::pin = _p;

	// configure pin as an input.
	*(_btn::dir) &= ~(1<<_btn::pin);

	// Insert nop for synchronization
	__asm__(" nop");
}

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function	$ is-pressed.
 *
 * \ arguments	$ void.
 *
 * \ details	$ check if button is pressed or not.
 *
 * \ return		$ bool.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
bool _btn :: is_pressed(void){
	return _btn :: read_pin();
}

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function	$ read_pin.
 *
 * \ arguments	$ void.
 *
 * \ details	$ read input data register.
 *              $ mask register to get pin value.
 *              $ bool register value after masking to decide if 1 or 0.
 *
 * \ return		$ bool.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
bool _btn :: read_pin(void){
	// PIN register value.
	uint8_t PINR_value = 0x00;

	// connected pin value.
	bool pin_value;

	// Read PIN register.
	PINR_value = *(_btn::in);

	// Mask to read the specific pin.
	pin_value = (bool) (PINR_value & (1<<_btn::pin));

	// Insert nop for synchronization
	__asm__(" nop");

	return pin_value;
}



