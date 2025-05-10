#ifndef SPEAKER_H_
#define SPEAKER_H_

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>  // PORTA PORTB등의 I/O 관련 register등이 있다.
#include <util/delay.h>  // _delay_ms _delay_us

void Music_Player(int *tone, int *Beats);
void init_speaker(void);
void cuby_buzzer(void);



#endif /* SPEAKER_H_ */