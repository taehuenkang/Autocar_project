#include "ultrasonic.h"

volatile char scm[50];

// 초음파 거리 전역 변수 버퍼 제거
// (ultrasonic_left_buffer, center_buffer 등 필요 없음)

ISR(INT4_vect) // 왼쪽 초음파
{
	if (ECHO_PIN_LEFT & (1 << ECHO_LEFT)) 
	{
		TCNT2 = 0;  // 타이머 초기화
	} 
	else 
	{
		ultrasonic_left_distance = 1000000.0 * TCNT2 * 128 / F_CPU;
	}
	
	if (ultrasonic_left_distance < SIDE_WARNING_RANGE)
	{
		warning_left = 1;
	}
	else 
	{
		warning_left = 0;
	}
}

ISR(INT5_vect) // 중앙 초음파
{
	if (ECHO_PIN_CENTER & (1 << ECHO_CENTER)) 
	{
		TCNT2 = 0;  // 타이머 초기화
	} 
	else 
	{
		ultrasonic_center_distance = 1000000.0 * TCNT2 * 128 / F_CPU;
	}
	
	if (ultrasonic_center_distance < CENTER_WARNING_RANGE)
	{
		warning_center = 1;
	}
	else
	{
		warning_center = 0;
	}
}

ISR(INT6_vect) // 오른쪽 초음파
{
	if (ECHO_PIN_RIGHT & (1 << ECHO_RIGHT)) 
	{
		TCNT2 = 0;  // 타이머 초기화
	} 
	else
	{
		ultrasonic_right_distance = 1000000.0 * TCNT2 * 128 / F_CPU;
	}
	
	if (ultrasonic_right_distance < SIDE_WARNING_RANGE) 
	{
		warning_right = 1;
	} 
	else 
	{
		warning_right = 0;
	}
}

void init_ultrasonic(void)
{
	// 8비트 타이머2 설정: 128 분주
	TCCR2 |= 1 << CS20 | 0 << CS21  | 1 << CS22;

	// LEFT 센서
	TRIG_DDR_LEFT |= (1 << TRIG_LEFT);
	ECHO_DDR_LEFT &= ~(1 << ECHO_LEFT);
	EICRB |= (0 << ISC41) | (1 << ISC40);
	EIMSK |= (1 << INT4);

	// CENTER 센서
	TRIG_DDR_CENTER |= (1 << TRIG_CENTER);
	ECHO_DDR_CENTER &= ~(1 << ECHO_CENTER);
	EICRB |= (0 << ISC51) | (1 << ISC50);
	EIMSK |= (1 << INT5);

	// RIGHT 센서
	TRIG_DDR_RIGHT |= (1 << TRIG_RIGHT);
	ECHO_DDR_RIGHT &= ~(1 << ECHO_RIGHT);
	EICRB |= (0 << ISC61) | (1 << ISC60);
	EIMSK |= (1 << INT6);
}

void trigger_ultrasonic(void)
{
	// LEFT
	TRIG_PORT_LEFT &= ~(1 << TRIG_LEFT);
	_delay_us(10);
	TRIG_PORT_LEFT |= (1 << TRIG_LEFT);
	_delay_us(15);
	TRIG_PORT_LEFT &= ~(1 << TRIG_LEFT);

	// CENTER
	TRIG_PORT_CENTER &= ~(1 << TRIG_CENTER);
	_delay_us(10);
	TRIG_PORT_CENTER |= (1 << TRIG_CENTER);
	_delay_us(15);
	TRIG_PORT_CENTER &= ~(1 << TRIG_CENTER);

	// RIGHT
	TRIG_PORT_RIGHT &= ~(1 << TRIG_RIGHT);
	_delay_us(10);
	TRIG_PORT_RIGHT |= (1 << TRIG_RIGHT);
	_delay_us(15);
	TRIG_PORT_RIGHT &= ~(1 << TRIG_RIGHT);
}

void ultrasonic_distance_check(void)
{
	trigger_ultrasonic();
	 printf("L:%5dcm\n", ultrasonic_left_distance);
	 printf("C:%5dcm\n", ultrasonic_center_distance);
	 printf("R:%5dcm\n", ultrasonic_right_distance);
}
