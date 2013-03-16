// Library version 0.1e. See example sketch for information, or read down
// Deskwizard 03/16/2013
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
#define ON	1
#define OFF 0
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

};

#endif
