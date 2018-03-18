/*
 * Atmega_Config.h
 *
 * Created: 5/23/2017 10:54:53 PM
 *  Author: Cardoz
 */ 


#ifndef ATMEGA_CONFIG_H_
#define ATMEGA_CONFIG_H_

//This is used to switch ON/OFF Debugging. Comment for switching the debugging mode to off.
//#define DEBUG_MSG_ON
#ifndef F_CPU
#define F_CPU 1000000UL
#endif

typedef struct			// Using to access individual bits/pins of a register/port
{
	unsigned int bit0:1;
	unsigned int bit1:1;
	unsigned int bit2:1;
	unsigned int bit3:1;
	unsigned int bit4:1;
	unsigned int bit5:1;
	unsigned int bit6:1;
	unsigned int bit7:1;
} reg;

#define REGISTER_BIT(rg,bt) ((volatile reg*)&rg)->bit##bt

typedef enum 
{
	FALSE = 0,
	TRUE
} detection_t;

typedef enum
{
	STANBY_STATE = 0,
	ACTIVE_STATE
}system_state_t;

#endif /* ATMEGA_CONFIG_H_ */