
#include "button.h"
#include "uart0.h"
#include "extern.h"
#include "def.h"
#include "ultrasonic.h"
#define OK 1
#define ERROR 0
#define CRITICAL_DISTANCE 40

// 선언 ----------------------------------------------------
void init_timer0(void);
void distance_check(void);
void manual_mode(void);
void auto_mode(void);
void init_autocar(void);

//main.c
volatile int msec_count = 0;
//ultrasonic.c
volatile int ultra_check=0;
//fnd.c
volatile int auto_count = 0; //fnd
//buzzer.c
volatile int siren_count=0;
//lcd.c
volatile int lcd_count=0;


//func_index
volatile uint8_t manual_state;


//위험 거리 판단
volatile int warning_left=1;
volatile int warning_right=1;
volatile int warning_center=1;

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int func_index = MANUAL_MODE;
void (*pfunc[])() =
{
	manual_mode,		// 수동모드
	distance_check,		// 초음파 거리 측정
	auto_mode_check,	// button check
	auto_mode			// 자율주행
};

// interrupt service routine은 반드시 main함수 이전에 정의한다.
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6; // 6 ~ 256으로 돌리기 위해
	msec_count++;
	ultra_check++;
	siren_count++;
	lcd_count++;
}



int main(void)
{
	init_autocar();
	stdout = &OUTPUT;   // 2. printf가 동작 되도록 stdout에 OUTPUT화일 포인터를 assign
	
	sei();// 전역적으로 인터럽트 허용
	printf("init\n");
	while (1)
	{			
		auto_mode_check();
		if (button_state)
		{
			auto_mode();
			if(lcd_count >= 1000)
			{
				I2C_LCD_Auto();
				lcd_count = 0;				
			}
		}
		else
		{
			stop();
			OCR3A = 0;
			manual_mode();
			//cuby_buzzer();
			if (bt_data != 0) {  // 데이터가 수신되었으면
				manual_state = bt_data;
				fnd_display(auto_count);
				if(lcd_count >= 1000)
				{
					I2C_LCD_Normal();
					lcd_count = 0;
				}				
				bt_data = 0;
			}
		}
		
		
	}

}



// AVR의 8bit counter timer0를 초기화한다.
// 임베디드/FPGA에서 가장 중요한건 초기화 -> init함수는 특히 신경쓰기!!
void init_timer0(void)
{
	// 분주 (divider / prescale)
	// 16MHz / 64 down!!
	// 16000000Hz / 64 = 250,000 Hz 로 만든다!
	// T(주기) = 1clock에 걸리는 시간 = 1 / 250000 = 0.000004sec = 0.004ms (4us)
	// 8bit timer overflow 발생 시 소요시간 = 0.004ms * 256 = 1.024ms = 0.001024sec
	//	-> interrupt를 정확하게 1ms마다 오게 하고싶으면, 
	//    사용자 지정으로 256이 아니라 250클록마다 인터럽트가 생기도록
	
	// TCMT0 초기값 설정
	TCNT0 = 6; // 정확히 1ms를 세기 위해 초기값을 0이 아니라 6으로 해준다. (250 펄스 이후에 인터럽트)
	
	// 분주비 설정 (64)
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;
	
	// timer overflow interrupt 허용 (TIMER0 OVF)
	TIMSK |= 1 << TOIE0;	// 지역 인터럽트 허용
}

void manual_mode(void)		// 수동 mode
{
	switch (manual_state)
	{
		case 'F' :
		case 'f' :
		//printf("press f\n");
		forward(1000);	 // 4us x 500 = 0.002sec (2ms)
		break;
		case 'B' :
		case 'b' :
		//printf("press b\n");
		backward(1000);	 // 4us x 500 = 0.002sec (2ms)
		break;
		case 'L' :
		case 'l' :
		//printf("press l\n");
		turn_left(1000);  // 4us x 700 = 0.0028sec (2.8ms)
		break;
		case 'R' :
		case 'r' :
		//printf("press r\n");
		turn_right(1000); // 4us x 700 = 0.0028sec (2.8ms)
		break;
		case 'S' :
		case 's' :
		//printf("press s\n");
		stop();
		break;
	}
}

void distance_check(void)	// 초음파 거리 측정
{
	func_index = MODE_CHECK;
}

void auto_mode(void)
{
	if (ultra_check >= 100) {
		trigger_ultrasonic();  // 초음파 센서 트리거
		ultra_check = 0;
	}
	Auto_siren();
	int normal_speed = 450;    // 일반 주행 속도
	int slow_speed = 400;      // 위험 감지 시 느린 속도
	int turn_speed = 1000;      // 회전 속도
	int back_speed = 700;      // 후진 속도

	int center = ultrasonic_center_distance;
	int left = ultrasonic_left_distance;
	int right = ultrasonic_right_distance;

	// 1. 전방이 매우 가까움 → 정지 후 후진
	if (center <= CRITICAL_DISTANCE) {
		stop();
		_delay_us(100);
		backward(back_speed);

		if (left >= right) {
			turn_left(turn_speed);
			} else {
			turn_right(turn_speed);
		}
		return;
	}

	// 2. 전방이 적당히 열려있고, 좌우 거리 차이가 크지 않으면 직진
	if (center > CENTER_WARNING_RANGE - 40) {  // 전방이 꽤 열려있음
		int side_diff = abs(left - right);

		if (left > SIDE_WARNING_RANGE && right > SIDE_WARNING_RANGE) {
			forward(normal_speed);  // 완전히 열려 있으면 빠르게 직진
			} else if (side_diff < 60) {
			forward(slow_speed);    // 좌우가 살짝 좁더라도 유사하면 천천히 직진
			} else {
			// 한쪽만 좁고 좌우 차이가 큼 → 회피
			if (left > right) {
				turn_left(turn_speed);
				} else {
				turn_right(turn_speed);
			}
		}
		return;
	}

	// 3. 전방이 좁음 → 좌우 중 더 넓은 방향으로 회피
	if (center <= CENTER_WARNING_RANGE) {
		if (left <= SIDE_WARNING_RANGE && right <= SIDE_WARNING_RANGE) {
			backward(back_speed);
			_delay_us(100);

			if (left >= right) {
				turn_left(turn_speed);
				} else {
				turn_right(turn_speed);
			}
			return;
		}

		// 좌우 중 넓은 방향으로 회피
		if (left > right) {
			turn_left(turn_speed);
			} else {
			turn_right(turn_speed);
		}
		return;
	}
}

void init_autocar(void)
{
		init_timer0();
		init_timer1();
		init_uart0();
		init_uart1();
		init_button();
		init_L298N();
		init_led();
		init_ultrasonic();
		init_fnd();
		init_speaker();
	
}