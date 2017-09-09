#include "Arduino.h"
#include "Wire.h"
#include "DAC7678.h"
#include "defines.h"


DAC7678::DAC7678(unsigned char _address) {
  dac7678_address = _address;   // Set DAC to address passed to transmit function

  // Preload HIGH-Z off mode - that way you can change it before begin()
  for (unsigned char x = 0; x <= 7; x++){
    off_mode[x] = HIGHZ;
  }

}

void DAC7678::begin() {
  Wire.begin();   // Initialize I2C Bus
  reset(); 		  // Sends Power-On Reset command to DAC
  disable();      // Make sure outputs are powered off
}

void DAC7678::setVREF(bool _refstate) {
  // Sets reference mode
  // _refstate 0: Internal vref turned off
  // _refstate 1: Internal vref turned on

  unsigned char lsdb;

  if (_refstate) {
        lsdb = 0x10;
    }
  else if (!_refstate) {
    lsdb = 0x00;
  }

  transmit(CMD_INTREF_RS, 0x00, lsdb); 
}


void DAC7678::reset() {
  // Issues Reset command (Same as Power-On reset)
  transmit(CMD_SOFT_RESET, 0x00, 0x00);
}

void DAC7678::offMode(unsigned char channel, unsigned char mode) {
  // Configure off mode for specified channel
  off_mode[channel] = mode;
}

void DAC7678::offMode(unsigned char mode) {
  // Configure off mode for all channels
  for (unsigned char x = 0; x <= 7; x++){
    off_mode[x] = mode;
  }
}

void DAC7678::LDAC(bool _state) {
  // Configure LDAC mode for all channels
  if (_state) {
    LDAC_reg = 0x00;        
  }
  else {
    LDAC_reg = 0xFF;
  }

  transmit(CMD_LDAC, LDAC_reg, 0x00);
}

void DAC7678::LDAC(unsigned char _channel, bool _state) {
  // Configure LDAC mode for specified channel
  if (_state) {
    LDAC_reg &= ~(1 << _channel);   
  }
  else {
    LDAC_reg |= (1 << _channel); 
  }

  transmit(CMD_LDAC, LDAC_reg, 0x00);
}

void DAC7678::select(unsigned char _channel) {
  // Select channel for update
  transmit(CMD_SELECT + _channel, 0x00, 0x00);
}

void DAC7678::enable() {
  // Enables all the channels at once

  transmit(CMD_POWER_DOWN, 0x1F, 0xE0);
}

void DAC7678::disable() {
  // Disables all the channels in one go
  // Doing it one by one is required in case of different off modes per channel

  for (unsigned char x = 0; x <= 7; x++) {
    disableChannel(x);
  }
}

void DAC7678::enableChannel(unsigned char channel) {
  // Enables the specific DAC output channel
  
  if (channel < 8) {

    unsigned int x = (unsigned int) (0x20 << channel);  // Channels are offset +5 bits
    unsigned char msdb = (unsigned char)(x >> 8);
    unsigned char lsdb  = x & 0xFF;

    transmit(CMD_POWER_DOWN, msdb, lsdb);
  }
}

void DAC7678::disableChannel(unsigned char channel) {
  // Disables the specific DAC output channel
  
  if (channel < 8) {

    unsigned int x = (unsigned int) (0x20 << channel);  // Channels are offset +5 bits
    unsigned char msdb = (unsigned char)(x >> 8);
    msdb |= off_mode[channel];
    unsigned char lsdb  = x & 0xFF;

    transmit(CMD_POWER_DOWN, msdb, lsdb);
  }
}

void DAC7678::set(int _value) { 
  // Sets all DAC channels to specified value 
  
  for (unsigned char x = 0; x <= 7; x++) {
    set(x, _value);
  }
}

void DAC7678::set(unsigned char _channel, int _value) {
  // Set specified channel (0-7) to the specified value
  
  //   Check for out of range values
  if (_value >= 4096 || _value < 0) {
    return;
  }


  if (_channel < 8) { // Check for out of range Channel #
    // Sets the variables
    unsigned char _command = CMD_IS_LDAC_BASE_ADDR + _channel;

    unsigned int x = (unsigned int) (_value << 4);  // Data is offset +4 bits
    unsigned char msdb = (unsigned char)(x >> 8);
    unsigned char lsdb  = x & 0xFF;

	// Send data to DAC
    transmit(_command, msdb, lsdb);
  }
}


void DAC7678::update(unsigned char _channel, int _value) {
  // Update specified channel (0-7) to the specified value

  
  //   Check for out of range values
  if (_value >= 4096 || _value < 0) {
    return;
  }

  if (_channel < 8) { // Check for out of range Channel #
    // Sets the variables
    unsigned char _command = CMD_WRITE_BASE_ADDR + _channel;

    unsigned int x = (unsigned int) (_value << 4);  // Data is offset +4 bits
    unsigned char msdb = (unsigned char)(x >> 8);
    unsigned char lsdb  = x & 0xFF;

	// Send data to DAC
    transmit(_command, msdb, lsdb);
  }
}

void DAC7678::clrMode(int _value) {
// Configures the DAC value to output on all channels when CLR pin is brought low
// Will set the CLR Code register to output either zero, half-range, full range or to ignore the CLR pin

  unsigned char lsdb = _value << 4;

	// Send data to DAC
  transmit(CMD_CLEAR_CODE, 0x00, lsdb);
}

unsigned int DAC7678::readChan(unsigned char _command) {

  unsigned int reading = 0;

  Wire.beginTransmission(dac7678_address); 
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

  Wire.beginTransmission(dac7678_address);
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

// DEPRECATED
void DAC7678::init() {
  begin();
}

void DAC7678::enable(unsigned char _state) {
  if (_state) {
    enable();
  }
  else {
    disable();
  }
}

void DAC7678::enable(unsigned char _channel, unsigned char _state) {
  if(_state) {
    enableChannel(_channel);
  }
  else {
    disableChannel(_channel);
  }
}
