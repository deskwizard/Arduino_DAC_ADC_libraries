## Arduino library for TI DAC7678 12bits i2c DAC

------------------------------------------------------------------------------    
####  Channel naming
#####  You can use either the number of the pin or the name mentionned here.


 | Library        | DAC Channel
| ------------- |:-------------:|
| 0      | PIN_A
| 1     |  PIN_B
| ... | 
|7| PIN_H


----------


####  Function list
 `* denotes default value`
 
	begin()                        Initialize the DAC

	reset()                        Send Reset command to DAC 
				       (Same as Power-On reset, called by begin() )

	setVREF(INT|EXT)               Vref Mode (internal (INT) or external (EXT*)

	clrMode(Clear_Mode)            Configure clear pin

	offMode(mode)                  Configure Power down (All channels)

	offMode(channel, mode)         Configure Power down (Single channel)

	LDAC(state)                    Configure LDAC (All channels)
	LDAC(channel, state)           Configure LDAC (Single channel)
 
	enable()                       Enable all channels
	enable(channel)                Enable the single channel

	disable()                      Disable all channels
	disable(channel)               Disable single channel

	set(value)    		       Set all channels
	set(channel, value)    	       Set specified channel

	update(channel, value)         Update Channel 
				       * See LDAC section below *

	readChan(Channel)              Read DAC channel
   


#####  *** Deprecated, kept for backwards compatibility ***

	init()                         Same as begin
	enable((channel,) PWON|PWOFF)  Replaced by enable/disable


----------


  
####  Power Off
 
The Power off state of the channels have 3 possible states on the DAC7678 and are used as follows:
 
	HIGHZ 	 -  DAC channel(s) powered OFF, High-Z mode (High Impedance) ** DEFAULT **
	L1K    -  DAC channel(s) powered OFF, 1K pulldown
	L100K  -  DAC channel(s) powered OFF, 100K resistor 
 


----------


####  Clear pin
 
The settings available for the value sets on all channels when clear pin is brought low are as follows:
 
	FULL  -  Channels set to 4095  
	MID   -  Channels set to mid-point   
	ZERO  -  Channels are set to 0        **  DEFAULT **
	NOCLR -  Clear pin is ignored


----------
#### LDAC function
To use the LDAC function, connect the LDAC pin to VCC.

When enabled for a pin, use the `update()` function to update the DAC register without writing to the output. To write to the, the output, use `set()` for the last pin to be written. 

If LDAC is not enabled, `update()` act exactly as `set()`

See the DAC7678_sync example.
