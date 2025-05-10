
#include "fnd.h"




void init_fnd(void)
{
	FND_DATA_DDR = 0xff;  // 출력 모드로 설정
	// FND_DIGIT_DDR |= 0xf0;   // 자릿수 선택 7654
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3
	| 1 << FND_DIGIT_D4;
	// fnd를 all off
	FND_DATA_PORT = ~0x00;   // fnd를 all off
}


void fnd_display(int mode)
{
	
	uint8_t fnd_font[] = {~0x8e, ~0xc7, ~0x88, ~0x80 , ~0x92};
	FND_DIGIT_PORT = 0xff; //PORTC 초기화
	switch(mode)
	{
		case 0:
		FND_DIGIT_PORT = ~0x10;   // PORTF 4번
		FND_DATA_PORT = fnd_font[mode];
		break;
		case 1:
		FND_DIGIT_PORT = ~0x10;   // PORTF 4번
		FND_DATA_PORT = fnd_font[mode];
		break;
		case 2:
		FND_DIGIT_PORT = ~0x10;   // PORTF 4번
		FND_DATA_PORT = fnd_font[mode];
		break;
		case 3:
		FND_DIGIT_PORT = ~0x10;   // PORTF 4번
		FND_DATA_PORT = fnd_font[mode];
		break;
		case 4:
		FND_DIGIT_PORT = ~0x10;   // PORTF 4번
		FND_DATA_PORT = fnd_font[mode];
		break;
	}
}

