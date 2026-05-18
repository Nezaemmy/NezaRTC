#include "NezaDateTime.h"

static const uint8_t dimBase[] = {31, 28, 31, 30, 31, 30,
                                  31, 31, 30, 31, 30, 31};

static const char *daysOfTheWeekNames[] = {
    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

static bool leap(uint16_t y) {
  return (y % 4 == 0) && ((y % 100 != 0) || (y % 400 == 0));
}

static uint8_t dim(uint16_t y, uint8_t m) {
  if (m < 1 || m > 12) return 0;
  if (m == 2 && leap(y)) return 29;
  return dimBase[m - 1];
}

NezaDateTime::NezaDateTime()
    : _year(2000), _month(1), _day(1), _hour(0), _minute(0), _second(0) {}

NezaDateTime::NezaDateTime(uint16_t year, uint8_t month, uint8_t day,
                           uint8_t hour, uint8_t minute, uint8_t second)
    : _year(year), _month(month), _day(day), _hour(hour), _minute(minute),
      _second(second) {}

NezaDateTime::NezaDateTime(uint64_t unixTime) {
  uint64_t t = unixTime;
  _second = t % 60;
  t /= 60;
  _minute = t % 60;
  t /= 60;
  _hour = t % 24;
  uint64_t days = t / 24;

  _year = 1970;
  while (true) {
    uint16_t dy = leap(_year) ? 366 : 365;
    if (days < dy) break;
    days -= dy;
    _year++;
  }

  _month = 1;
  while (true) {
    uint8_t dm = dim(_year, _month);
    if (days < dm) break;
    days -= dm;
    _month++;
  }

  _day = days + 1;
}

uint16_t NezaDateTime::year() const { return _year; }
uint8_t NezaDateTime::month() const { return _month; }
uint8_t NezaDateTime::day() const { return _day; }
uint8_t NezaDateTime::hour() const { return _hour; }
uint8_t NezaDateTime::minute() const { return _minute; }
uint8_t NezaDateTime::second() const { return _second; }

bool NezaDateTime::isValid() const {
  return _year >= 1970 && _month >= 1 && _month <= 12 &&
         _day >= 1 && _day <= dim(_year, _month) &&
         _hour < 24 && _minute < 60 && _second < 60;
}

uint8_t NezaDateTime::dayOfWeek() const {
  uint16_t y = _year;
  uint8_t m = _month;
  uint8_t d = _day;

  if (m < 3) {
    m += 12;
    y--;
  }

  uint16_t k = y % 100;
  uint16_t j = y / 100;
  uint8_t h = (d + (13 * (m + 1)) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;

  return (h + 6) % 7;  // Sunday = 0, Monday = 1, ... Saturday = 6
}

uint8_t NezaDateTime::dayOfTheWeek() const {
  uint8_t dow = dayOfWeek();
  return (dow == 0) ? 7 : dow;  // Monday = 1, ... Sunday = 7
}

uint8_t NezaDateTime::daysOfTheWeek() const {
  return dayOfTheWeek();
}

const char *NezaDateTime::dayOfWeekName() const {
  return daysOfTheWeekNames[dayOfTheWeek() - 1];
}

const char *NezaDateTime::dayOfTheWeekName() const {
  return dayOfWeekName();
}

const char *NezaDateTime::daysOfTheWeekName() const {
  return dayOfWeekName();
}

uint64_t NezaDateTime::unixtime64() const {
  uint64_t days = 0;
  for (uint16_t y = 1970; y < _year; y++) days += leap(y) ? 366 : 365;
  for (uint8_t m = 1; m < _month; m++) days += dim(_year, m);
  days += _day - 1;
  return days * 86400ULL + (uint64_t)_hour * 3600ULL +
         (uint64_t)_minute * 60ULL + _second;
}

uint32_t NezaDateTime::unixtime() const {
  return (uint32_t)unixtime64();
}
