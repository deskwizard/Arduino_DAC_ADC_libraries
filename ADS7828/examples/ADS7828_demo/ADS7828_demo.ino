/*
 Example sketch for the TI ADS7828 12bits i2C ADC Library
 Datasheet: http://www.ti.com/lit/ds/symlink/ads7828.pdf

 Deskwizard (03/16/2013)

 ------------------------------------------------------------------------------
 Library information -  Current Library version: 0.dc - March 16th 2013
 Tested working on IDE version 1.0.3

 This is a basic library to use the ADS7828 ADC.
 
 Both single ended and differential modes are currently available.
 Please read the information below if you plan to use differential mode.
 
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
 
 CH0 - CH7 are obviously the ADC input channels (Optional?)
 
 ------------------------------------------------------------------------------    
 Function list
 
  init(INT)*                  Initialize the ADC and the I2C bus with internal voltage reference. (* Use one or the other)
  init()*                     Initialize the ADC and the I2C bus with external voltage reference. (* Use one or the other)
  read(channel, SD|DF)        Read the specified ADC channel (0-7) in either single ended (SD) or differential (DF) mode.
                              Reading a channel will return a value between 0-4095
                              
 ------------------------------------------------------------------------------    
 Using differential mode
 
 When using differential mode, it is important to pay attention to the channel naming.
 The channel configuration is contained in Table II on page 11 of the datasheet.
 
 It is important to respect the polarity of the channels when reading in differential mode
 or your readings will be 0.
 
 For example:
 
  Differential Channel 0:      Positive side on Channel 0, Negative side on Channel 1
  Differential Channel 1:      Positive side on Channel 1, Negative side on Channel 0
  Differential Channel 2:      Positive side on Channel 2, Negative side on Channel 3
  Differential Channel 3:      Positive side on Channel 3, Negative side on Channel 2
  etc...
*/


#include <Wire.h>                     // Include Wire.h for I2C functionality
#include <ADS7828.h>                  // Include ADS7828.h for TI ADS7828 functions

ADS7828 adc(0x49);                    // Set the ADS7828 i2c address to 0x49 (A0 connected to ground, A1 connected to 5v)


void setup()
{
  Serial.begin(9600);                 // Start Serial 
  Serial.println("Sketch begin");     // Print begin message
  adc.init();                         // Initialize ADC in external reference mode and initialize the i2c bus
}

void loop()
{
  
  unsigned int read_value = 0;        // Sets/reset the variable for reading DAC to 0
  unsigned int vref = 4095;           // Sets vref value for calculations (set to 2500 for internal reference)
  float voltage = 0;                  // Sets/reset the variable for voltage caltulations to 0
  
  for(int x = 0; x < 8; x++){                     // Loops from 0 to 7
    
    read_value = adc.read(x, SD);                 // Read value of ADC channel x (0 to 7) in Single-ended mode
    voltage = read_value * (vref / 4095.0);       // Calculate voltage output value according to the voltage reference and resolution
    
    Serial.print("Channel ");                 // Prints...
    Serial.print(x);                          // Prints channel number
    Serial.print(": ");                       // Prints...
    Serial.print(read_value);                 // Prints value read from ADC
    Serial.print(" (");                       // Prints...
    Serial.print(voltage,0);                  // Prints value of DAC output voltage on channel 0 to serial monitor
    Serial.println(" mV)");                   // Prints...
  }
  
  Serial.println();                      // Prints blank line
  read_value = adc.read(0, DF);          // read channel 0 in differential mode (Chan 0 +, Chan 1 -)
  Serial.println(read_value);            // Prints value read from ADC
  Serial.println();                      // Prints blank line
  delay(1000);                           // Wait for 1 second
}



