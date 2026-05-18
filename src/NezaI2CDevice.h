#ifndef NEZA_I2C_DEVICE_H
#define NEZA_I2C_DEVICE_H

#include <Arduino.h>
#include <Wire.h>

class NezaI2CDevice {
public:
  NezaI2CDevice(uint8_t addr = 0x68, TwoWire *wire = &Wire);
  void setBus(TwoWire *wire);
  bool begin();
  bool detected();
  bool write(const uint8_t *buffer, size_t len);
  bool write_then_read(const uint8_t *writeBuffer, size_t writeLen,
                       uint8_t *readBuffer, size_t readLen);

private:
  uint8_t _addr;
  TwoWire *_wire;
};

#endif
