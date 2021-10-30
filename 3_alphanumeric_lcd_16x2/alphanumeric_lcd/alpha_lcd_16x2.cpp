/*
 * alpha_lcd_16x2.cpp
 *
 *  Created on: Oct 21, 2021
 *      Author: Mahmoude Mohamed Younis
 */
/* include lcd 16x2 header file */
#include "alpha_lcd_16x2.h"

/* include lcd 16x2 command header file */
#include "alpha_lcd_16x2_commands.h"

/* delay function */
static void delay(volatile uint16_t time){
    for(time *= 7; time>0; time--)
        asm(" nop");
}

/* $$ Initialization function $$
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   $ init.
 *
 * \ arguments  $ volatile uint8_t *Data_dir = Data bus direction register.
 *              $ volatile uint8_t *Data_out = Data bus output register.
 *              $ volatile uint8_t *Control_dir = Control bus direction register.
 *              $ volatile uint8_t *Control_out = Control bus output register.
 *              $ uint8_t data_pins[4] = Data pins; {D4, D5, D6, D7}.
 *              $ uint8_t control_pins[3] = Control pins; {RS, RW, EN}.
 *
 * \ details    $ 1.Wait for 15ms, Power-on initialization time for LCD16x2.
 *              $ 2.Send 0x02 command which initializes LCD 16x2 in 4-bit mode.
 *              $ 3.Send 0x28 command which configures LCD in 2-line, 4-bit mode, and 5x8 dots.
 *              $ 4.Send any Display ON command (0x0E, 0x0C)
 *              $ 5.Send 0x06 command (increment cursor)
 *
 * \ return     $ void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void alpha_lcd_16x2 :: init(volatile uint8_t *Data_dir, volatile uint8_t *Data_out, volatile uint8_t *Control_dir, volatile uint8_t *Control_out, uint8_t data_pins[4], uint8_t control_pins[3]){
    /* 1.set Hardware configuration */
    // Data bus.
    alpha_lcd_16x2::data_dir = Data_dir;
    alpha_lcd_16x2::data_out = Data_out;
    for(uint8_t i=0; i<4; i++){
        // set data pins.
        alpha_lcd_16x2::data_pin[i] = data_pins[i];

        // configure as output.
        *(alpha_lcd_16x2::data_dir) |= (1<<alpha_lcd_16x2::data_pin[i]);
    }

    // Control bus.
    alpha_lcd_16x2::control_dir = Control_dir;
    alpha_lcd_16x2::control_out = Control_out;
    alpha_lcd_16x2::RS = control_pins[0];
    alpha_lcd_16x2::RW = control_pins[1];
    alpha_lcd_16x2::EN = control_pins[2];
    // configure control pins as output.
    *(alpha_lcd_16x2::control_dir) |= (1<<alpha_lcd_16x2::RS);
    *(alpha_lcd_16x2::control_dir) |= (1<<alpha_lcd_16x2::RW);
    *(alpha_lcd_16x2::control_dir) |= (1<<alpha_lcd_16x2::EN);

    /* 2.LCD Power ON delay always > 15 ms */
    delay(20);

    /* 3.send init command */
    alpha_lcd_16x2::send_command(__init__);
    delay(1);

    /* 4.4-bit mode, 2-lines and 5x8 dots */
    alpha_lcd_16x2::send_command(_4_bits_2_lines_5x8_dots);
    delay(1);

    /* 5.display on, cursor on, no blinking */
    alpha_lcd_16x2::send_command(display_on_cursor_on_no_blinking);
    delay(1);

    /* 6.cursor will shift to the right, writing direction is the shift to right */
    alpha_lcd_16x2::send_command(increment_cursor);
    delay(1);

    /* 7.clear display */
    alpha_lcd_16x2::send_command(clear_display);
    delay(1);

    /* 8.return to home */
    alpha_lcd_16x2::send_command(Return_Home);
    delay(1);

    /* exit */
    return;
}

