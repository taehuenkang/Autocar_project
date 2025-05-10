/*
 * uart0.h
 *
 * Created: 2025-03-10 오전 10:33:04
 *  Author: microsoft
 */ 


#ifndef UART0_H_
#define UART0_H_

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>
#include <stdio.h>		// printf
#include <string.h>
#include "def.h"
extern volatile uint8_t uart0_data;

#endif /* UART0_H_ */