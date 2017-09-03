/*
 Library for the TI DAC7678 12bits i2C DAC 
 Datasheet: http://www.ti.com/lit/ds/symlink/dac7678.pdf
 
 I tried to put as much comment and be clear as possible as I hate examples
 that lack any kind of information. The source code is also commented as much
 as I thought necessary/helpful.
 
 I hope it helps some people as I had quite a bit of fun with this one.
 
 Deskwizard (03/11/2013)
 
 ------------------------------------------------------------------------------
 Library information -  Current Library version: 0.1g - March 17th 2013
 Tested working on IDE versions 1.0.1, 1.0.2 and 1.0.3
 
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
 
 init()                       	Initialize the DAC (Initialize I2C bus and send Power-On Reset command)
 reset()                      	Send Power-On Reset command to DAC
 offMode((channel,) mode)     	Configure Power down mode for specific channel if specified or all channels if only the mode is specified 
 setVREF(INT|EXT)             	Sets reference mode to internal (INT) or external (EXT)   (Defaults to EXT) 
 clrMode(Clear_Mode);         	Reset all DAC to "Clear_Mode" when CLR pin is brought low (Values: FULL, MID, ZERO, NOCLR)
 
 
 DAC use Functions
 
 set((channel,) value)        	Set specified DAC channel to specified value if specified, or all channels if only the value is specified
 enable((channel,) PWON|PWOFF)  Powers ON or OFF the specific DAC Channels if specified, or all channels if only the state is specified.
 readChan(Channel)            	Read the current value of the specified DAC channel
 
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



#include "Arduino.h"
#include "DAC7678.h"
#include "Wire.h"


unsigned char off_mode[8]= {0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60}; // Default off mode to HIGH-Z

unsigned char command;  // Command Byte
unsigned char msdb;     // Most Significant Data Byte
unsigned char lsdb;		// Least Significant Data Byte	

DAC7678::DAC7678(unsigned char _address) {
  dac7678_address = _address;   // Set DAC to address passed to function
}

void DAC7678::init() {
  Wire.begin();   // Initialize I2C Bus
  reset(); 		  // Sends Power-On Reset command to DAC
}

void DAC7678::setVREF(bool _refstate) {
  // Sets reference mode
  // _refstate 0: Internal vref turned off
  // _refstate 1: Internal reference on
  unsigned char msdb;
  unsigned char lsdb;
  unsigned char command;

  command = 0x80;
  msdb = 0x00;
  
    if (_refstate) {
      lsdb = 0x10;
    }
    else if (!_refstate) {
      lsdb = 0x00;
    }
  transmit(command, msdb, lsdb); 
}


void DAC7678::reset() {
  // Issues Reset command similar to Power-On reset.
  transmit(0x70, 0x00, 0x00);
}

void DAC7678::offMode(unsigned char channel, unsigned char mode) {
    off_mode[channel] = mode;
}

void DAC7678::offMode(unsigned char mode) {

  for (unsigned char x = 0; x <= 7; x++){
    off_mode[x] = mode;
  }
}

void DAC7678::enable(unsigned char channel, unsigned char state) {
  // Sets Power-Down register for the specified DAC channel
  // a.k.a  Power-Down Register with specific channels bit set.
  // Enables or Disables (Powers up or down) the specific DAC output
  
  if (channel >= 0 || channel < 8) {
     unsigned char _command = 0x40;
     unsigned int _value = 2 << channel;    
	 unsigned char msdb = 0;

    if(!state) {
	    msdb = _value >> 4;
	    msdb |= off_mode[channel];
	}
	if(state) {
	    msdb = _value >> 4;
    }
 
  unsigned char lsdb = _value << 4;
  transmit(_command, msdb, lsdb);
  }
}


void DAC7678::enable(unsigned char state) {
  // Sets Power-Down register for all DAC channels
  // a.k.a  Power-Down Register with all channels bit set.
  // Enables or Disables all the channels at once

  unsigned char _command = 0x40;
  unsigned char lsdb = 0xE0; 
  unsigned char msdb = 0;

  if (!state) {

      for (unsigned char x = 0; x <= 7; x++) {
	    unsigned int _value = 2 << x;  
      	 msdb = _value >> 4;
	     msdb |= off_mode[x];
	     lsdb = _value << 4;
	     transmit(_command, msdb, lsdb);
      }
  }
  if (state) {
      msdb = 0x1F;
      transmit(_command, msdb, lsdb);
  }
}

void DAC7678::set(int _value) {
  // Sets all DAC channels to specified value 
  // a.k.a. Individual Software LDAC Register with broadcast address
  // Will update and latch all channels
  
  // Check for out of range values
  if (_value >= 4096) {
      _value = 4095; 
    }
  if (_value < 0) {
      _value = 0; 
    }
  // Sets the variables
  unsigned char _command = 0x3F;   
  unsigned char msdb = _value >> 4;
  unsigned char lsdb = _value << 4;
  // Send data to DAC
  transmit(_command, msdb, lsdb);
}

void DAC7678::set(unsigned char channel, int _value) {
  // Set specified channel (0-7) to the specified value
  // a.k.a. Individual Software LDAC Register with specific channel address
  // Will  update and latch specified channel
  
  //   Check for out of range values
  if (_value >= 4096) {
      _value = 4095; 
    }
  if (_value < 0) {
      _value = 0; 
    }

  if (channel >= 0 || channel < 8) { // Check for out of range Channel #
    // Sets the variables
    unsigned char _command = 0x30 + channel;
    unsigned char msdb = _value >> 4;
    unsigned char lsdb = _value << 4;
	// Send data to DAC
    transmit(_command, msdb, lsdb);
  }
}

void DAC7678::clrMode(int _value) {
// Sets the DAC value to output on all channels when CLR pin is brought low
// a.k.a. Clear Code Register
// Will set the CLR Code register to output either zero, half-range, full range or to ignore the CLR pin
    unsigned char _command = 0x50;
    unsigned char msdb = 0x00;
    unsigned char lsdb = _value << 4;
	// Send data to DAC
    transmit(_command, msdb, lsdb);
}

unsigned int DAC7678::readChan(unsigned char _command) {
  unsigned int reading = 0;
  Wire.beginTransmission(dac7678_address); // Send a start or repeated start command with a slave address and the R/W bit set to '0' for writing.
  Wire.write(_command);                    // Then send a command byte for the register to be read.
  Wire.endTransmission();
  Wire.requestFrom(dac7678_address, 2);

  if(2 <= Wire.available()) {       // if two bytes were received
    reading = Wire.read();          // receive high byte
    reading = reading << 8;         // shift high byte to be high 8 bits
    reading |= Wire.read();
    reading = reading >> 4;
  }
  return reading;
}

unsigned int DAC7678::readDAC(unsigned char _command) {

  unsigned int reading = 0;
  Wire.beginTransmission(dac7678_address); // Send a start or repeated start command with a slave address and the R/W bit set to '0' for writing.
  Wire.write(_command);      // Then send a command byte for the register to be read.
  Wire.endTransmission();
  Wire.requestFrom(dac7678_address, 2);

  if(2 <= Wire.available()) {   // if two bytes were received
    reading = Wire.read();      // receive high byte
    reading = reading << 8;     // shift high byte to be high 8 bits
    reading |= Wire.read();
    reading = reading >> 4;
  }
  return reading;
}

void DAC7678::transmit(unsigned char _command, unsigned char _msdb, unsigned char _lsdb) {
  // Transmit the actual command and high and low bytes to the DAC
  Wire.beginTransmission(dac7678_address);
  Wire.write(_command);
  Wire.write(_msdb);
  Wire.write(_lsdb);
  Wire.endTransmission();
}
