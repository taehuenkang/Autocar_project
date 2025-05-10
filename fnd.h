/*
 * fnd.h
 *
 * Created: 2025-03-06 오후 12:23:18
 *  Author: microsoft
 */ 


#ifndef FND_H_
#define FND_H_
#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us

#define FND_DATA_PORT  PORTC
#define FND_DATA_DDR   DDRC

#define FND_DIGIT_PORT PORTF
#define FND_DIGIT_DDR  DDRF
#define FND_DIGIT_D1  4
#define FND_DIGIT_D2  5
#define FND_DIGIT_D3  6
#define FND_DIGIT_D4  7

void init_fnd(void);
void fnd_display(int mode);

#endif /* FND_H_ */