/*
 Library for the TI ADS7828 12bits i2C ADC
 Datasheet: http://www.ti.com/lit/ds/symlink/ads7828.pdf

 Deskwizard (03/16/2013)

 ------------------------------------------------------------------------------
 Library information -  Current Library version: 0.1b - March 16th 2013
 Tested working on IDE version 1.0.3

 This is a basic library to use the ADS7828 ADC.
 Only single ended mode is currently available.
 Both Internal and external voltage reference mode are available.
 Please read the datasheet if you are to use the internal reference!

 ------------------------------------------------------------------------------    
 Chip Wiring
 All connections necessary unless specifically mentionned.
 
 Connect GND pin to Ground
 Connect +VDD pin to 5v
 Connect SCL pin to Arduino SCL (Analog 5)
 Connect SDA pin to Arduino SDA (Analog 4)
 Connect A0 pin to either ground or 5v (set device address accordingly)
 Connect A1 pin to either ground or 5v (set device address accordingly) 
 Connect COM pin to ground (Single-ended mode common channel)
 
 CH0 - CH7 are obviously the ADC input channels
 
  ------------------------------------------------------------------------------    
 Function list
 
  init(INT)*                  Initialize the ADC and the I2C bus with internal voltage reference. (* Use one or the other)
  init()*                     Initialize the ADC and the I2C bus with external voltage reference. (* Use one or the other)
  read(channel)               Read the specified ADC channel (0-7) - Will return a value between 0-4095
  
*/

#ifndef ADS7828_H
#define ADS7828_H

#include "Arduino.h"
#include <Wire.h>

#define EXT 0 // External VREF
#define INT 1 // Internal VREF

class ADS7828 
{  


public:
	ADS7828(unsigned char _address);
	void init();
	void init(boolean _vref);
	unsigned int read(unsigned char channel);
	
};


#endif