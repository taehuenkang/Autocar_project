
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "speaker.h"
#include "fnd.h"
#include "extern.h"
extern volatile int msec_count;
#define DO_01   1911
#define DO_01_H 1817
#define RE_01   1703
#define RE_01_H 1607
#define MI_01   1517
#define FA_01   1432
#define FA_01_H 1352
#define SO_01   1276
#define SO_01_H 1199
#define LA_01   1136
#define LA_01_H 1073
#define TI_01   1012
#define DO_02   956
#define DO_02_H 909
#define RE_02   851
#define RE_02_H 804
#define MI_02   758
#define FA_02   716
#define FA_02_H 676
#define SO_02   638
#define SO_02_H 602
#define LA_02   568
#define LA_02_H 536
#define TI_02   506
#define DO_03   478
#define DO_03_H 450
#define RE_03   425
#define RE_03_H 401
#define MI_03   378
#define FA_03   358
#define SO_03   319
#define LA_03   284
#define TI_03   253
#define DO_04   239
#define RE_04   212
#define MI_04   189
#define FA_04   179
#define SO_04   159

#define F_CLK       16000000L //클럭
#define F_SCALER   8 //프리스케일러
#define BEAT_1   150
#define BEAT_2   250
#define BEAT_3   325
#define BEAT_4   400


// Stage_Clear 멜로디 (긴 버전)
const int Stage_Clear_Tune[] = {
	FA_03,SO_03,LA_03,FA_03,SO_03,TI_03,
	DO_04,LA_03,SO_03,
	FA_03,SO_03,LA_03,FA_03,SO_03,TI_03,
	DO_04,SO_03,
	FA_03,SO_03,LA_03,FA_03,SO_03,TI_03,
	DO_04,LA_03,SO_03,
	SO_04,FA_04  ,MI_04,RE_04,MI_04,DO_04,'/0'
};

// Stage_Clear 비트 (각 음의 길이)
const int Stage_Clear_Beats[] = {
	BEAT_1, BEAT_1, BEAT_1, BEAT_1,BEAT_1, BEAT_1,
	BEAT_4, BEAT_1,BEAT_4,
	BEAT_1, BEAT_1, BEAT_1,BEAT_1, BEAT_1, BEAT_1,
	BEAT_4,BEAT_4,
	BEAT_1, BEAT_1, BEAT_1,
	BEAT_1, BEAT_1, BEAT_1,
	BEAT_4,BEAT_1, BEAT_4,
	BEAT_1, BEAT_3,BEAT_1, BEAT_3, BEAT_1, BEAT_1
};

const int Siren_Tune[] = {
	SO_04,DO_04,FA_04,MI_04,DO_04,0,
	SO_04,DO_04,FA_04,MI_04,RE_04,DO_04,
	RE_04,MI_04,MI_04,RE_04,DO_04,
	RE_04,MI_04,MI_04,RE_04,DO_04,
};

const int Siren_Beats[] = {
	BEAT_3,BEAT_1,BEAT_1,BEAT_1,BEAT_1,
	BEAT_3,BEAT_1,BEAT_1,BEAT_1,BEAT_1,BEAT_1,
	BEAT_1,BEAT_1,BEAT_1,BEAT_1,BEAT_1,
	BEAT_1,BEAT_1,BEAT_1,BEAT_1,BEAT_1,
};

void Music_Player (int *tone, int *Beats)
{
	msec_count=0;
	int i=0;
	while(*tone != '/0')
	{
		if(msec_count >= Beats[i++])
		{
			OCR1A = *tone;
			tone++;
			OCR3A = 0;
			msec_count=0;
		}
	}
	return;
}
void cuby_buzzer(void)
{	
	static uint8_t idx=0;
	
	if( siren_count == Stage_Clear_Beats[idx])
	{
		OCR3A = 0;
		idx = (idx + 1) % (sizeof(Stage_Clear_Tune)/sizeof(int));
		siren_count=0;
		//OCR3A = Stage_Clear_Tune[idx];
		OCR3A = 1500;
	}
	
}
// Timer3 위상교정 PWM
void init_speaker(void)
{
	DDRE |= 1 << 3;   // PWM CHANNEL  OC3A(PE3) 출력 모드로 설정 한다.
	TCCR3A = (1<<COM3A0); // COM3A0 : 비교일치시 PE3 출력 반전 (P328 표14-6 참고)
	TCCR3B = (1<<WGM32) | (1<<CS31);   // WGM32 : CTC 4(P327 표14-5) CS31: 8분주(P318)
	// CTC mode : 비교일치가 되면 카운터는 reset되면서 PWM 파형 출력 핀의 출력이 반전 됨.
	// 정상모드와 CTC모드의 차이점은 비교일치 발생시 TCNT1의 레지스터값을 0으로 설정 하는지 여부 이다.
	// 정상모드를 사용시 TCNT1이 자동으로 0으로 설정 되지 않아 인터럽트 루틴에서 TCNT1을 0으로 설정 해 주었다.
	// 위상교정 PWM mode4  CTC 분주비 8  16000000/8 ==> 2,000,000HZ(2000KHZ) :
	// up-dounting: 비교일치시 LOW, down-counting시 HIGH출력
	// 1/2000000 ==> 0.0000005sec (0.5us)
	// P599 TOP 값 계산 참고
	// PWM주파수 = OSC(16M) / ( 2(up.down)x N(분주율)x(1+TOP) )
	// TOP =  (fOSC(16M) /  2(up.down)x N(분주율)x 원하는주파수 )) -1
	//-----------------------------------------------------------
	// - BOTTOM :  카운터가 0x00/0x0000 일때를 가리킨다.
	// - MAX : 카운터가 0xFF/0xFFFF 일 때를 가리킨다.
	// - TOP?:  카운터가 가질 수 있는 최대값을 가리킨다. 오버플로우 인터럽트의 경우 TOP은 0xFF/0xFFFF
	//          이지만 비교일치 인터럽트의 경우 사용자가 설정한 값이 된다.
	
	TCCR3C = 0;  // P328 그림 14-11 참고
	OCR3A = 0;   // 비교 일치값을 OCR3A에 넣는다.
	
	return;
}

void delay_ms(int ms)
{
	while(ms-- != 0)_delay_ms(1);
}

void Auto_siren(void)
{
	if(siren_count <= 500)
	{
		OCR3A = 500;
	}
	else if (siren_count <= 1000){
		OCR3A = 700;
	}
	else if (siren_count >= 1000)
	{
		siren_count=0;
	}
}