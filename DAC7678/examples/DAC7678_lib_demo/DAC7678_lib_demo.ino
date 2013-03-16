/*
 Example sketch for the TI DAC7678 12bits i2C DAC Library
 Datasheet: http://www.ti.com/lit/ds/symlink/dac7678.pdf
 
 I tried to put as much comment and be clear as possible as I hate examples
 that lack any kind of information. The source code is also commented as much
 as I thought necessary/helpful.
 
 I hope it helps some people as I had quite a bit of fun with this one.
 
 Deskwizard (03/11/2013)
 
 ------------------------------------------------------------------------------
 Library information -  Current Library version: 0.1b - March 13th 2013
 Tested working on IDE versions 1.0.1, 1.0.2 and 1.0.3
  
 * Internal VREF modes are partially implemented. (Only static mode is implemented)
 * All power states are implemented.
 * Only writing to the device is currently implemented.
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
 
 init()                       Initialize the DAC (Initialize I2C bus and send Power-On Reset command)
 reset()                      Send Power-On Reset command to DAC
 enableAll(State )            Powers ON or OFF all the DAC Channels  **** See Power Off information ***
 enableChan(channel, ON|OFF)  Powers ON or OFF the specified DAC Channels **** See Power Off information ***
 setVREF(INT|EXT)             Sets reference mode to internal (INT) or external (EXT)   (Defaults to EXT) 
 clrMode(Clear_Mode);         Reset all DAC to "Clear_Mode" when CLR pin is brought low (Values: FULL, MID, ZERO, NOCLR)
 
 
 DAC use Functions
 
 setAll(value)                Set all DAC channels to specified value (Update + Write specific channel)
 setChan(channel, value)      Set specified DAC channel to specified value (Update + Write all channels)
 
 ------------------------------------------------------------------------------    
 Power Off information
 
 The Power off state of the channels have 4 possible states on the DAC7678 and are used as follows:
 
 ON         -  DAC channel(s) Powered ON
 OFF_HIGHZ  -  DAC channel(s) powered OFF, Pin set to High-Z mode (High Impedance)
 OFF_1K     -  DAC channel(s) powered OFF, Pin pulled down through 1K resistor
 OFF_10K    -  DAC channel(s) powered OFF, Pin pulled down through 100K resistor 
 
 
 ------------------------------------------------------------------------------    

 
 */


#include <Wire.h>                     // Include Wire.h for I2C functionality
#include <DAC7678.h>                  // Include DAC7678.h for TI DAC7678 functions

DAC7678 dac(0x4C);                    // Set the DAC address to 0x4C or 76 or 0b01001100 (all valid notations of the same address) 
                                      // See Figure 10, Datasheet page 28 for addresses

void setup()
{
  dac.init();                         // initialize the DAC
  dac.setVREF(EXT);                   // Configure VREF to External (Not really needed as it defaults to External)
  dac.clrMode(ZERO);                  // All Channels will reset to 0 if CLR pin is brought low
  dac.setAll(2000);                   // Set all powered on channels to 2000/4095
  delay(5000);                        // wait a little

  dac.enableChan(PIN_A, OFF_HIGHZ);   // Power down Channel F in High Impedance Mode
  dac.setAll(1000);                   // Set all powered on channels to 2000/4095
  delay(5000);                        // wait a little

  dac.setAll(500);                    // Set all powered on channels to 500/4095
  dac.setChan(PIN_A, 1500);           // Set Channel F to 1500/4095
  dac.enableChan(PIN_A, ON);          // Power up DAC Channel F
  delay(5000);                        // wait a little
}

void loop()
{
  int value = 0;                      // Sets/reset the variable to 0

  dac.enableAll(ON);                  // Power up all DAC Channels

  for (int x = 0; x < 20; x++){
    value = value + 200;
    dac.setAll(value);                // write the value to all DAC channels
    dac.setChan(5, value/2);          // write half the value to DAC channel 5
    dac.setChan(PIN_H, value/4);      // write a quarter of the value to DAC channel 7
    delay(1000);                      // Wait 1 second
  }

  dac.enableAll(OFF_100K);            // Power down all DAC channels in 100k pull-down mode
  delay(5000);                        // Wait 5 seconds
}