/* $$ send command function $$
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   $ send_command.
 *
 * \ arguments  $ uint8_t command
 *
 * \ details    $ 1. First, send a Higher nibble of command.
 *              $ 2. Make RS pin low, RS=0 (command REG.)
 *              $ 3. Make RW pin low, RW=0 (write operation) or connect it to ground.
 *              $ 4. Give High to Low pulse at Enable (E).
 *              $ 5. Send lower nibble of command.
 *              $ 6. Give High to Low pulse at Enable (E).
 *
 * \ return     $ void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void alpha_lcd_16x2 :: send_command(uint8_t command){
    /* store command in 8-bit register. */
    bool cmd_arr[8];
    for (uint8_t i=0; i<8; i++){
        cmd_arr[i] = (bool) (command&(1<<i));
    }

    /* sending upper nibble. */
    for(uint8_t i=4; i<=7; i++){
        if(cmd_arr[i] == true)
            alpha_lcd_16x2::set_data_pin(alpha_lcd_16x2::data_pin[i-4]);
        else if(cmd_arr[i] == false)
            alpha_lcd_16x2::clear_data_pin(alpha_lcd_16x2::data_pin[i-4]);

        /* Synchronization */
        __asm__(" nop");
    }

    /* RS=0, command register. */
    *(alpha_lcd_16x2::control_out) &= ~(1<<alpha_lcd_16x2::RS);

    /* Enable pulse */
    *(alpha_lcd_16x2::control_out) |= (1<<alpha_lcd_16x2::EN);
    delay(1);
    *(alpha_lcd_16x2::control_out) &= ~ (1<<alpha_lcd_16x2::EN);
    delay(1);

    /* Sending lower nibble */
    for(uint8_t i=0; i<=3; i++){
        if(cmd_arr[i] == true)
            alpha_lcd_16x2::set_data_pin(alpha_lcd_16x2::data_pin[i]);
        else if(cmd_arr[i] == false)
            alpha_lcd_16x2:: clear_data_pin(alpha_lcd_16x2::data_pin[i]);

        /* Synchronization */
        __asm__(" nop");
    }

    /* RS=0, command register. */
    *(alpha_lcd_16x2::control_out) &= ~(1<<alpha_lcd_16x2::RS);

    /* Enable pulse */
    *(alpha_lcd_16x2::control_out) |= (1<<alpha_lcd_16x2::EN);
    delay(1);
    *(alpha_lcd_16x2::control_out) &= ~ (1<<alpha_lcd_16x2::EN);
    delay(1);

    /* exit */
    return;
}

/* $$ print character function $$
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   $ print_character.
 *
 * \ arguments  $ uint8_t character
 *
 * \ details    $ 1. First, send a Higher nibble of character.
 *              $ 2. Make RS pin low, RS=1 (data REG.)
 *              $ 3. Make RW pin low, RW=0 (write operation) or connect it to ground.
 *              $ 4. Give High to Low pulse at Enable (E).
 *              $ 5. Send lower nibble of character.
 *              $ 6. Give High to Low pulse at Enable (E).
 *
 * \ return     $ void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void alpha_lcd_16x2 :: print_char(uint8_t character){
    /* store character in 8-bit register. */
    bool char_arr[8];
    for (uint8_t i=0; i<8; i++){
        char_arr[i] = (bool) (character&(1<<i));
    }

    /* sending upper nibble. */
    for(uint8_t i=4; i<=7; i++){
        if(char_arr[i] == true)
            alpha_lcd_16x2::set_data_pin(alpha_lcd_16x2::data_pin[i-4]);
        else if(char_arr[i] == false)
            alpha_lcd_16x2::clear_data_pin(alpha_lcd_16x2::data_pin[i-4]);

        /* Synchronization */
        __asm__(" nop");
    }

    /* RS=1, data register. */
    *(alpha_lcd_16x2::control_out) |= (1<<alpha_lcd_16x2::RS);

    /* Enable pulse */
    *(alpha_lcd_16x2::control_out) |= (1<<alpha_lcd_16x2::EN);
    delay(1);
    *(alpha_lcd_16x2::control_out) &= ~ (1<<alpha_lcd_16x2::EN);
    delay(1);

    /* Sending lower nibble */
    for(uint8_t i=0; i<=3; i++){
        if(char_arr[i] == true)
            alpha_lcd_16x2::set_data_pin(alpha_lcd_16x2::data_pin[i]);
        else if(char_arr[i] == false)
            alpha_lcd_16x2:: clear_data_pin(alpha_lcd_16x2::data_pin[i]);

        /* Synchronization */
        __asm__(" nop");
    }

    /* RS=1, data register. */
    *(alpha_lcd_16x2::control_out) |= (1<<alpha_lcd_16x2::RS);

    /* Enable pulse */
    *(alpha_lcd_16x2::control_out) |= (1<<alpha_lcd_16x2::EN);
    delay(1);
    *(alpha_lcd_16x2::control_out) &= ~ (1<<alpha_lcd_16x2::EN);
    delay(1);

    /* exit */
    return;
}

/* $$ move cursor function $$
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   $ move_cursor_to.
 *
 * \ arguments  $ uint8_t x = row number.
 *              $ uint8_t y = column number.
 *
 * \ details    $ decide what is row should to go.
 *              $ mask y to get the specific column.
 *
 * \ return     $ void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void alpha_lcd_16x2 :: move_cursor_to(uint8_t x, uint8_t y){
    if (x == 0 && y<16)
        /* Command of first row and required position<16 */
        alpha_lcd_16x2 :: send_command( (y & 0x0F)|force_cusror_to_start_with_1st_row);
    /* Command of second row and required position<16 */
    else if (x == 1 && y<16)
        alpha_lcd_16x2 :: send_command( (y & 0x0F)|force_cusror_to_start_with_2nd_row);

    // exit
    return;
}

