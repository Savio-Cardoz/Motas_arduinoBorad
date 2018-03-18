/*
 * atmegaUart.c
 *
 * Created: 5/19/2017 12:53:43 AM
 *  Author: Cardoz
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Atmega_Uart.h"

ISR(USART_RX_vect)		// USART interrupt service routine.
{
	uint8_t err;
	err = Ring_Add(UDR0);

	if (err == -1)
	{
		/* Error Handling code here. We may need to think if we need error handling for this */
	}
}
 
void USART_Init(void)
{
   //UBRR0L = BAUD_PRESCALE;// Load lower 8-bits into the low byte of the UBRR register
   //UBRR0H = (BAUD_PRESCALE >> 8); 
	 /* Load upper 8-bits into the high byte of the UBRR register
    Default frame format is 8 data bits, no parity, 1 stop bit
  to change use UCSRC, see AVR datasheet*/ 
	UBRR0 = 12;
	UCSR0A |= (1 << U2X0);
  // Enable receiver and transmitter and receive complete interrupt 
  UCSR0B = ((1<<TXEN0)|(1<<RXEN0) | (1<<RXCIE0));
}


void USART_SendByte(uint8_t u8Data)
{
  while((UCSR0A &(1<<UDRE0)) == 0);
  UDR0 = u8Data;
}

//void USART_putstring(char* StringPtr)
//{
	//while(*StringPtr != 0x00)
	//{
		//USART_SendByte(*StringPtr);
		//StringPtr++;
	//}
//}

/**
* This method is used to send debug messages on the UART
* @author Faisal Khan
* @param none
* @date 29/05/2017
*/
void SendDebug(char * StringPtr)
{
	while(*StringPtr != 0x00)
	{
		USART_SendByte(*StringPtr);
		StringPtr++;
	}
}

uint8_t USART_ReceiveByte()
{
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

void USART_Transmit_dec(unsigned int int_data)
{
	unsigned int temp1, temp2, dec_adc_value = 0;
	
	temp1 = int_data % 4096;
	temp2 = int_data / 4096;
	dec_adc_value = dec_adc_value + (temp2 * 4096);
	temp2 = temp1;
	temp1 = temp1 % 256;
	temp2 = temp2 / 256;
	dec_adc_value = dec_adc_value + (temp2 * 256);
	temp2 = temp1;
	temp1 = temp1 % 16;
	temp2 = temp2 / 16;
	dec_adc_value = (dec_adc_value + (temp2 * 16) + temp1);
	temp1 = dec_adc_value;
	USART_SendByte((dec_adc_value / 10000) | 0x30);
	dec_adc_value = temp1 % 10000;
	USART_SendByte((dec_adc_value / 1000) | 0x30);
	dec_adc_value = temp1 % 1000;
	USART_SendByte((dec_adc_value /100) | 0x30);
	dec_adc_value = temp1 % 100;
	USART_SendByte((dec_adc_value / 10) | 0x30);
	dec_adc_value = temp1 % 10;
	USART_SendByte(dec_adc_value | 0x30);
}

int Ring_Add(char c) {
    ring_pos_t next_head = (ring_head + 1) % RING_SIZE;
    if (next_head != ring_tail) {
        /* there is room */
        ring_data[ring_head] = c;
        ring_head = next_head;
        return 0;
    } else {
        /* no room left in the buffer */
        return -1;
    }
}

int Ring_Remove(void) {
    if (ring_head != ring_tail) {
        int c = ring_data[ring_tail];
        ring_tail = (ring_tail + 1) % RING_SIZE;
        return c;
    } else {
        return -1;
    }
}

uint8_t Is_Buffer_Empty()
{
	if(ring_head == ring_tail)
		return BUFFER_EMPTY;	
	else return BUFFER_NOT_EMPTY;
}

void flush_buffer()
{
	ring_head = ring_tail = 0;
}