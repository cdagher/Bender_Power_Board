/**
 * @file main.cpp
 * @author Chris Dagher (christopherdaghe@u.boisestate.edu)
 * @brief Control the power distribution board on Bender 3.14. This handles relay switching and voltage monitoring
 * @version 1.0
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include "board.h"

#ifndef __AVR_ATtinyX4__
  #error Only ATTINYX4 boards supported!
#endif

float B1_V;
float B2_V;
float B3_V;

enum battery {
  BAT1 = B1_ENABLE_PIN,
  BAT2 = B2_ENABLE_PIN,
  BAT3 = B3_ENABLE_PIN
};
battery enabledBat;

float measureBatteryVoltage(battery battery);
void engageBattery(battery battery);
void disengageBattery(battery battery);
void switchBattery(battery old_batt, battery new_batt);

void setup() {
  // set up batter anable pins and disengage all batteries
  pinMode(B1_ENABLE_PIN, OUTPUT);
  pinMode(B2_ENABLE_PIN, OUTPUT);
  pinMode(B3_ENABLE_PIN, OUTPUT);
  digitalWrite(BAT1, 0);
  digitalWrite(BAT2, 0);
  digitalWrite(BAT3, 0);
  delay(RELAY_RELEASE_TIME);

  // set up voltage measurement pins and measure battery voltages
  ADCSRA &= 0b01111111; // disable the ADC before power off
  PRR |= 0b00000001;    // turn the ADC off
  ADMUX |= 0b00000001;  // set ADC reference to external voltage on PA0
  PRR &= 0b11111110;    // turn the ADC back on
  ADCSRA |= 0b10000000; // re-enable the ADC after power on
  pinMode(V_SENSE_B1_PIN, INPUT);
  pinMode(V_SENSE_B2_PIN, INPUT);
  pinMode(V_SENSE_B3_PIN, INPUT);
  B1_V = measureBatteryVoltage(BAT1);
  B2_V = measureBatteryVoltage(BAT2);
  B3_V = measureBatteryVoltage(BAT3);

  // choose the first available battery, hang if none are available
  if (B1_V > LOW_VOLTAGE_THRESHOLD) {
    engageBattery(BAT1);
    enabledBat = BAT1;
  } else if (B2_V > LOW_VOLTAGE_THRESHOLD) {
    engageBattery(BAT2);
    enabledBat = BAT2;
  } else if (B3_V > LOW_VOLTAGE_THRESHOLD) {
    engageBattery(BAT3);
    enabledBat = BAT3;
  } else {
    while (1) ;
  }
}

void loop() {

  if (measureBatteryVoltage(enabledBat) <= LOW_VOLTAGE_THRESHOLD) {
    B1_V = measureBatteryVoltage(BAT1);
    B2_V = measureBatteryVoltage(BAT2);
    B3_V = measureBatteryVoltage(BAT3);

    // if all batteries are drained, power down the robot and hang (sorry downstream systems)
    if (B1_V <= LOW_VOLTAGE_THRESHOLD && B2_V <= LOW_VOLTAGE_THRESHOLD && B3_V <= LOW_VOLTAGE_THRESHOLD) {
      disengageBattery(enabledBat);
      while (1) ;
    }

    // switch to the battery with the highest voltage
    if (enabledBat == BAT1) {
      switchBattery(BAT1, (B2_V > B3_V) ? BAT2 : BAT3);
    } else if (enabledBat == BAT2) {
      switchBattery(BAT2, (B1_V > B3_V) ? BAT1 : BAT3);
    } else { // enabledBat = BAT3
      switchBattery(BAT3, (B1_V > B2_V) ? BAT1 : BAT2);
    }
  }

  delay(200);

}

float measureBatteryVoltage(battery battery) {
  int sensePin;
  switch (battery) {
    case BAT1: 
      sensePin = V_SENSE_B1_PIN;
      break;
    case BAT2:
      sensePin = V_SENSE_B2_PIN;
      break;
    case BAT3:
      sensePin = V_SENSE_B3_PIN;
      break;
    default:
      return 0.0;
  }
  
  return ((analogRead(sensePin) * V_SENSE_CONVERSION) * V_SENSE_CONSTANT);
}

void engageBattery(battery battery) {
  digitalWrite(battery, 1);
  delay(RELAY_OPERATE_TIME);
  analogWrite(battery, RELAY_DUTY_CYCLE);
}

void disengageBattery(battery battery) {
  digitalWrite(battery, 0);
  delay(RELAY_RELEASE_TIME);
}

void switchBattery(battery old_batt, battery new_batt) {
  disengageBattery(old_batt);
  engageBattery(new_batt);
}
