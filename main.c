/*
 * Name: Nathan Culler
 * Date: 6/30/2023
 * Assignment: Lab 3
 * YouTube: https://youtu.be/c7TLoglDB-A
 *
 * This Program:
 * When this code is compiled it will create a "train" that goes around the SSD
 */


#include <msp430.h>
#include <Four_Digit_Display.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {IDLE, FORWARD, BACKWARD} state_t;

bool isS1Pressed = false;
bool isS2Pressed = false;

void Init();
void SetRedLED(bool state);
void SetGreenLED(bool state);
void delay_s(uint16_t seconds);
void PositionOne();
void PositionTwo();
void PositionThree();
void PositionFour();
void PositionFive();
void PositionSix();
void PositionSeven();
void PositionEight();
void PositionNine();
void PositionTen();
void PositionEleven();
void PositionTwelve();
void DrawTrain(volatile unsigned int* pos);
state_t RunIdleState(volatile uint32_t* i, volatile unsigned int* currentPosition,state_t previousState, bool* changeDirection);
state_t RunForwardState(volatile uint32_t* i, volatile unsigned int* currentPosition, state_t currentState, bool* changeDirection);
state_t RunBackwardState(volatile uint32_t* i, volatile unsigned int* currentPosition, state_t currentState, bool* changeDirection);

int main(void)
{
    Init();
	four_digit_init();
	four_digit_clear();

	volatile uint32_t x = 0;
	volatile uint32_t* i = &x;
	volatile unsigned int cP = 1;
	volatile unsigned int* currentPosition = &cP;

	state_t currentState = IDLE;
	state_t previousState = FORWARD;
	state_t nextState = currentState;
	bool cD = false;
	bool* changeDirection = &cD;

	while (1){
	    switch(currentState){
	    case IDLE:
	        nextState = RunIdleState(i, currentPosition, previousState, changeDirection);
	        break;
	    case FORWARD:
	        nextState = RunForwardState(i, currentPosition, currentState, changeDirection);
	        previousState = currentState;
	        break;
	    case BACKWARD:
	        nextState = RunBackwardState(i, currentPosition, currentState, changeDirection);
	        previousState = currentState;
	        break;
	    default:
	        four_digit_clear();
	        SetGreenLED(true);
	        SetRedLED(true);
	        break;
	    }
	    currentState = nextState;
	}
	return 0;
}

void Init(){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P2DIR &= ~BIT3;
    P2REN |= BIT3;
    P2OUT |= BIT3;
    P2IE |= BIT3; //enables interrupt
    P2IES |= BIT3; //high-to-low trigger condition
    P2IFG &= ~BIT3; //clears flag
    P4DIR &= ~BIT1;
    P4REN |= BIT1;
    P4OUT |= BIT1;
    P4IE |= BIT1; //enables interrupt
    P4IES |= BIT1; //high-to-low trigger condition
    P4IFG &= ~BIT1; //clears flag
    P6DIR |= BIT6;
    P6OUT &= ~BIT6;

    __enable_interrupt(); //Compiler specific function to enable interrupt
}

#pragma vector=PORT2_VECTOR
__interrupt void port2ISR(void){
    isS2Pressed = true;

    P2IFG &= ~BIT3;
}


#pragma vector=PORT4_VECTOR
__interrupt void port4ISR(void){
    isS1Pressed = true;

    P4IFG &= ~BIT1;
}

state_t RunIdleState(volatile uint32_t* i, volatile unsigned int* currentPosition, state_t previousState, bool* changeDirection){

    state_t nextState;

    SetGreenLED(false);
    SetRedLED(true);

    if(*i == 0){
        DrawTrain(currentPosition);
    }
    *i = 1; //So the state doesn't keep redrawing the Idle Train

    if(isS2Pressed){
        //__delay_cycles(10000);
        isS2Pressed = false;
        *changeDirection = true;
    }

    if(isS1Pressed && *changeDirection == true && previousState == FORWARD){
        //__delay_cycles(10000);
        isS1Pressed = false;
        nextState = BACKWARD;
    }
    else if(isS1Pressed && *changeDirection == true && previousState == BACKWARD){
        //__delay_cycles(10000);
        isS1Pressed = false;
        nextState = FORWARD;
    }
    else if(isS1Pressed){
        //__delay_cycles(10000);
        isS1Pressed = false;
        nextState = previousState;
    }
    else {
        nextState = IDLE;
    }
    return nextState;
}

