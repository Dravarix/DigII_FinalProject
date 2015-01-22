#include <msp430.h>
#include "servo.h"
#include "DCMotor.h"
#include "time.h"

/*
 * main.c
 *
 * Author: James Ritchie III, Mario DeSantis, Jon Frederickson
 * Date Submitted: December 16, 2014
 * Latest Version: 20141216
 *
 *
 */
int main(void) {
     WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    //Run initialization
    servoInit();
    DCMotorInit();

    while(1){
    	setDCMotorSpeed(127,127);
    	delay(100000);
		//Move to upper bound
		servoSetDuty(DUTY_LOWER_BOUND);
		//Add delay
		delay(100000);
		//Move to upper bound
		servoSetDuty(DUTY_UPPER_BOUND);
		//delay
		delay(100000);
    	//setDCMotorSpeed(-50,-50);
    	//delay(100000);
    	//setDCMotorSpeed(0,0,0,0);
    	//delay(1);
    }
	
}
