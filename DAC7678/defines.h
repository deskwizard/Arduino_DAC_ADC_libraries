#ifndef DAC7678_DEFINES_H
#define DAC7678_DEFINES_H

#define ENABLE 1
#define DISABLE 0

// VREF options (Flexible Mode)
#define EXT 0 // External VREF
#define INT 1 // Internal VREF

// DAC power mode
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

#define CMD_WRITE_BASE_ADDR     0x00
#define CMD_SELECT              0x10
#define CMD_IS_LDAC_BASE_ADDR   0x30
#define CMD_POWER_DOWN          0x40
#define CMD_CLEAR_CODE          0x50
#define CMD_LDAC                0x60
#define CMD_SOFT_RESET          0x70
#define CMD_INTREF_RS           0x80

#endif
