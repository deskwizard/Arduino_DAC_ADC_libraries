#ifndef DAC7678_H
#define DAC7678_H

#include <Wire.h>



// VREF options (Flexible Mode)
#define EXT 0 // External VREF
#define INT 1 // Internal VREF
// DAC power modes
#define ON 0
#define OFF_100K 1
#define OFF_1K 2
#define OFF_HIGHZ 3
// Clear pin Modes
#define NOCLR 3
#define MID 1
// Library version 0.1b. See example sketch for information, or read down
// Deskwizard 03/10/2013

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
	void enableAll(unsigned char state);
	void enableChan(unsigned char channel, unsigned char state);
	void setAll(int _value);
	void setChan(unsigned char channel, int _value);
	void clrMode(int _value);
	unsigned char DAC;
	
	private:
	void transmit(unsigned char _command, unsigned char _msdb, unsigned char _lsdb);

};

#endif
