/*
 * uart0.c
 *
 * Created: 2025-03-10 오전 10:32:53
 *  Author: microsoft
 */ 

#include "uart0.h"

void init_uart0(void);
void UART0_transmit(uint8_t data);
void pc_command_processing(void);
char UART0_Receive(void) ;

/*
PC comportmaster로 부터 1byte가 들어오면 (즉, stop bit가 감지되면) RX complete인터럽트가 호출된다
== 한 바이트마다 인터럽트 처리를 해야하는것!
*/
volatile uint8_t uart0_data=0;

ISR(USART0_RX_vect)
{
	uart0_data = UDR0;  // 수신된 데이터를 uart0_data 변수에 저장
	//UART0_transmit(uart0_data);
}


/*
[init에 고려해야할것]

전송속도 : 9600bps -> 1초에 960자를 받음
				1글자 송수신에 약 1ms가 걸린다 (1글자 == 10비트)

비동기 방식 : start, stop 비트의 활용

RX : interrupt (받는것)
TX : polling (보내는것)

*/
void init_uart0(void)
{
	// 전송속도 설정
	UBRR0H = 0x00;
	UBRR0L = 207; // 2배속 모드임
	//UBRR0L = 16;
	//UBRR0L = 832;
	
	UCSR0A |= 1 << U2X0; // 2배속 통신
	
	UCSR0C |= 0x06; // 비동기, none 8bit, data 8 bit
	
	// 인터럽트, uart 기능 설정
	//UCSR0B |= 1 << RXEN0; // 수신 허용
	//UCSR0B |= 1 << TXEN0; // 송신 허용
	//UCSR0B |= 1 << RXCIE0; // 수신 인터럽트 허용
	UCSR0B |= 1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0;
	
}

void UART0_transmit(uint8_t data) // 문자 송신
{
	// 데이터가 전송중이면, 끝날때까지 기다려야함
	// == 레지스터가 비었는지 안비었는지 확인해양함
	while(!(UCSR0A & 1 << UDRE0)) ; // 버퍼가 비어있을때까지 여기서 무한대기
	
	UDR0 = data; // data를 h/w에 전송
}