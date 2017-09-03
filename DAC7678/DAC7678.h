/*
 Library for the TI DAC7678 12bits i2C DAC 
 Datasheet: http://www.ti.com/lit/ds/symlink/dac7678.pdf
 
 I tried to put as much comment and be clear as possible as I hate examples
 that lack any kind of information. The source code is also commented as much
 as I thought necessary/helpful.
 
 I hope it helps some people as I had quite a bit of fun with this one.
 
 Deskwizard (03/11/2013)
 
 ------------------------------------------------------------------------------
 Library information -  Current Library version: 0.1h - September 3rd 2017
 
 * Internal VREF modes are partially implemented. (Only static mode is implemented)
 * All power states are implemented.
 * Partial writing and reading is implemented
 * Only software LDAC is currently implemented (Synchronous mode). Please permanently connect the LDAC to ground.
 
 ------------------------------------------------------------------------------    
 Chip Wiring
 All connections necessary unless specifically mentionned. (* = optional)
 
 Connect GND pin to Ground
 Connect AVDD pin to 5v
 Connect LDAC pin to ground
 Connect SCL pin to Arduino SCL (Analog 5)
 Connect SDA pin to Arduino SDA (Analog 4)
 Connect CLR pin to 5v through a 10k resistor (CLR ---/\/10k\/\--- 5v)
 * Connect VREF to external reference -- Please use your brain and read datasheet !! --
 
 
 ------------------------------------------------------------------------------    
 Channel naming 
 You can use either the number of the pin or the name mentionned here.
 
 Library  |  Chip Pin (Datasheet)
 0    =    PIN_A
 1    =    PIN_B
 ...
 7    =    PIN_H
 
 ------------------------------------------------------------------------------    
 Function list (value is 0-4095 unless otherwise noted.)
 
 Control/Setup Functions
 
 init()                       	  Initialize the DAC (Initialize I2C bus and send Power-On Reset command)
 reset()                      	  Send Power-On Reset command to DAC
 offMode((channel,) mode)     	  Configure Power down mode for specific channel if specified or all channels if only the mode is specified 
 setVREF(INT|EXT)             	  Sets reference mode to internal (INT) or external (EXT)   (Defaults to EXT) 
 clrMode(Clear_Mode);         	  Reset all DAC to "Clear_Mode" when CLR pin is brought low (Values: FULL, MID, ZERO, NOCLR)
 
 
 DAC use Functions
 
 set((channel,) value)    		  Set specified DAC channel to specified value if specified, or all channels if only the value is specified
 enable((channel,) PWON|PWOFF)    Powers ON or OFF the specific DAC Channels if specified, or all channels if only the state is specified.
 readChan(Channel)            	  Read the current value of the specified DAC channel
 
 Note: The set commands will accept out of range values, but will set the actual DAC to 0 for values under 0
 and 4095 for values over 4095 to prevent unexpected behavior.
 
 ------------------------------------------------------------------------------    
 Power Off mode information
 
 The Power off state of the channels have 3 possible states on the DAC7678 and are used as follows:
 
 HIGHZ  -  DAC channel(s) powered OFF, Pin set to High-Z mode (High Impedance)
 L1K    -  DAC channel(s) powered OFF, Pin pulled down through 1K resistor
 L100K  -  DAC channel(s) powered OFF, Pin pulled down through 100K resistor 
 
 
 ------------------------------------------------------------------------------    
 
 
 */
#ifndef DAC7678_H
#define DAC7678_H

#include "Arduino.h"
#include <Wire.h>

// VREF options (Flexible Mode)
#define EXT 0 // External VREF
#define INT 1 // Internal VREF

// dac power mode (new)
#define L100K 0x40
#define L1K 0x20
#define HIGHZ 0x60
#define PWON	1
#define PWOFF 0
// Clear pin Modes
#define NOCLR 3
#define MID 1
#define FULL 2
#define ZERO 0
// Channel # to Pin name numbering
#define PIN_A 0
#define PIN_B 1
#define PIN_C 2
#define PIN_D 3
#define PIN_E 4
#define PIN_F 5
#define PIN_G 6
#define PIN_H 7

class DAC7678 
{  


public:
	DAC7678(unsigned char _address);
	void init();
	void setVREF(bool _refstate);
	void reset();
	void offMode(unsigned char channel, unsigned char mode);
	void offMode(unsigned char mode);
	void enable(unsigned char state);
	void enable(unsigned char channel, unsigned char state);
	void set(int _value);
	void set(unsigned char channel, int _value);
	void clrMode(int _value);
	unsigned char DAC;
	unsigned int readChan(unsigned char _command);
	unsigned int readDAC(unsigned char _command);
	
	private:
	void transmit(unsigned char _command, unsigned char _msdb, unsigned char _lsdb);
    int dac7678_address;		// DAC I2C address
};

#endif
