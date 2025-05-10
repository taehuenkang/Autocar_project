
#include "button.h"
#include "extern.h"
#include "def.h"
#include "led.h"


//auto mode check count
volatile int button_state = 0;
                                        
void init_button(void)
{
	BUTTON_DDR |= ( 1 << BUTTON0PIN );
}


int get_button(int button_num, int button_pin)
{
	static unsigned char button_status[BUTTON_NUMBER] =
	{BUTTON_RELEASE};	
	// 	지역 변수에 static을 쓰면 전역 변수처럼 함수를 빠져 나갔다 다시 들어 와도 값을 유지 한다.  
	int current_state;
	
	current_state = BUTTON_PIN & (1 << button_pin);   // 버튼을 읽는다. 
	if (current_state && button_status[button_num] == BUTTON_RELEASE)  // 버튼이 처음 눌려진 noise high 
	{
		_delay_ms(60);   // noise가 지나가기를 기다린다. 
		button_status[button_num] = BUTTON_PRESS;   // noise가 지나간 상태의 High 상태 
		return 0;   // 아직은 완전히 눌렸다 떼어진 상태가 아니다. 
	}
	else if (current_state==BUTTON_RELEASE && button_status[button_num] == BUTTON_PRESS)
	{
		_delay_ms(60);
		button_status[button_num] = BUTTON_RELEASE;   // 다음 버튼 체크를 위해서 초기화
		return 1;   // 완전히 1번 눌렸다 떼어진 상태로 인정		
	}
	
	return 0;   // 버튼이 open상태 
}

void auto_mode_check(void)	// button check
{
	if(get_button(BUTTON0, BUTTON0PIN))
		button_state = !button_state;
		
	if(button_state)
	{
		AUTO_RUN_LED_PORT |= 1 << AUTO_RUN_LED_PIN;	   // LED ON
	}
	else
	{
		AUTO_RUN_LED_PORT &= ~(1 << AUTO_RUN_LED_PIN); // LED OFF
		stop();
	}
		
	func_index = AUTO_MODE;
}