state_t RunForwardState(volatile uint32_t* i, volatile unsigned int* currentPosition, state_t currentState, bool* changeDirection){

    state_t nextState;

    SetGreenLED(true);
    SetRedLED(false);

    if(*i == 0){
        DrawTrain(currentPosition);
    }

    if(isS1Pressed){
        isS1Pressed = false;
        nextState = IDLE;
        *i = 1;
    }
    else if(isS2Pressed){
        *i = 1;
        isS2Pressed = false;
        nextState = BACKWARD;
    }
    else {
        ++*currentPosition;
        if(*currentPosition > 12){
            *currentPosition = 1;
        }
        *i = 0;
        nextState = FORWARD;
        delay_s(1);
    }

    return nextState;
}

state_t RunBackwardState(volatile uint32_t* i, volatile unsigned int* currentPosition, state_t currentState, bool* changeDirection){

    state_t nextState;

    SetGreenLED(true);
    SetRedLED(false);

    if(*i == 0){
        DrawTrain(currentPosition);
    }

    if(isS1Pressed){
        isS1Pressed = false;
        nextState = IDLE;
        *i = 1;

    }
    else if(isS2Pressed){
        *i = 1;
        isS2Pressed = false;
        nextState = FORWARD;
    }
    else {
        --*currentPosition;
        if(*currentPosition < 1){
            *currentPosition = 12;
        }
        *i = 0;
        nextState = BACKWARD;
        delay_s(1);
    }
    return nextState;
}

void delay_s(uint16_t seconds){
    volatile uint16_t i;
    for(i = 0; i < seconds; i++){
        __delay_cycles(1000000);
    }
}

void SetRedLED(bool state){
    if(state){
        P1OUT |= BIT0;
    }
    else {
        P1OUT &= ~BIT0;
    }
}

void SetGreenLED(bool state){
    if(state){
        P6OUT |= BIT6;
    }
    else {
        P6OUT &= ~BIT6;
    }
}

void DrawTrain(volatile unsigned int* pos){
    four_digit_clear();

    switch(*pos){
    case 1:
        display_segment(POS_1, SEG_A | SEG_E | SEG_F);
        break;
    case 2:
        display_segment(POS_2, SEG_A);
        display_segment(POS_1, SEG_A | SEG_F);
        break;
    case 3:
        display_segment(POS_3, SEG_A);
        display_segment(POS_2, SEG_A);
        display_segment(POS_1, SEG_A);
        break;
    case 4:
        display_segment(POS_4, SEG_A);
        display_segment(POS_3, SEG_A);
        display_segment(POS_2, SEG_A);
        break;
    case 5:
        display_segment(POS_4, SEG_A | SEG_B);
        display_segment(POS_3, SEG_A);
        break;
    case 6:
        display_segment(POS_4, SEG_A | SEG_B | SEG_C);
        break;
    case 7:
        display_segment(POS_4, SEG_B | SEG_C | SEG_D);
        break;
    case 8:
        display_segment(POS_4, SEG_C | SEG_D);
        display_segment(POS_3, SEG_D);
        break;
    case 9:
        display_segment(POS_2, SEG_D);
        display_segment(POS_3, SEG_D);
        display_segment(POS_4, SEG_D);
        break;
    case 10:
        display_segment(POS_1, SEG_D);
        display_segment(POS_2, SEG_D);
        display_segment(POS_3, SEG_D);
        break;
    case 11:
        display_segment(POS_1, SEG_E | SEG_D );
        display_segment(POS_2, SEG_D);
        break;
    case 12:
        display_segment(POS_1, SEG_F | SEG_E | SEG_D );
        break;
    default:
        display_segment(POS_1, SEG_A | SEG_B | SEG_C |SEG_D | SEG_E | SEG_F);
        display_segment(POS_2, SEG_A | SEG_B | SEG_C |SEG_D | SEG_E | SEG_F);
        display_segment(POS_3, SEG_A | SEG_B | SEG_C |SEG_D | SEG_E | SEG_F);
        display_segment(POS_4, SEG_A | SEG_B | SEG_C |SEG_D | SEG_E | SEG_F);
        break;
    }
}
