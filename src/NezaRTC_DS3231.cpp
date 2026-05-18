#include "NezaRTC_DS3231.h"

uint8_t NezaRTC_DS3231::bcd2bin(uint8_t v) { return v - 6 * (v >> 4); }
uint8_t NezaRTC_DS3231::bin2bcd(uint8_t v) { return v + 6 * (v / 10); }

bool NezaRTC_DS3231::begin(TwoWire *wire) {
  _i2c.setBus(wire);
  return _i2c.begin();
}

uint8_t NezaRTC_DS3231::readReg(uint8_t r) {
  uint8_t v = 0;
  _i2c.write_then_read(&r, 1, &v, 1);
  return v;
}

bool NezaRTC_DS3231::writeReg(uint8_t r, uint8_t v) {
  uint8_t b[2] = {r, v};
  return _i2c.write(b, 2);
}

bool NezaRTC_DS3231::adjust(const NezaDateTime &dt) {
  if (!dt.isValid() || dt.year() < 2000 || dt.year() > 2199) return false;
  uint8_t yy = dt.year() % 100;
  uint8_t monthReg = bin2bcd(dt.month());
  if (dt.year() >= 2100) monthReg |= 0x80;

  uint8_t b[8] = {0x00, bin2bcd(dt.second()), bin2bcd(dt.minute()),
                  bin2bcd(dt.hour()), bin2bcd(dt.dayOfWeek() + 1),
                  bin2bcd(dt.day()), monthReg, bin2bcd(yy)};
  bool ok = _i2c.write(b, 8);
  uint8_t st = readReg(0x0F);
  st &= ~0x80;
  ok &= writeReg(0x0F, st);
  return ok;
}

NezaDateTime NezaRTC_DS3231::now() {
  uint8_t r = 0;
  uint8_t b[7];
  if (!_i2c.write_then_read(&r, 1, b, 7)) return NezaDateTime();
  uint16_t base = (b[5] & 0x80) ? 2100 : 2000;
  return NezaDateTime(base + bcd2bin(b[6]), bcd2bin(b[5] & 0x1F),
                      bcd2bin(b[4]), bcd2bin(b[2] & 0x3F),
                      bcd2bin(b[1]), bcd2bin(b[0] & 0x7F));
}

float NezaRTC_DS3231::temperature() {
  uint8_t r = 0x11, b[2];
  if (!_i2c.write_then_read(&r, 1, b, 2)) return 0.0f;
  return (int8_t)b[0] + ((b[1] >> 6) * 0.25f);
}

void NezaRTC_DS3231::setAlarm1(const NezaDateTime &dt) {
  uint8_t b[5] = {0x07, bin2bcd(dt.second()), bin2bcd(dt.minute()),
                  bin2bcd(dt.hour()), bin2bcd(dt.day())};
  _i2c.write(b, 5);
  uint8_t c = readReg(0x0E);
  c |= 0x05;
  writeReg(0x0E, c);
  clearAlarm1();
}

void NezaRTC_DS3231::setAlarm2(uint8_t minute, uint8_t hour) {
  uint8_t b[4] = {0x0B, bin2bcd(minute), bin2bcd(hour), 0x80};
  _i2c.write(b, 4);
  uint8_t c = readReg(0x0E);
  c |= 0x06;
  writeReg(0x0E, c);
  clearAlarm2();
}

bool NezaRTC_DS3231::alarm1Triggered() { return readReg(0x0F) & 0x01; }
bool NezaRTC_DS3231::alarm2Triggered() { return readReg(0x0F) & 0x02; }
bool NezaRTC_DS3231::alarmTriggered() { return readReg(0x0F) & 0x03; }

void NezaRTC_DS3231::clearAlarm1() { uint8_t s = readReg(0x0F); s &= ~0x01; writeReg(0x0F, s); }
void NezaRTC_DS3231::clearAlarm2() { uint8_t s = readReg(0x0F); s &= ~0x02; writeReg(0x0F, s); }
void NezaRTC_DS3231::clearAlarm()  { uint8_t s = readReg(0x0F); s &= ~0x03; writeReg(0x0F, s); }
void NezaRTC_DS3231::disableAlarms() { uint8_t c = readReg(0x0E); c &= ~0x03; writeReg(0x0E, c); clearAlarm(); }
bool NezaRTC_DS3231::lostPower() { return readReg(0x0F) & 0x80; }
