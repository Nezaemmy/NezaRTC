#ifndef NEZA_RTC_DS1307_H
#define NEZA_RTC_DS1307_H

#include <Arduino.h>
#include <Wire.h>
#include "NezaDateTime.h"
#include "NezaI2CDevice.h"

class NezaRTC_DS1307 {
public:
  bool begin(TwoWire *wire = &Wire);
  bool adjust(const NezaDateTime &dt);
  NezaDateTime now();
  bool isRunning();

private:
  static const uint8_t MARK_REG = 0x08;
  static const uint8_t CENT_REG = 0x09;
  static const uint8_t MARK = 0x4E;
  NezaI2CDevice _i2c = NezaI2CDevice(0x68);
  uint8_t bcd2bin(uint8_t v);
  uint8_t bin2bcd(uint8_t v);
  bool writeReg(uint8_t r, uint8_t v);
  uint8_t readReg(uint8_t r);
};

#endif
