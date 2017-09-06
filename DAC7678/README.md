Arduino library for TI DAC7678 12bits i2c DAC

------------------------------------------------------------------------------    
  Channel naming 
  You can use either the number of the pin or the name mentionned here.
 
  Library  |  Chip Pin (Datasheet)
  0    =    PIN_A
  1    =    PIN_B
  ...
  7    =    PIN_H
 
------------------------------------------------------------------------------  
  Function list
 
 
  begin()                         Initialize the DAC

  reset()                      	  Send Reset command to DAC (Same as Power-On reset, called by begin() )

  setVREF(INT|EXT)             	  Sets reference mode to internal (INT) or external (EXT)   (Defaults to EXT) 

  clrMode(Clear_Mode);         	  Configure DAC value for clear pin (Active low)

  offMode(mode)     	          Configure Power down mode all channels
  offMode(channel, mode)     	  Configure Power down mode for specified channel

  LDAC(state)                     Configure LDAC setting for all channels
  LDAC(channel, state)            Configure LDAC setting for specified channel
 
  enable()                        Enable the all channels
  enable(channel)                 Enable the specific channel
  disable()                       Disable the all channels
  disable(channel)                Disable the specific channel

  set(value)    		          Set all channels to specified value (0 - 4095)
  set(channel, value)    		  Set specified channel to specified value (0 - 4095)

  update(channel, value)    	  Update Channel to specified value (0 - 4095)
                                  * Write right away if LDAC = 0, write at next set() call if LDAC = 1 *

  readChan(Channel)            	  Read the current value of the specified DAC channel
 

  *** Deprecated, kept for backwards compatibility ***

  init()                          Same as begin
  enable((channel,) PWON|PWOFF)   Replaced by separate enable/disable functions  

------------------------------------------------------------------------------    
  Power Off
 
  The Power off state of the channels have 3 possible states on the DAC7678 and are used as follows:
 
  HIGHZ  -  DAC channel(s) powered OFF, Pin set to High-Z mode (High Impedance) ** DEFAULT **
  L1K    -  DAC channel(s) powered OFF, Pin pulled down through 1K resistor
  L100K  -  DAC channel(s) powered OFF, Pin pulled down through 100K resistor 
 
------------------------------------------------------------------------------    
  Clear pin
 
  The settings available for the value sets on all channels when clear pin is brought low are as follows:
 
  FULL  -  Channels set to 4095  
  MID   -  Channels set to mid-point   
  ZERO  -  Channels are set to 0        **  DEFAULT **
  NOCLR -  Clear pin is ignored
 
------------------------------------------------------------------------------ 
 
