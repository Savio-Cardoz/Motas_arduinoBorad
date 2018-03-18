/*
 * dfplayer.c
 *
 * Created: 5/20/2017 12:37:57 AM
 *  Author: Cardoz
 *
 * Functions to access the status of DFPlayer Module. Send Commands.
 *  Note we are using micro-SD card is U-Disk for this module. Hence, while quering the number 
 * of files on card use the 'Query U-Disk files' command
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "Atmega_Uart.h"
#include "Dfplayer.h"

command_t command;

/************************************************************************
*		Function to send a command to the DFplayer
	
	argument 1: Type of command. Refer Dfplayer.h for command name definitions
	argument 2: Parameter to the command         
	                                                            
************************************************************************/
void Dfplayer_Cmd(uint8_t cmd, uint16_t parameter)
{
	uint8_t cmd_lenght = 0x06;			// Currently all commands that'll be used have a lenght of 6. So this is fixed
	uint8_t tempholder = 0;

	/*	Create the CMD string	*/
	command.header = CMD_STRT_BYTE;
	command.version = CMD_VER;
	command.lenght = cmd_lenght;
	command.type = cmd;
	command.feedback = CMD_RECV_NFB;
	command.parah = parameter / 256;
	command.paral = parameter % 256;
	command.tail = CMD_END_BYTE;
	
	flush_buffer();
	Dfplayer_Send(&command);

	command.parah = 0;
	command.paral = 0;
/*************************************************************************
*  If Commands are query based, a immediate response will be received
*************************************************************************/
	_delay_ms(50);
	if(Is_Buffer_Empty() == BUFFER_NOT_EMPTY)
	{
		do{
			tempholder = Ring_Remove();
		}while(tempholder != CMD_STRT_BYTE);
		command.version = Ring_Remove();				// Remove the VER byte from the buffer
		command.lenght = Ring_Remove();				// Remove the CMD byte from the buffer
		command.type = Ring_Remove();
		command.feedback = Ring_Remove();
		command.parah = Ring_Remove();
		command.paral = Ring_Remove();
		//command.tail = Ring_Remove();
	}
}

void Dfplayer_Send(command_t* strct_ptr)
{
	USART_SendByte(strct_ptr->header);
	USART_SendByte(strct_ptr->version);
	USART_SendByte(strct_ptr->lenght);
	USART_SendByte(strct_ptr->type);
	USART_SendByte(strct_ptr->feedback);
	USART_SendByte(strct_ptr->parah);
	USART_SendByte(strct_ptr->paral);
	USART_SendByte(strct_ptr->tail);
}



/* Query the DFPLAYER module for the presence of a card.
 * returns		0 if card is present
				1 if no card is present
 */				
				 
uint8_t Dfplayer_Query_Card_Presence()
{
	uint8_t tempholder = 0;
	command.header = CMD_STRT_BYTE;
	command.version = CMD_VER;
	command.lenght = 0x06;
	command.type = CMD_TF_FILES;
	command.feedback = CMD_RECV_NFB;
	command.parah = 0x00;
	command.paral = 0x00;
	command.tail = CMD_END_BYTE;
	
	flush_buffer();
	Dfplayer_Send(&command);

	/*************************************************************************
	*  If Commands are query based, a immediate response will be received
	*************************************************************************/
	_delay_ms(10);
	if(Is_Buffer_Empty() == BUFFER_NOT_EMPTY)
	{
		do{
			tempholder = Ring_Remove();
		}while(tempholder != CMD_STRT_BYTE);
		command.version = Ring_Remove();				// Remove the VER byte from the buffer
		command.lenght = Ring_Remove();				// Remove the CMD byte from the buffer
		command.type = Ring_Remove();
		command.feedback = Ring_Remove();
		command.parah = Ring_Remove();
		command.paral = Ring_Remove();
	}
	
	if(command.type == CMD_ERROR)
	return 1;
	
	else return 0;
}

/*	Query the DFplayer for the number of mp3 files on the microSD card.
	Parameters: None
	Returns:
*/	
uint16_t Dfplayer_Query_Files()
{
	uint8_t tempholder = 0;
	uint16_t num_of_files = 0;
	
	command.header = CMD_STRT_BYTE;
	command.version = CMD_VER;
	command.lenght = 0x06;
	command.type = CMD_TF_FILES;
	command.feedback = CMD_RECV_NFB;
	command.parah = 0x00;
	command.paral = 0x00;
	command.tail = CMD_END_BYTE;
	
	flush_buffer();
	Dfplayer_Send(&command);

	/*************************************************************************
	*  If Commands are query based, a immediate response will be received
	*************************************************************************/
	_delay_ms(50);
	if(Is_Buffer_Empty() == BUFFER_NOT_EMPTY)
	{
		do{
			tempholder = Ring_Remove();
		}while(tempholder != CMD_STRT_BYTE);
		command.version = Ring_Remove();				// Remove the VER byte from the buffer
		command.lenght = Ring_Remove();				// Remove the CMD byte from the buffer
		command.type = Ring_Remove();
		command.feedback = Ring_Remove();
		command.parah = Ring_Remove();
		command.paral = Ring_Remove();
	
		if(command.type != CMD_ERROR)
		{
			num_of_files = command.parah;
			num_of_files = ((num_of_files << 8) + command.paral);
		}
	}
	return num_of_files;
}

uint16_t Dfplayer_Query_Curr_track()
{
	uint16_t curr_track = 0;
	
	flush_buffer();
	Dfplayer_Cmd(CMD_CURR_TRCK, 0);
	curr_track = command.parah;
	curr_track = command.paral + (curr_track << 8);
	return curr_track;
}

uint8_t Dfplayer_Query_track_playing()
{
	uint8_t track_playing = FALSE;
	
	flush_buffer();
	Dfplayer_Cmd(CMD_STATUS, 0);
	
	#if DEBUG_MSG_ON
		SendDebug("track state: ");
		USART_SendByte(command.paral);
		USART_SendByte(0x0a);
	#endif
	
	if(command.paral != 0)
		track_playing = TRUE;
	
	return track_playing;
}

