/*
 * peripheral_drivers.h
 *
 * Created: 5/17/2017 10:56:58 PM
 *  Author: Cardoz
 */ 


#ifndef PERIPHERAL_DRIVERS_H_
#define PERIPHERAL_DRIVERS_H_
#include "Atmega_Config.h"

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))			
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))	

//#define PIR_INTERRUPT_VECT(number) INT##number##_vect
#define PIR_INTERRUPT_VECT 			INT1_vect						// PIR output should be input to controller on INT0
#define USS_INTERRUPT_VECT			INT0_vect
#define PIR_INPUT_PORT				PORTD
#define PIR_INPUT_DIR_REG			DDRD
#define PIR_INPUT_PIN				PIND3				

#define USS_TRIGGER_PIN				PORTD4
#define USS_TRIGGER_PORT			PORTD
#define USS_TRIGGER_DIR_REG			DDRD
#define USS_ECHO_PIN				PIND2
#define USS_ECHO_PORT				PORTD
#define USS_ECHO_DIR_REG			DDRD

#define RELAY_PIN					PORTB0
#define RELAY_PORT					PORTB
#define RELAY_PORT_DIR_REG			DDRB

#define RELAY_ON					(RELAY_PORT |= (1 << RELAY_PIN))
#define RELAY_OFF					(RELAY_PORT &= ~(1 << RELAY_PIN))

#define HW_RESET					4
#define HW_RESET_PORT				PORTC
#define HW_RESET_DIR				DDRC
#define RESET_HW					(HW_RESET_PORT &= (1 << HW_RESET))

/*********************************************************************************************************************
*					Local variables
********************************************************************************************************************/


/**********************************************************************************************************************
*					Function Prototypes
*********************************************************************************************************************/
void Trigger_Ultrasonic_Sensor();
void Init_Pir();
void Init_Ultrasonic_Sensor();
uint16_t Get_Uss_Count();
uint8_t Get_Pir_count();
void Reset_Pir_count();
uint8_t Get_Pir_status();
void Led_On(uint8_t led_color);
void Led_Off(uint8_t led_color);
void Init_Gpio();
void init_relay();
void Stop_Timer1();
void Start_Timer1();
void init_timer2();
void Init_Timer1();
void Init_Timer0();

#endif /* PERIPHERAL_DRIVERS_H_ */