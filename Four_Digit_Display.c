/*
    Four_Digital_Display.cpp
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


#include "Four_Digit_Display.h"

#define HAL_PIN_HIGH 1
#define HAL_PIN_LOW 0
#define HAL_PIN_INPUT 0
#define HAL_PIN_OUTPUT 1

volatile uint8_t __pinDta = 4;
volatile uint8_t __pinClk = 1;

volatile uint8_t Cmd_DispCtrl  = 0;
volatile uint8_t _PointFlag    = 0;     //_PointFlag=1:the clock point on

uint8_t pin_read(volatile uint8_t pin);
void pin_write(volatile uint8_t pin, volatile uint8_t val);
void pin_dir(volatile uint8_t pin, volatile uint8_t val);
uint8_t codingSegment(uint8_t DispData);

uint8_t pin_read(volatile uint8_t pin){
    if(pin == __pinClk){
        if(CLK_IN_REG & (1 << pin)){
            return HAL_PIN_HIGH;
        }
        return HAL_PIN_LOW;
    } else {
        if(DATA_IN_REG & (1 << pin)){
            return HAL_PIN_HIGH;
        }
        return HAL_PIN_LOW;
    }
}

void pin_write(volatile uint8_t pin, volatile uint8_t val){
    if(pin == __pinClk){
        if(val){
            CLK_OUT_REG |= 1 << pin;
        } else {
            CLK_OUT_REG &= ~(1 << pin);
        }
    } else {
        if(val){
            DATA_OUT_REG |= 1 << pin;
        } else {
            DATA_OUT_REG &= ~(1 << pin);
        }
    }
}

void pin_dir(volatile uint8_t pin, volatile uint8_t val){
    if(pin == __pinClk){
        if(val){
            CLK_DIR_REG |= 1 << pin;
        } else {
            CLK_DIR_REG &= ~(1 << pin);
        }
    } else {
        if(val){
            DATA_DIR_REG |= 1 << pin;
        } else {
            DATA_DIR_REG &= ~(1 << pin);
        }
    }
}

const uint8_t TubeTab[] = {
    0x3f, 0x06, 0x5b, 0x4f,
    0x66, 0x6d, 0x7d, 0x07,
    0x7f, 0x6f, 0x77, 0x7c,
    0x39, 0x5e, 0x79, 0x71
}; //0~9,A,b,C,d,E,F

// write a byte
void writeByte(volatile uint8_t wr_data) {
    volatile uint8_t i, count1;
    for (i = 0; i < 8; i++) {                         //sent 8bit data
        pin_write(__pinClk, HAL_PIN_LOW);
        if (wr_data & 0x01) {
            pin_write(__pinDta, HAL_PIN_HIGH);    //LSB first
        } else {
            pin_write(__pinDta, HAL_PIN_LOW);
        }
        wr_data >>= 1;
        pin_write(__pinClk, HAL_PIN_HIGH);
    }
    pin_write(__pinClk, HAL_PIN_LOW);                //wait for the ACK
    pin_write(__pinDta, HAL_PIN_HIGH);
    pin_write(__pinClk, HAL_PIN_HIGH);

    pin_dir(__pinDta, HAL_PIN_INPUT);
    while (pin_read(__pinDta)) {
        count1 += 1;
        if (count1 == 200) { //
            pin_dir(__pinDta, HAL_PIN_OUTPUT);
            pin_write(__pinDta, HAL_PIN_LOW);
            count1 = 0;
        }
        pin_dir(__pinDta, HAL_PIN_INPUT);
    }
    pin_dir(__pinDta, HAL_PIN_OUTPUT);
}

// send start signal to TM1637
void start(void) {
    pin_write(__pinClk, HAL_PIN_HIGH);
    pin_write(__pinDta, HAL_PIN_HIGH);
    pin_write(__pinDta, HAL_PIN_LOW);
    pin_write(__pinClk, HAL_PIN_LOW);
}

// End of transmission
void stop(void) {
    pin_write(__pinClk, HAL_PIN_LOW);
    pin_write(__pinDta, HAL_PIN_LOW);
    pin_write(__pinClk, HAL_PIN_HIGH);
    pin_write(__pinDta, HAL_PIN_HIGH);
}

//
void codingS(uint8_t* DispData) {
    uint8_t PointData;
    uint8_t i;
    if (_PointFlag == POINT_ON) {
        PointData = 0x80;
    } else {
        PointData = 0;
    }
    for (i = 0; i < 4; i ++) {
        if (DispData[i] == 0x7f) {
            DispData[i] = 0x00;
        } else {
            DispData[i] = TubeTab[DispData[i]] + PointData;
        }
    }
}

//
uint8_t coding(uint8_t DispData) {
    uint8_t PointData;
    if (_PointFlag == POINT_ON) {
        PointData = 0x80;
    } else {
        PointData = 0;
    }
    if (DispData == 0x7f) {
        DispData = 0x00 + PointData;    //The bit digital tube off
    } else {
        DispData = TubeTab[DispData] + PointData;
    }
    return DispData;
}

// initialize
void four_digit_init() {
    __pinDta = DATA_PIN;
    __pinClk = CLK_PIN;

    pin_dir(__pinDta, HAL_PIN_OUTPUT);
    pin_dir(__pinClk, HAL_PIN_OUTPUT);

    four_digit_set_brightness(BRIGHTEST);

    four_digit_clear();
}

// set brightness
void four_digit_set_brightness(enum brightness_t b) {         //set before calling display
    Cmd_DispCtrl = 0x88 + b;
}

// show :
void four_digit_set_point(bool disp_point) {            //set before calling display
    _PointFlag = disp_point;
}

// display a bit
void four_digit_display(uint8_t BitAddr, uint8_t DispData) {
    uint8_t SegData;
    SegData = coding(DispData);
    start();          //start signal sent to TM1637 from MCU
    writeByte(ADDR_FIXED);//
    stop();           //
    start();          //
    writeByte(BitAddr | 0xc0); //
    writeByte(SegData);//
    stop();            //
    start();          //
    writeByte(Cmd_DispCtrl);//
    stop();           //
}

// display a segment
void display_segment(uint8_t BitAddr, uint8_t Seg) {
    uint8_t SegData;
    SegData = Seg;
    start();          //start signal sent to TM1637 from MCU
    writeByte(ADDR_FIXED);//
    stop();           //
    start();          //
    writeByte(BitAddr | 0xc0); //
    writeByte(SegData);//
    stop();            //
    start();          //
    writeByte(Cmd_DispCtrl);//
    stop();           //
}

// clear display
void four_digit_clear() {
    four_digit_display(0x00, 0x7f);
    four_digit_display(0x01, 0x7f);
    four_digit_display(0x02, 0x7f);
    four_digit_display(0x03, 0x7f);
}
