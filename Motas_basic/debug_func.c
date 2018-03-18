/*
 * debug_func.c
 *
 * Created: 9/23/2017 5:41:28 PM
 *  Author: Cardoz
 */ 

#include <avr/io.h>
#include "debug_func.h"

void DebugLedInit()
{
	RED_LED_DIR_REG |= (1 << RED_LED);
	YELLOW_LED_DIR_REG |= (1 << YELLOW_LED);
	
	RED_LED_PORT &= ~(1 << RED_LED);
	YELLOW_LED_PORT &= ~(1 << YELLOW_LED);
	
	ARDUINO_LED_DIR_REG |= (1 << ARDUINO_LED_PIN);
	ARDUINO_LED_PORT &= ~(1 << ARDUINO_LED_PIN);
}


/*
		To control a LED on the external GPIO for debugging
		param led_state :	LED_ON	selected colour light will glow
							LED_OFF	light wil switch off
							
		param led_colour:	LED_RED
							LED_YELLOW
							LED_ORANGE
*/
void DebugLedTransmit(debug_led_state_t led_state, debug_led_colour_t led_colour)
{
	if(led_state == LED_ON)
	{
		switch(led_colour)
		{
			case LED_RED:
				RED_LED_PORT |= (1 << RED_LED);
				break;
				
			case LED_YELLOW:
				YELLOW_LED_PORT |= (1 << YELLOW_LED);
				break;
				
			case LED_ORANGE:
				RED_LED_PORT |= (1 << RED_LED);
				YELLOW_LED_PORT |= (1 << YELLOW_LED);
				break;
				
			case ARDUINO_LED:
				ARDUINO_LED_PORT |= (1 << ARDUINO_LED_PIN);
				break;
		} 
	}
	else if(led_state == LED_OFF)
	{
		switch(led_colour)
		{
			case LED_RED:
				RED_LED_PORT &= ~(1 << RED_LED);
				break;
		
			case LED_YELLOW:
				YELLOW_LED_PORT &= ~(1 << YELLOW_LED);
				break;
		
			case LED_ORANGE:
				RED_LED_PORT &= ~(1 << RED_LED);
				YELLOW_LED_PORT &= ~(1 << YELLOW_LED);
				break;
			
			case ARDUINO_LED:
				ARDUINO_LED_PORT &= ~(1 << ARDUINO_LED_PIN);
				break;
		}
	}
}