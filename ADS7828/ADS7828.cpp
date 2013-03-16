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

#include "Arduino.h"
#include "ADS7828.h"
#include "Wire.h"

int address;		// ADC I2C address
bool vref_int = 0;  // default voltage reference is external

// command address for the channels, allows 0-7 channel mapping in the correct order
unsigned char chan_register[8] = {0x00, 0x40, 0x10, 0x50, 0x20, 0x60, 0x30, 0x70}; 

ADS7828::ADS7828(unsigned char _address){
  address = _address;   // Set ADC i2c address to the one passed to the function
}

void ADS7828::init(){
  Wire.begin();   // Initialize I2C Bus
}

void ADS7828::init(boolean _vref){
  Wire.begin();   // Initialize I2C Bus
  vref_int = _vref;  // Set vref trigger to internal
}

unsigned int ADS7828::read(unsigned char channel)
{
  unsigned char command = 0;		// Initialize command variable
  unsigned int reading = 0;			// Initialize reading variable
  
  command = chan_register[channel]; // put required channel address in command variable

  command = command ^ 0x80; 		// Enable Single-ended mode (toggle MSB, SD bit in datasheet)
  
  if (vref_int){ 
    command = command ^ 0x08; 	    // Enable internal voltage reference if vref_int = 1
  }

  Wire.beginTransmission(address); 	// Send a start or repeated start command with a slave address and the R/W bit set to '0' for writing.
  Wire.write(command);      		// Then send a command byte for the register to be read.
  Wire.endTransmission();			// Send stop command
  
  Wire.requestFrom(address, 2);		// Request 2 bytes from the ADC

  if(2 <= Wire.available())    		// if two bytes were received
  {
    reading = Wire.read();     		// receive high byte
    reading = reading << 8;    		// shift high byte to be high 8 bits
    reading |= Wire.read();    		// receive low byte into lower 8 bits
  }
  return reading;					// return the full 12 bits reading  from the channel
}
