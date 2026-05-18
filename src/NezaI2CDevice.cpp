#include "NezaI2CDevice.h"

NezaI2CDevice::NezaI2CDevice(uint8_t addr, TwoWire *wire)
    : _addr(addr), _wire(wire) {}

void NezaI2CDevice::setBus(TwoWire *wire) { _wire = wire; }

bool NezaI2CDevice::begin() {
  _wire->begin();
  return detected();
}

bool NezaI2CDevice::detected() {
  _wire->beginTransmission(_addr);
  return _wire->endTransmission() == 0;
}

bool NezaI2CDevice::write(const uint8_t *buffer, size_t len) {
  _wire->beginTransmission(_addr);
  for (size_t i = 0; i < len; i++) _wire->write(buffer[i]);
  return _wire->endTransmission() == 0;
}

bool NezaI2CDevice::write_then_read(const uint8_t *writeBuffer, size_t writeLen,
                                    uint8_t *readBuffer, size_t readLen) {
  _wire->beginTransmission(_addr);
  for (size_t i = 0; i < writeLen; i++) _wire->write(writeBuffer[i]);
  if (_wire->endTransmission(false) != 0) return false;

  size_t received = _wire->requestFrom((int)_addr, (int)readLen);
  if (received != readLen) return false;

  for (size_t i = 0; i < readLen; i++) readBuffer[i] = _wire->read();
  return true;
}
