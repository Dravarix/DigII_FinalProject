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

	uint8_t lastSpeed;
	lastSpeed = speed;

	while(1){
		while (lastSpeed == speed); // wait for speed modification
			if (speed > 50) {
				setDCMotorSpeed(SCALE * (speed - 50));
				setDCMotorDirection(FORWARD);
				//uart_send(speed);
			} else if (speed < 50) {
				setDCMotorSpeed(SCALE * (50 - speed));
				setDCMotorDirection(REVERSE);
				//uart_send(speed);
			} else {
				setDCMotorSpeed(0);
				setDCMotorDirection(BRAKE);
				//uart_send(speed);
			}
		lastSpeed = speed; // will be modified in interrupt
	}
	
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {        // Vector 2 - RXIFG
	while (!(IFG2 & UCA0TXIFG));             // USCI_A0 TX buffer ready?
	char data = UCA0RXBUF;                  // TX -> RXed character
	if (data != '\n'){
		switch(data){
				case 'l':
					servoSetDuty(DUTY_LOWER_BOUND);
					break;
				case 'r':
					servoSetDuty(DUTY_UPPER_BOUND);
					break;
				case 's':
					servoSetDuty(DUTY_MID_BOUND);
					break;
				default:
					speed = data;
					break;
			}
	}
	UCA0TXBUF = data;
}
