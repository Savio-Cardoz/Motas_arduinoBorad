/*
 * Atmega_Config.c
 *
 * Created: 5/23/2017 10:55:15 PM
 *  Author: Cardoz
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "Atmega_Config.h"
#include "Peripheral_Drivers.h"
#include "debug_func.h"
#include "Atmega_Uart.h"

void Atmega_init()
{	
	USART_Init();
#if DEBUG_MSG_ON
	SendDebug("Serial OK!");
#endif 
	//Init_Ultrasonic_Sensor();		//FK
	Init_Pir();
	//init_timer2();
	//Init_Timer1();
	//Init_Timer0();
	Init_Gpio();
	DebugLedInit();
	//Trigger_Ultrasonic_Sensor();
	sei();
}