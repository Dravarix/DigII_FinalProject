#include "system.h"
/*
 * main.c
 *
 * Author: James Ritchie III, Mario DeSantis, Jon Frederickson
 * Date Submitted: December 16, 2014
 * Latest Version: 20141216
 *
 */

volatile uint8_t speed = 0;

int main(void) {
	 WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	//Run initialization
	servoInit();
	DCMotorInit();
	uart_init();
	_enable_interrupt();

	//Last set speed
	uint8_t lastSpeed;
	//set speed to last speed to initialize.
	lastSpeed = speed;

	while(1){
		while (lastSpeed == speed); // Wait for speed modification
			if (speed > 50) { //If slider in app was over 50%, go forward
				setDCMotorSpeed(SCALE * (speed - 50));
				setDCMotorDirection(FORWARD);
			} else if (speed < 50) { //If slider in app was belowe 50%, go backward
				setDCMotorSpeed(SCALE * (50 - speed));
				setDCMotorDirection(REVERSE);
			} else {
				setDCMotorSpeed(0);
				setDCMotorDirection(BRAKE);
			}
		lastSpeed = speed; // Will be modified in interrupt
	}
	
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {        // Vector 2 - RXIFG
	while (!(IFG2 & UCA0TXIFG));             // USCI_A0 TX buffer ready?
	char data = UCA0RXBUF;                  // TX -> RXed character
	if (data != '\n'){
		switch(data){ //Check to see what data was sent.
				case 'l': //If an l is received, set servo to turn left.
					servoSetDuty(DUTY_LOWER_BOUND);
					break;
				case 'r': //If an r is received, set servo to turn left.
					servoSetDuty(DUTY_UPPER_BOUND);
					break;
				case 's': //If an s is received, set servo to go straight.
					servoSetDuty(DUTY_MID_BOUND);
					break;
				default: //If none of the values above are received, set speed.
					speed = data;
					break;
			}
	}
	UCA0TXBUF = data; //Seed feedback to the controller.
}
