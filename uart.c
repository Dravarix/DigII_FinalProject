/* UART driver from http://www.argenox.com/library/msp430/ch8-msp430_uart.php */
#include "uart.h"

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

volatile char rx_data = 0;
volatile bool rx_wasRead = false;

void uart_init(void) {
	DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
	P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2;
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 8;                              // 1MHz 115200
	UCA0BR1 = 0;                              // 1MHz 115200
	UCA0MCTL = UCBRS2 + UCBRS0;               // Modulation UCBRSx = 5
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

void uart_send(char data) {
	//uint8_t i;
	//for (i = 0; i < length; i++) {
		while (!(IFG2 & UCA0TXIFG));
		UCA0TXBUF = data;
	//}
}

char uart_read(void) {
	rx_wasRead = true;
	return rx_data;
}

bool uart_wasRead(void){
	return rx_wasRead;
}
