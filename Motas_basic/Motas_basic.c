/*
 * Motas_basic.c
 *
 * Created: 14-01-2018 09:58:02
 *  Author: Cardoz
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Atmega_Config.h"
#include "Peripheral_Drivers.h"
#include "Atmega_Uart.h"
#include "debug_func.h"
#include "Dfplayer.h"

static uint16_t uss_threshold_count = 0, number_of_tracks = 0;
static uint8_t flag_register = 0, track_number = 1;
static system_state_t system_state = STANBY_STATE;

#define flag_uss_detection REGISTER_BIT(flag_register, 0)
#define flag_pir_detection REGISTER_BIT(flag_register, 1)
#define flag_track_playing REGISTER_BIT(flag_register, 2)

#define PIR_THRESHOLD_COUNT 0

/* function prototypes	*/
void check_uss_detection();
void check_pir_detection();

int main(void)
{
	/* Initialize all system peripherals	*/
	Init_Pir();
	Init_Ultrasonic_Sensor();
	Init_Timer0();					// Setup the timer to trigger the USS at regular intervals.
	Init_Timer1();					// Setup the timer that will count the duration(lenght) of the echo'ed USS pulse
	USART_Init();
	init_relay();
	DebugLedInit();
	
	_delay_ms(15000);				// Startup delay to warm up the PIR sensor
	
	sei();
	
	DebugLedTransmit(LED_ON, YELLOW_LED);
	_delay_ms(1000);
	DebugLedTransmit(LED_ON, LED_RED);
	_delay_ms(1000);
	DebugLedTransmit(LED_OFF, YELLOW_LED);
	_delay_ms(1000);
	DebugLedTransmit(LED_OFF, LED_RED);
	
	#if DEBUG_MSG_ON
		SendDebug("Serial OK!");
	#endif
	
	/* Get a USS threshold value */
	Trigger_Ultrasonic_Sensor();
	_delay_ms(1000);
	uss_threshold_count = Get_Uss_Count();
	/* reduce the count by 30% */
	uss_threshold_count -= 1000; 
	#if DEBUG_MSG_ON
		SendDebug("USS calibrated at:");
		USART_Transmit_dec(uss_threshold_count);
	#endif
	
	/* Find the number of files present in the SD card	*/
	number_of_tracks = Dfplayer_Query_Files();
	#if DEBUG_MSG_ON
		SendDebug("tracks: ");
		USART_Transmit_dec(number_of_tracks);
		USART_SendByte(0x0a);
	#endif
		
	/* Reset the mp3 player module	*/
	Dfplayer_Cmd(CMD_RESET, 0);	
	
	/* start main application	*/
    while(1)
    {		
		
		#if DEBUG_LED_ON
			DebugLedTransmit(LED_ON, LED_RED);
			_delay_ms(50);
			DebugLedTransmit(LED_OFF, LED_RED);
			_delay_ms(100);
		#endif
		
		switch(system_state)
		{
			case STANBY_STATE:
				#if DEBUG_LED_ON
					DebugLedTransmit(LED_ON, YELLOW_LED);
				#endif
				
				check_pir_detection();
				check_uss_detection();
				RELAY_OFF;
				#if DEBUG_MSG_ON
					USART_SendByte(0x0a);
					SendDebug("STANDBY");
					SendDebug(" flag_USS:");
					USART_Transmit_dec(flag_uss_detection);
					SendDebug(" flag_PIR:");
					USART_Transmit_dec(flag_pir_detection);
				#endif
				
				/* Area entry detection	*/
//				if(flag_uss_detection || flag_pir_detection)	//vineet elevator
				if(flag_pir_detection)		// vineet signboard
				{
					/* re-checking the PIR sensor for motion if presence deteced by USS	*/
					check_pir_detection();
					
					if(flag_pir_detection == TRUE)
					{
						#if DEBUG_MSG_ON
							USART_SendByte(0x0a);
							SendDebug("Playing track: ");
							USART_Transmit_dec(track_number);
						#endif
						
						Dfplayer_Cmd(CMD_PLY_TRCK, track_number);		// Send command to mp3 player module to play track
						_delay_ms(1000);
						track_number++;
						if(track_number > number_of_tracks)
							track_number = 1;
						
						flag_track_playing = TRUE;
						system_state = ACTIVE_STATE;
						#if DEBUG_MSG_ON
							SendDebug("Presence detected");
							USART_SendByte(0x0a);
						#endif	
					}
					flag_uss_detection = FALSE;
					flag_pir_detection = FALSE;
					Reset_Pir_count();
				}
				break;
			
			case ACTIVE_STATE:
				#if DEBUG_LED_ON
					DebugLedTransmit(LED_OFF, YELLOW_LED);
				#endif
				
				check_pir_detection();
				check_uss_detection();
				RELAY_ON;
				#if DEBUG_MSG_ON
					USART_SendByte(0x0a);
					SendDebug("ACTIVE ");
					SendDebug(" flag_USS:");
					USART_Transmit_dec(flag_uss_detection);
					SendDebug(" flag_PIR:");
					USART_Transmit_dec(flag_pir_detection);
				#endif
				
				/* Area exit detection	*/
				//if(flag_uss_detection)
				//{
					//flag_uss_detection = FALSE;
					///* Here we check for any triggering of the PIR sensor after the detection by the USS	*/
					//flag_pir_detection = FALSE;
					//Reset_Pir_count();
					//_delay_ms(8000);
					//check_pir_detection();
					//if(flag_pir_detection == FALSE)
					//{
						//Dfplayer_Cmd(CMD_PAUSE, 0);
										//
						//flag_track_playing = FALSE;
//
						//system_state = STANBY_STATE;
										//
						//#if DEBUG_MSG_ON
							//SendDebug("Exited");
							//USART_SendByte(0x0a);
						//#endif
					//}
				//}
				
				/* check validity of the track playing flag by querying the mp3 module for its status	*/
				/*else*/ if(FALSE == Dfplayer_Query_track_playing())
				{
					/****************** Case for Vineet	elevator*********************/
					//if(track_number > number_of_tracks) // number incremented when track was first played
						//track_number = 1;
					//#if DEBUG_MSG_ON
						//USART_SendByte(0x0a);
						//SendDebug("Playing track: ");
						//USART_Transmit_dec(track_number);
					//#endif
					//Dfplayer_Cmd(CMD_PLY_TRCK, track_number);
					//track_number++;
					//_delay_ms(1000);
					/****************** Case for Vineet	*********************/
					
					
					/****************** case for vineet signboard ******************/
					system_state = STANBY_STATE;
					Dfplayer_Cmd(CMD_PAUSE, 0);
					RELAY_OFF;
					_delay_ms(5000);
				}
				break;
				
			default:
				#if DEBUG_MSG_ON
					SendDebug("defaulted");
				#endif
				break;
		}
    }
}


/*	This function will only set the USS detection flag if the current USS count is less than the set threshold.
	The flag is to be cleared once it has been processed by the main application\
*/
void check_uss_detection()
{
	uint16_t current_uss_count = Get_Uss_Count();
	
	/* compare the current uss count with the threshold set	*/
	if(current_uss_count < uss_threshold_count)
	{
		flag_uss_detection = TRUE;
	}
}

/*	This function will only set the PIR detection flag if the current PIR count is less than the set threshold.
	The flag is to be cleared once it has been processed by the main application\
*/
void check_pir_detection()
{
	uint8_t current_pir_count = Get_Pir_count();
	
	/* compare the current pir count with the threshold set	*/
	if(current_pir_count > PIR_THRESHOLD_COUNT)
	{
		flag_pir_detection = TRUE;
	}
}