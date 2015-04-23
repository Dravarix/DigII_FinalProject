#include "system.h"

void DCMotorInit(void){
	//Set the configuration for Timer_A to Up Mode with SMCLK as source
	TA1CTL |= TASSEL_2 + MC_1;
	TA1CCTL1 |= OUTMOD_7;
	TA1CCTL2 |= OUTMOD_7;

	//Set up pins for input into DC Motor Driver.
	//This either need to be high or low to enable
	//current out in the corresponding output pin
	P1DIR |= PIN4 + PIN5 + PIN6 + PIN7;

	//Set up pins for Enable. Enable pins on
	//driver enable the set of pins to work.
	//Also set up so PWM can be outputted
	//through these pins.
	P2DIR |= PIN2 + PIN5;
	P2SEL |= PIN2 + PIN5;

	//Set TA1CCR0 so that our period is 20 ms
	TA1CCR0 = MAXCOUNT;

	//Set TA1CCR1 to have an initial duty cycle
	TA1CCR1 = 0;
	//Set TA1CCR1 to have an initial duty cycle
	TA1CCR2 = 0;
}

void setDCMotorDirection(uint8_t dir){
	switch(dir){
		case FORWARD:
			//Set reverse pins low
			P1OUT &= (~PIN5 + ~PIN6);
			//Set forwards pins high
			P1OUT |= PIN4 + PIN7;
			break;
		case BRAKE:
			//Set reverse pins low
			P1OUT &= (~PIN5 + ~PIN6);
			//Set forwards pins high
			P1OUT |= PIN4 + PIN7;
			break;
		case REVERSE:
			//Set forward pins low
			P1OUT &= (~PIN4 + ~PIN7);
			//Set backwards pins high
			P1OUT |= PIN5 + PIN6;
			break;
	}
}

void setDCMotorSpeed(uint16_t speed){
	//Both motors going forwards
	if(speed > 0){
		//Set duty cycle for the enable pins.
		//This controls how fast the motors go.
		//Boundary condition check
		if(speed > MAXCOUNT){
			TA1CCR1 = MAXCOUNT;
			TA1CCR2 = MAXCOUNT;
		}
		else{
			TA1CCR1 = speed;
			TA1CCR2 = speed;
		}
	}
	else{ //Stop motors
		//Set a duty cycle of 0 for enable pins
		TA1CCR1 = 0;
		TA1CCR2 = 0;
	}

}

void delay(uint32_t number){
	while(number--){
		//for(i = 0; i < 160000; i++);
	}
}


