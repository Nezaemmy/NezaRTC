#ifndef NEZA_RTC_DS3231_H
#define NEZA_RTC_DS3231_H

#include <Arduino.h>
#include <Wire.h>
#include "NezaDateTime.h"
#include "NezaI2CDevice.h"

class NezaRTC_DS3231 {
public:
  bool begin(TwoWire *wire = &Wire);
  bool adjust(const NezaDateTime &dt);
  NezaDateTime now();
  float temperature();
  void setAlarm1(const NezaDateTime &dt);
  void setAlarm2(uint8_t minute, uint8_t hour);
  bool alarm1Triggered();
  bool alarm2Triggered();
  bool alarmTriggered();
  void clearAlarm1();
  void clearAlarm2();
  void clearAlarm();
  void disableAlarms();
  bool lostPower();

private:
  NezaI2CDevice _i2c = NezaI2CDevice(0x68);
  uint8_t bcd2bin(uint8_t v);
  uint8_t bin2bcd(uint8_t v);
  uint8_t readReg(uint8_t r);
  bool writeReg(uint8_t r, uint8_t v);
};

#endif
