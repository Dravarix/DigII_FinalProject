/**
 * Sets up and controls two DC Motors using Timer A1
 * for PWM and 4 GPIO pins.
 *
 * Author: James Ritchie III, Mario DeSantis, Jon Frederickson
 * Date Submitted: December 16, 2014
 * Latest Version: 20141216
 */
#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include <stdint.h>

#define maxSpeed 127

/**
 * Initializes the pins used for the DC Motors.
 * This sets Pins 2.2 and 2.5 up for PWM using
 * Timer A1 on the MSP430. Also sets up pins
 * 1.4 and 1.5 to control the input pins of
 * motor A, and 1.6 and 1.7 pins to control the
 * input pins of motor B.
 */
void DCMotorInit(void);

/**
 * Sets the motor speed and direction based on the input
 * for both motors. A negative value indicates a backwards
 * direction. This function will enable the correct input
 * pins for motor direction, while disabling the other
 * input pins. It also sends a PWM signal to the enable
 * pins to control speed of the motor.
 *
 * @param motorASpeed - int value from -128 to 127
 * @param motorBSpeed = int value from -128 to 127
 */
void setDCMotorSpeed(int8_t motorASpeed, int8_t motorBSpeed);

/**
 * Creates a delay based on the inputted number to block
 * the usage of the CPU.
 */
void delay(uint32_t number);

#endif /* DCMOTOR_H_ */
