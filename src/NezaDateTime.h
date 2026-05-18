#ifndef NEZA_DATETIME_H
#define NEZA_DATETIME_H

#include <Arduino.h>

class NezaDateTime {
public:
  NezaDateTime();
  NezaDateTime(uint16_t year, uint8_t month, uint8_t day,
               uint8_t hour = 0, uint8_t minute = 0, uint8_t second = 0);
  explicit NezaDateTime(uint64_t unixTime);

  uint16_t year() const;
  uint8_t month() const;
  uint8_t day() const;
  uint8_t hour() const;
  uint8_t minute() const;
  uint8_t second() const;

  bool isValid() const;

  uint8_t dayOfWeek() const;
  uint8_t dayOfTheWeek() const;
  uint8_t daysOfTheWeek() const;

  const char *dayOfWeekName() const;
  const char *dayOfTheWeekName() const;
  const char *daysOfTheWeekName() const;

  uint32_t unixtime() const;
  uint64_t unixtime64() const;

private:
  uint16_t _year;
  uint8_t _month;
  uint8_t _day;
  uint8_t _hour;
  uint8_t _minute;
  uint8_t _second;
};

#endif
