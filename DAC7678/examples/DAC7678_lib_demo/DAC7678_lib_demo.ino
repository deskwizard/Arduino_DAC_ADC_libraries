/*
 Example sketch for the TI DAC7678 12bits i2C DAC Library
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


#include <Wire.h>                     // Include Wire.h for I2C functionality
#include <DAC7678.h>                  // Include DAC7678.h for TI DAC7678 functions

DAC7678 dac(0x4C);                    // Set the DAC address to 0x4C or 76 or 0b01001100 (all valid notations of the same address) 
// See Figure 10, Datasheet page 28 for addresses

void setup()
{
  Serial.begin(9600);                               // Start Serial 
  Serial.println("Sketch begin");                   // Print welcome message
  Serial.println("Waiting for loop() to start");    // Print welcome message
  Serial.println("Please be patient...");           // Print welcome message
  Serial.println();                                 // Print blank line

  dac.init();                         // initialize the DAC
  dac.setVREF(INT);                   // Configure VREF mode (Not really needed for External as it defaults to External)
  dac.clrMode(ZERO);                  // All Channels will reset to 0 if CLR pin is brought low

  dac.set(2000);                   // Set all powered on channels to 2000/4095
  dac.set(PIN_A, 140);            // Sets channel 0 to 140/4095

  delay(5000);                        // wait a little
  dac.offMode(L100K);                 // Sets power off mode for all pins 100K pulldown (Will override previous specific channel settings)
  dac.offMode(PIN_A, HIGHZ);          // Sets power off mode for channel 0 to HIGHZ
  dac.enable(PIN_A, PWOFF);             // Power down Channel A
  dac.set(1000);                   // Set all powered on channels to 2000/4095
  delay(5000);                        // wait a little

  dac.set(500);                    // Set all powered on channels to 500/4095
  dac.set(PIN_A, 1500);           // Set Channel F to 1500/4095
  dac.enable(PIN_A, PWON);              // Power up DAC Channel F
  delay(5000);                        // wait a little
}

void loop()
{
  int set_value = 0;                  // Sets/reset the variable for setting DAC to 0
  unsigned int read_value = 0;        // Sets/reset the variable for reading DAC to 0
  unsigned int vref = 5000;           // Set vref value for calculations (set to 5000 for internal reference)
  float voltage = 0;                  // Sets/reset the variable for voltage caltulations to 0

  dac.enable(PWOFF);                 // Power down all DAC channels 
  delay(5000);                        // Wait 5 seconds

  dac.enable(PWON);                  // Power up all DAC Channels
  
  for (int x = 0; x < 10; x++){
    set_value = set_value + 400;

    dac.set(set_value);                // write the value to all DAC channels
    dac.set(5, set_value/2);          // write half the value to DAC channel 5
    dac.set(PIN_H, set_value/4);      // write a quarter of the value to DAC channel 7

    read_value = dac.readDAC(PIN_A);          // Read value of DAC channel 0
    voltage = read_value * (vref / 4095.0);   // Calculate voltage output according to the voltage reference

    Serial.print("Channel ");             // Prints...
    Serial.print(PIN_A);                  // Prints pin #
    Serial.print(" set to: ");            // Prints...
    Serial.print(read_value);
    Serial.print(" (");
    Serial.print(voltage);                // Prints value of DAC output voltage on channel 0 to serial monitor
    Serial.println(" mV)");               // Prints...
    delay(2000);                          // Wait 1 second
  }
  Serial.println("Loop end");
  Serial.println();

}

















