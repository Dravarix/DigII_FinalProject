/** @file
 * @addtogroup DigitalII CarProject
 *
 * Sets up the parameters for the specific system. It also contains all of the
 * includes needed.
 *
 * @author James Ritchie, Jon Frederickson, Mario DeSantis
 * @version 2015.04.22
 *
 * @{
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#define PIN0 0x01
#define PIN1 0x02
#define PIN2 0x04
#define PIN3 0x08
#define PIN4 0x10
#define PIN5 0x20
#define PIN6 0x40
#define PIN7 0x80

#include <msp430.h>
#include <ctype.h>
#include "DCMotor.h"
#include "uart.h"
#include "servo.h"
#include <stdint.h>
#include <stdbool.h>

/** @} */
#endif /* SYSTEM_H_ */
