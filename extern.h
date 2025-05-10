
#ifndef EXTERN_H_
#define EXTERN_H_

#include <stdint.h>

//button.c
extern void init_button(void);
extern int get_button(int button_num, int button_pin);
extern void auto_mode_check(void);
extern void init_led(void);
extern volatile int button_state;	//button0==>auto mode, manual mode

//pwm.c
extern void init_timer1(void);
extern void init_L298N(void);
extern void stop(void);
extern void forward(int speed);
extern void backward(int speed);
extern void turn_left(int speed);
extern void turn_right(int speed);

//fnd.c
extern void init_fnd(void);
extern int fnd_main(void);
extern void fnd_display(int mode);

//UART.c
extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);
extern char UART0_Receive(void);
extern volatile uint8_t bt_data;
extern volatile uint8_t uart0_data;
extern int func_index;

//Ultrasonic.c
extern void init_ultrasonic(void);
extern void distance_ultrasonic(void);
extern void ultrasonic_distance_check(void);

//main.c
extern volatile int msec_count;
extern volatile int siren_count;
extern volatile int lcd_count;
extern volatile int auto_count;

//buzzer.c
extern void Auto_siren(void);
extern void cuby_buzzer(void);
extern void init_speaker(void);
extern const int Stage_Clear_Beats[];
extern const int Stage_Clear_Tune[];

//I2c_LCD.c
extern void I2C_LCD_Normal(void);
extern void I2C_LCD_Auto(void);



#endif /* EXTERN_H_ */