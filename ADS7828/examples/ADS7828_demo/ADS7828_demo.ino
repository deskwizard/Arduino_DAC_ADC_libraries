/*
 Example sketch for the TI ADS7828 12bits i2C ADC Library
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


#include <Wire.h>                     // Include Wire.h for I2C functionality
#include <ADS7828.h>                  // Include ADS7828.h for TI ADS7828 functions

ADS7828 adc(0x49);                    // Set the ADC address to 0x49 


void setup()
{
  Serial.begin(9600);                 // Start Serial 
  Serial.println("Sketch begin");     // Print begin message
  adc.init();                         // Initialize ADC in external reference mode and initialize the i2c bus
}

void loop()
{
  
  unsigned int read_value = 0;        // Sets/reset the variable for reading DAC to 0
  unsigned int vref = 4095;           // Set vref value for calculations (set to 2500 for internal reference)
  float voltage = 0;                  // Sets/reset the variable for voltage caltulations to 0
  
  
  
  for(int x = 0; x < 8; x++){                 // Loops from 0 to 7
    
    read_value = adc.read(x);                 // Read value of ADC channel x
    voltage = read_value * (vref / 4095.0);   // Calculate voltage output according to the voltage reference
    
    Serial.print("Channel ");                 // Prints...
    Serial.print(x);                          // Prints channel number
    Serial.print(": ");                       // Prints...
    Serial.print(read_value);                 // Prints value read from ADC
    Serial.print(" (");                       // Prints...
    Serial.print(voltage,0);                  // Prints value of DAC output voltage on channel 0 to serial monitor
    Serial.println(" mV)");                   // Prints...
  }
  
  Serial.println();                      // Prints blank line
  delay(1000);                           // Wait for 1 second
}



