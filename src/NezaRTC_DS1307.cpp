#include "NezaRTC_DS1307.h"

uint8_t NezaRTC_DS1307::bcd2bin(uint8_t v) { return v - 6 * (v >> 4); }
uint8_t NezaRTC_DS1307::bin2bcd(uint8_t v) { return v + 6 * (v / 10); }

bool NezaRTC_DS1307::begin(TwoWire *wire) {
  _i2c.setBus(wire);
  return _i2c.begin();
}

bool NezaRTC_DS1307::writeReg(uint8_t r, uint8_t v) {
  uint8_t b[2] = {r, v};
  return _i2c.write(b, 2);
}

uint8_t NezaRTC_DS1307::readReg(uint8_t r) {
  uint8_t v = 0;
  _i2c.write_then_read(&r, 1, &v, 1);
  return v;
}

bool NezaRTC_DS1307::adjust(const NezaDateTime &dt) {
  if (!dt.isValid()) return false;
  uint8_t yy = dt.year() % 100;
  uint8_t centuryIndex = (dt.year() - 2000) / 100;
  uint8_t b[8] = {0x00, bin2bcd(dt.second() & 0x7F), bin2bcd(dt.minute()),
                  bin2bcd(dt.hour()), bin2bcd(dt.dayOfWeek() + 1),
                  bin2bcd(dt.day()), bin2bcd(dt.month()), bin2bcd(yy)};
  bool ok = _i2c.write(b, 8);
  ok &= writeReg(MARK_REG, MARK);
  ok &= writeReg(CENT_REG, centuryIndex);
  return ok;
}

NezaDateTime NezaRTC_DS1307::now() {
  uint8_t r = 0;
  uint8_t b[7];
  if (!_i2c.write_then_read(&r, 1, b, 7)) return NezaDateTime();
  uint16_t base = 2000;
  if (readReg(MARK_REG) == MARK) base = 2000 + (uint16_t)readReg(CENT_REG) * 100;
  return NezaDateTime(base + bcd2bin(b[6]), bcd2bin(b[5]), bcd2bin(b[4]),
                      bcd2bin(b[2] & 0x3F), bcd2bin(b[1]), bcd2bin(b[0] & 0x7F));
}

bool NezaRTC_DS1307::isRunning() {
  uint8_t r = 0, s = 0;
  if (!_i2c.write_then_read(&r, 1, &s, 1)) return false;
  return !(s & 0x80);
}
