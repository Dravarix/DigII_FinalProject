/** @file
 * @addtogroup DigitalII CarProject
 *
 * Sets up and controls two DC Motors using Timer A1
 * for PWM and 4 GPIO pins.
 *
 * setDCMotorDirection() will set the motors to forward, reverse, or brake.
 *
 * setDCMotorSpeed() will set the speed of the motors.
 *
 * delay() is used for idling CPU and is for debugging purposes.
 *
 * @author James Ritchie III, Mario DeSantis, Jon Frederickson
 * @version 2015.04.22
 *
 * @{
 */
#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include <msp430.h>
#include <stdint.h>

#define FORWARD		2
#define REVERSE 	1
#define BRAKE  		0

#define MAXCOUNT 	32768
#define SCALE		MAXCOUNT/50

/**
 * Initializes the pins used for the DC Motors. This sets Pins 2.2 and 2.5 up
 * for PWM using Timer A1 on the MSP430. Also sets up pins 1.4 and 1.5 to
 * control the input pins of motor A, and 1.6 and 1.7 pins to control the input
 * pins of motor B.
 */
void DCMotorInit(void);

/**
 * Sets the direction of the motors. If a 2 is received, they are set to go
 * forward. If a 1 is received, they are set to go backwards. If a 0 is
 * received, they are in brake mode.
 *
 * @param dir Direction the motor is going in. 2 means forward, 1 means reverse,
 * and 0 means zero speed.
 */
void setDCMotorDirection(uint8_t dir);

/**
 * Sets the motor speed based on the input for both motors. This function will
 * enable the correct input pins for motor direction, while disabling the other
 * input pins. It also sends a PWM signal to the enable pins to control speed
 * of the motor.
 *
 * @param speed - int value from 0 to 127
 */
void setDCMotorSpeed(uint16_t speed);

/**
 * Creates a delay based on the inputted number to block the usage of the CPU.
 *
 * @param number Length of delay in miliseconds.
 */
void delay(uint32_t number);

/** @} */
#endif /* DCMOTOR_H_ */
