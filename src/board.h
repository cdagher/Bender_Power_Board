/**
 * @file board.h
 * @author C Dagher (christopherdaghe@u.boisestate.edu)
 * @brief Pin mappings for the power distribution board on Bender 3.14
 * @version 1.0
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <pins_arduino.h>

#define B1_ENABLE_PIN PIN_PB1  // pin to enable the relay for battery 1
#define B2_ENABLE_PIN PIN_PB2  // pin to enable the relay for battery 2
#define B3_ENABLE_PIN PIN_PA7  // pin to enable the relay for battery 3
#define RELAY_DUTY_CYCLE 155   // duty cycle after relay has engaged

#define V_SENSE_B1_PIN PIN_PA3  // pin to sense the voltage of battery 1
#define V_SENSE_B2_PIN PIN_PA2  // pin to sense the voltage of battery 2
#define V_SENSE_B3_PIN PIN_PA1  // pin to sense the voltage of battery 3

#define V_SENSE_CONVERSION 0.0048875855327468
#define V_SENSE_CONSTANT 13 // determined from the battery voltage divider (120k --- 10k)
#define LOW_VOLTAGE_THRESHOLD 30

#define MOSI_PIN PIN_PA6
#define MISO_PIN PIN_PA5
#define SCK_PIN  PIN_PA4

#define CLKIN_PIN PIN_PB0  // external clock generator pin
#define AREF_PIN PIN_PA0   // analog reference pin