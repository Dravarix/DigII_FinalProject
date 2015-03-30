#include "DCMotor.h"
#include <msp430.h>
#include "msp430_launchpad.h"

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
	TA1CCR0 = 22000;

	//Set TA1CCR1 to have an initial duty cycle
	TA1CCR1 = 0;
	//Set TA1CCR1 to have an initial duty cycle
	TA1CCR2 = 0;
}

void setDCMotorSpeed(int8_t motorASpeed, int8_t motorBSpeed){
	//Both motors going forwards
	if(motorASpeed > 0 && motorBSpeed > 0){
		//Set reverse pins low
		P1OUT &= (~PIN5 + ~PIN6);
		//Set forwards pins high
		P1OUT |= PIN4 + PIN7;

		//Set duty cycle for the enable pins.
		//This controls how fast the motors go.
		//Boundary condition check
		if(motorASpeed > maxSpeed){
			TA1CCR1 = 173*maxSpeed;
		}
		else{
			TA1CCR1 = 173*motorASpeed;
		}
		//Boundary condition check
		if(motorBSpeed > maxSpeed){
			TA1CCR2 = 173*maxSpeed;
		}
		else{
			TA1CCR2 = 173*motorBSpeed;
		}
	}
	//Both motors going backwards
	else if(motorASpeed < 0 && motorBSpeed < 0){
		//Set forward pins low
		P1OUT &= (~PIN4 + ~PIN7);
		//Set backwards pins high
		P1OUT |= PIN5 + PIN6;

		//Set duty cycle for the enable pins.
		//Boundary condition check
		if(motorASpeed < -maxSpeed){
			TA1CCR1 = -173*maxSpeed;
		}
		else{
			TA1CCR1 = -173*motorASpeed;
		}
		//Boundary condition check
		if(motorBSpeed < -maxSpeed){
			TA1CCR2 = -173*maxSpeed;
		}
		else{
			TA1CCR2 = -173*motorBSpeed;
		}

	}
	//If motorA is going backwards and motorB is
	//going forwards.
	else if(motorASpeed < 0 && motorBSpeed > 0){
		//Turn off forward pin for motor A
		//Turn off backward pin for motor B
		P1OUT &= (~PIN4 + ~PIN6);
		//Turn on forward pin for motor B
		//Turn on backward pin for motor A
		P1OUT |= PIN5 + PIN7;

		//Set duty cycle for the enable pins.
		//Boundary condition check
		if(motorASpeed < -maxSpeed){
			TA1CCR1 = -173*maxSpeed;
		}
		else{
			TA1CCR1 = -173*motorASpeed;
		}
		//Boundary condition check
		if(motorBSpeed > maxSpeed){
			TA1CCR2 = 173*maxSpeed;
		}
		else{
			TA1CCR2 = 173*motorBSpeed;
		}
	}
	//If motorA is going forward and motorB is
	//going backwards
	else if(motorASpeed > 0 && motorBSpeed < 0){
		//Turn off forward pin for motor A
		//Turn off backward pin for motor B
		P1OUT &= (~PIN5 + ~PIN7);
		//Turn on forward pin for motor B
		//Turn on backward pin for motor A
		P1OUT |= PIN4 + PIN6;

		//Set duty cycle for the enable pins.
		//This controls how fast the motors go.
		//Boundary condition check
		if(motorASpeed > maxSpeed){
			TA1CCR1 = 173*maxSpeed;
		}
		else{
			TA1CCR1 = 173*motorASpeed;
		}
		//Boundary condition check
		if(motorBSpeed < -maxSpeed){
			TA1CCR2 = -173*maxSpeed;
		}
		else{
			TA1CCR2 = -173*motorBSpeed;
		}
	}
	else{ //Stop motors
		//Set all pins low
		P1OUT &= (~PIN4 + ~PIN5 + ~PIN6 + ~PIN7);

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


