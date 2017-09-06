/*
  Example sketch for the TI DAC7678 12bits I²C DAC Library
  Demonstrate sequential writes to the DAC and reads back DAC values

  ------------------------------------------------------------------------------

  Chip Wiring

  Connect GND pin to Ground
  Connect AVDD pin to 5v
  Connect ADDR0 to ground
  Connect LDAC pin to ground
  Connect SDA pin to Arduino SDA (Analog 4 on Uno)
  Connect SCL pin to Arduino SCL (Analog 5 on Uno)
  Connect CLR pin to 5v

*/

#include <DAC7678.h>                  // Include DAC7678.h for TI DAC7678 functions

DAC7678 dac(0x48);                    // Set the DAC address to 0x48 (72 in decimal)

unsigned int vref = 5000;             // Voltage reference value for calculations (set to 5000 for internal reference)
unsigned int read_value = 0;          // Value read from DAC
float voltage = 0;                    // Read voltage


void setup() {

  Serial.begin(9600);
  Serial.println("Sketch started");
  Serial.println();

  dac.begin();                        // Initialize the DAC
  dac.setVREF(INT);                   // Configure for internal voltage reference

  delay(2000);                        // Wait 2 seconds
}


void loop() {

  int set_value = 0;

  dac.enable();                    // Power up all DAC Channels

  for (int x = 0; x < 10; x++) {

    set_value = set_value + 400;

    dac.set(set_value);                       // Write the value to all DAC channels
    dac.set(1, set_value / 2);                // Write half the value to DAC channel 1


    read_value = dac.readDAC(0);              // Read value of DAC channel 0
    voltage = read_value * (vref / 4095.0);   // Calculate voltage output according to the voltage reference

    Serial.print("Channel 0 set to ");
    Serial.print((voltage / 1000),3);         // Prints the calculated voltage
    Serial.println(" V");


    read_value = dac.readDAC(1);              // Read value of DAC channel 0
    voltage = read_value * (vref / 4095.0);   // Calculate voltage output according to the voltage reference

    Serial.print("Channel 1 set to ");
    Serial.print((voltage / 1000),3);         // Prints the calculated voltage
    Serial.println(" V");
    Serial.println();

    delay(2000);                              // Wait 2 seconds
  }


  Serial.println("Loop end. Powering off for 5 seconds.");
  Serial.println();

  dac.disable();                  // Power down all DAC channels
  delay(5000);

}
