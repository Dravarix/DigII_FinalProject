/*
 * Sets up and controls a servo motor using pin 2.6 and Timer A0.
 *
 * Author: James Ritchie III, Mario DeSantis, Jon Frederickson
 * Date Submitted: December 16, 2014
 * Latest Version: 20141216
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>

#define DUTY_LOWER_BOUND 1100
#define DUTY_UPPER_BOUND 2200

/**
 * Initializes servo, setting up pin 2.6 to be used for PWM output
 * to the servo. Also sets the default start state for the servo.
 */
void servoInit(void);

/*
 * Sets the direction of the servo.
 *
 * @param regVal
 */
void servoSetDuty(uint16_t regVal);


#endif /* SERVO_H_ */
