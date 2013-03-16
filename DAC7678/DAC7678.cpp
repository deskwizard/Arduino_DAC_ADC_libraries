// Library version 0.1e. See example sketch for information, or read down
// Deskwizard 03/16/2013

#include "DAC7678.h"
#include "Wire.h"
#include "Arduino.h"

unsigned char off_mode[8]= {0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60}; // Default off mode to HIGH-Z

unsigned char command;  // Command Byte
unsigned char msdb;     // Most Significant Data Byte
unsigned char lsdb;		// Least Significant Data Byte	
int address;		// DAC I2C address

DAC7678::DAC7678(unsigned char _address){
  address = _address;   // Set DAC to address passed to function
}

void DAC7678::init(){
  Wire.begin();   // Initialize I2C Bus
  reset(); 		  // Sends Power-On Reset command to DAC
}

void DAC7678::setVREF(bool _refstate){
  // Sets reference mode
  // _refstate 0: Internal vref turned off
  // _refstate 1: Internal reference on
  unsigned char msdb;
  unsigned char lsdb;
  unsigned char command;

  command = 0x80;
  msdb = 0x00;
  
    if (_refstate){
      lsdb = 0x10;
    }
    else if (!_refstate){
      lsdb = 0x00;
    }
  transmit(command, msdb, lsdb); 
}


void DAC7678::reset(){
  // Issues Reset command similar to Power-On reset.
  transmit(0x70, 0x00, 0x00);
}

void DAC7678::offMode(unsigned char channel, unsigned char mode){

off_mode[channel] = mode;

}

void DAC7678::offMode(unsigned char mode){

  for (unsigned char x = 0; x <= 7; x++){
  off_mode[x] = mode;
  }

}

void DAC7678::enable(unsigned char channel, unsigned char state){
  // Sets Power-Down register for the specified DAC channel
  // a.k.a  Power-Down Register with specific channels bit set.
  // Enables or Disables (Powers up or down) the specific DAC output
  
  if (channel >= 0 || channel < 8){
     unsigned char _command = 0x40;
     unsigned int _value = 2 << channel;    
	 unsigned char msdb = 0;

	 if(!state)
	 {
	 msdb = _value >> 4;
	 msdb |= off_mode[channel];
	 }
	if(state)
	 {
	 msdb = _value >> 4;
	 }
 
  unsigned char lsdb = _value << 4;
  transmit(_command, msdb, lsdb);
  }

}


void DAC7678::enable(unsigned char state){
  // Sets Power-Down register for all DAC channels
  // a.k.a  Power-Down Register with all channels bit set.
  // Enables or Disables all the channels at once

  unsigned char _command = 0x40;
  unsigned char lsdb = 0xE0; 
  unsigned char msdb = 0;

  if (!state)
  {
  for (unsigned char x = 0; x <= 7; x++){
	unsigned int _value = 2 << x;  
  	 msdb = _value >> 4;
	 msdb |= off_mode[x];
	 lsdb = _value << 4;
	 transmit(_command, msdb, lsdb);
  }
  }
  if (state)
  {
  msdb = 0x1F;
  transmit(_command, msdb, lsdb);
  }
}

void DAC7678::set(int _value){
  // Sets all DAC channels to specified value 
  // a.k.a. Individual Software LDAC Register with broadcast address
  // Will update and latch all channels
  
  // Check for out of range values
  if (_value >= 4096){
      _value = 4095; 
    }
  if (_value < 0){
      _value = 0; 
    }
  // Sets the variables
  unsigned char _command = 0x3F;   
  unsigned char msdb = _value >> 4;
  unsigned char lsdb = _value << 4;
  // Send data to DAC
  transmit(_command, msdb, lsdb);
}

void DAC7678::set(unsigned char channel, int _value){
  // Set specified channel (0-7) to the specified value
  // a.k.a. Individual Software LDAC Register with specific channel address
  // Will  update and latch specified channel
  
  //   Check for out of range values
  if (_value >= 4096){
      _value = 4095; 
    }
  if (_value < 0){
      _value = 0; 
    }

  if (channel >= 0 || channel < 8){ // Check for out of range Channel #
    // Sets the variables
    unsigned char _command = 0x30 + channel;
    unsigned char msdb = _value >> 4;
    unsigned char lsdb = _value << 4;
	// Send data to DAC
    transmit(_command, msdb, lsdb);
  }
}

void DAC7678::clrMode(int _value)
{
// Sets the DAC value to output on all channels when CLR pin is brought low
// a.k.a. Clear Code Register
// Will set the CLR Code register to output either zero, half-range, full range or to ignore the CLR pin
    unsigned char _command = 0x50;
    unsigned char msdb = 0x00;
    unsigned char lsdb = _value << 4;
	// Send data to DAC
    transmit(_command, msdb, lsdb);

}

unsigned int DAC7678::readChan(unsigned char _command)
{
  unsigned int reading = 0;
  Wire.beginTransmission(address); // Send a start or repeated start command with a slave address and the R/W bit set to '0' for writing.
  Wire.write(_command);      // Then send a command byte for the register to be read.
  Wire.endTransmission();
  Wire.requestFrom(address, 2);
  if(2 <= Wire.available())    // if two bytes were received
  {
    reading = Wire.read();  // receive high byte
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.read();
    reading = reading >> 4;
  }
  return reading;
}

unsigned int DAC7678::readDAC(unsigned char _command)
{
  unsigned int reading = 0;
  Wire.beginTransmission(address); // Send a start or repeated start command with a slave address and the R/W bit set to '0' for writing.
  Wire.write(_command);      // Then send a command byte for the register to be read.
  Wire.endTransmission();
  Wire.requestFrom(address, 2);
  if(2 <= Wire.available())    // if two bytes were received
  {
    reading = Wire.read();     // receive high byte
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.read();
    reading = reading >> 4;
  }
  return reading;
}

void DAC7678::transmit(unsigned char _command, unsigned char _msdb, unsigned char _lsdb){
  // Transmit the actual command and high and low bytes to the DAC
  Wire.beginTransmission(address);
  Wire.write(_command);
  Wire.write(_msdb);
  Wire.write(_lsdb);
  Wire.endTransmission();

}