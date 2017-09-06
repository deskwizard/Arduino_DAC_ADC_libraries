#ifndef DAC7678_H
#define DAC7678_H

#include "Arduino.h"
#include <Wire.h>
#include "defines.h"

class DAC7678 {  

    public:
	    DAC7678(unsigned char _address);
	    void begin();
	    void reset();
	    void setVREF(bool _refstate);
        void LDAC(bool _state);
        void LDAC(unsigned char _channel, bool _state); 
	    void offMode(unsigned char channel, unsigned char mode);
	    void offMode(unsigned char mode);
	    void enable();
        void disable();
	    void enableChannel(unsigned char channel);
	    void disableChannel(unsigned char channel);
	    void set(int _value);
	    void set(unsigned char channel, int _value);
        void select(unsigned char _channel);
        void update(unsigned char _channel, int _value);
	    void clrMode(int _value);
	    unsigned char DAC;
	    unsigned int readChan(unsigned char _command);
	    unsigned int readDAC(unsigned char _command);
	
        // deprecated, kept for backwards compatibility
	    void init();
	    void enable(unsigned char state);
	    void enable(unsigned char channel, unsigned char state);

    private:
        int dac7678_address;
        unsigned char off_mode[8];
        unsigned char LDAC_reg = 0xFF;
	    void transmit(unsigned char _command, unsigned char _msdb, unsigned char _lsdb);

};

#endif
