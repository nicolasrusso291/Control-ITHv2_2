#pragma once

#include <Adafruit_Sensor.h>
#include <Adafruit_AHT10.h>
#include <Wire.h>

#define AHT10_ADDRESS 0x38
#define SDA_PIN       4
#define SCL_PIN       5

void CONTROL_ITH(void);
void VAR_TH(void);
boolean i2c_device_check(uint8_t _addr);
void ERROR_PROBLEM(void);