/* $$ print character to custom position function $$
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   $ print_character.
 *
 * \ arguments  $ uint8_t character
 *              $ uint8_t r = row position and was initialized to 0.
 *              $ uint8_t c = column position and was initialized to 0.
 *
 * \ details    $ 1. move cursor to r & c position.
 *              $ 2. print character.
 *
 * \ return     $ void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void alpha_lcd_16x2 :: print_char_to(uint8_t character, uint8_t r, uint8_t c){
    // move cursor to r & c position.
    alpha_lcd_16x2 :: move_cursor_to(r, c);

    // print character.
    alpha_lcd_16x2 :: print_char(character);

    // exit
    return;
}

/* $$ print string function $$
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   $ print_str.
 *
 * \ arguments  $ string.
 *
 * \ details    $ 1. get string's length.
 *              $ 2. print each character.
 *              $ 3. check string length if higher than 16-digit.
 *
 * \ return     $ void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void alpha_lcd_16x2 :: print_str(const char *str){
    uint8_t i=0;
    for(i=0; i<strlen(str); i++){
        if(i<16){
            alpha_lcd_16x2 :: print_char(str[i]);
        }
        else{
            alpha_lcd_16x2 :: move_cursor_to(1, i-16);
            alpha_lcd_16x2 :: print_char(str[i]);
        }
    }

    // exit
    return;
}

/* $$ print string to a custom position function $$
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   $ print_str_to.
 *
 * \ arguments  $ string.
 *              $ uint8_t r = row number.
 *              $ uint8_t c = column number.
 *
 * \ details    $ 1. move cursor to r & c position.
 *              $ 2. print string.
 *
 * \ return     $ void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void alpha_lcd_16x2 :: print_str_to(const char *str, uint8_t r, uint8_t c){
    // move cursor to r & c position.
    alpha_lcd_16x2 :: move_cursor_to(r, c);

    // print string.
    alpha_lcd_16x2 :: print_str(str);

    // exit
    return;
}

/* $$ print number function $$
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * \ function   $ print_num.
 *
 * \ arguments  $ double num.
 *
 * \ details    $ 1. convert number into string.
 *              $ 2. print number.
 *
 * \ return     $ void.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
/* >> function to convert int number into string. */
static void int_to_str(long number, char *buffer){
    // convert integer number into string.
    ltoa(number, buffer);

    // exit.
    return;
}

static void float_to_str(double number, char *buffer, bool *after_point){
    /* used in float conversion. */
    double _10_pow[5] = {10.0, 100.0, 1000.0, 10000.0, 100000.0};
    double float_part = 0;
    double result = 0;

    /* check if float number is 0*/
    if(number == 0){
        *after_point = 0;
        return;
    }
    /* loop to extract float part and convert it to integer. */
    int i = 0;
    char temp_buffer[5] = "\0";
    while(i<5){
        float_part = number * _10_pow[i];
        result = float_part - (int)float_part;
        if( ((int)(float_part)) == 0){
            buffer[i] = '0';
        }
        if(result == 0){
            break;
        }
        i++;
    }

    /* convert float part into string. */
    ltoa((long)float_part, temp_buffer);
    strcat(buffer, temp_buffer);

    /* after point !=0 */
    *after_point = 1;

    return;
}
void alpha_lcd_16x2 :: print_num(double num){
    /* int part string. */
    char __int_part_buffer[7];

    /* float part string. */
    char __float_part_buffer[9];

    /* total number buffer. */
    char total_number_buffer[17];

    /* check if negative */
    if(num < 0 ){
        num *= -1;
        strcat(total_number_buffer, "-");
    }
    /* if number is integer. */
    if( (num-(long)num) == 0){
        int_to_str(num, __int_part_buffer);
        alpha_lcd_16x2 :: print_str(__int_part_buffer);
        return;
    } // _END_IF_
    else{
        bool after_point = false;
        /* Extract float part. */
        double float_part = num - (long)(num);

        /* temp string. */
        char temp_buffer[9];

        /* convert float number */
        float_to_str(float_part, temp_buffer, &after_point);

        if(after_point == 0){
            /* convert int number. */
            int_to_str(num, __int_part_buffer);
            alpha_lcd_16x2 :: print_str(__int_part_buffer);
            return;
        } // _END_IF_
        else{
            /* add dot {.} */
            __float_part_buffer[0] = '.';
            strcat(__float_part_buffer, temp_buffer);

            /* convert int number. */
            int_to_str(num, __int_part_buffer);
            strcat(total_number_buffer, __int_part_buffer);

            strcat(total_number_buffer, __float_part_buffer);
            alpha_lcd_16x2 :: print_str(total_number_buffer);
        } // _END_ELSE_
    } // _END_ELSE_

    // exit.
    return;
}

/* print number to a custom position */
void alpha_lcd_16x2 :: print_num_to(double num, uint8_t r, uint8_t c){
    // move cursor.
    alpha_lcd_16x2 :: move_cursor_to(r, c);

    // print number.
    alpha_lcd_16x2 :: print_num(num);

    // exit.
    return;
}
