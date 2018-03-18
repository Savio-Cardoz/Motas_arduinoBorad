/*
 * atmegaUart.h
 *
 * Created: 5/19/2017 12:53:28 AM
 *  Author: Cardoz
 */ 


#ifndef ATMEGAUART_H_
#define ATMEGAUART_H_

#include <avr/io.h>
#include "Atmega_Config.h"

/*************************************************************************
*		Ring Buffer declarations
*************************************************************************/
#define RING_SIZE   12
#define BUFFER_EMPTY 0
#define BUFFER_NOT_EMPTY 1
typedef uint8_t ring_pos_t;
volatile ring_pos_t ring_head;
volatile ring_pos_t ring_tail;
volatile char ring_data[RING_SIZE];

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 8UL))) - 1)
/*************************************************************************/

void USART_Init(void);
void USART_SendByte(uint8_t u8Data);
void USART_putstring(char* StringPtr);
void SendDebug(char * StringPtr);
uint8_t USART_ReceiveByte();
void USART_Transmit_dec(unsigned int int_data);
int Ring_Add(char c);
int Ring_Remove(void);
uint8_t Is_Buffer_Empty();
void flush_buffer();



#endif /* ATMEGAUART_H_ */