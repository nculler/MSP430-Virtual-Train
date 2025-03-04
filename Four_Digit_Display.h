/*
    Four_Digital_Display.h
    This is a Suli-compatible Library.
    2014 Copyright (c) Seeed Technology Inc.  All right reserved.
    Author:Loovee
    2014-4-9
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef __FOUR_DIGIT_DISPLAY_H__
#define __FOUR_DIGIT_DISPLAY_H__

#include <stdint.h>
#include <msp430.h>
#include <stdbool.h>

//************definitions for TM1637*********************
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44

#define STARTADDR  0xc0
/**** definitions for the clock point of the digit tube *******/
#define POINT_ON   1
#define POINT_OFF  0
/**************definitions for brightness***********************/
//#define  BRIGHT_DARKEST 0
//#define  BRIGHT_TYPICAL 2
//#define  BRIGHTEST      7

enum brightness_t {
    BRIGHT_DARKEST = 0,
    BRIGHT_TYPICAL = 2,
    BRIGHTEST      = 7
};

// Seven segment display segment mappings
#define SEG_A 0b00000001
#define SEG_B 0b00000010
#define SEG_C 0b00000100
#define SEG_D 0b00001000
#define SEG_E 0b00010000
#define SEG_F 0b00100000
#define SEG_G 0b01000000

// Digit positions
#define POS_1 0
#define POS_2 1
#define POS_3 2
#define POS_4 3

// Pin connections
#define DATA_PIN        4
#define CLK_PIN         1
#define DATA_DIR_REG    P1DIR
#define CLK_DIR_REG     P1DIR
#define DATA_OUT_REG    P1OUT
#define CLK_OUT_REG     P1OUT
#define DATA_IN_REG     P1IN
#define CLK_IN_REG     P1IN

void four_digit_init();                                     // init

void four_digit_set_brightness(enum brightness_t b);        // set before calling display
void four_digit_set_point(bool disp_point);                 // set before calling display
void four_digit_display(uint8_t BitAddr, uint8_t DispData);
void display_segment(uint8_t BitAddr, uint8_t segment);
void four_digit_clear();

#endif
