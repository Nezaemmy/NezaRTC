#include <NezaRTC.h>

NezaRTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);

  if (!rtc.begin()) {
    Serial.println("DS3231 not found");
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    // When time needs to be set on a new device, or after a power loss.
    // May 18, 2026 at 2pm you would call:
    //rtc.adjust(NezaDateTime(2026, 5, 18, 14, 0, 0));
  }
  // DS3231 hardware supports 2000-2199 in this library using the century bit.
  // When time needs to be re-set on a previously configured device
  // May 18, 2026 at 2pm you would call:
  bool ok = rtc.adjust(NezaDateTime(2026, 5, 18, 14, 0, 0));
  if (!ok) Serial.println("Date rejected by DS3231 supported hardware range");
}

void loop() {
  NezaDateTime now = rtc.now();

  Serial.print(now.year()); 
  Serial.print('/');
  Serial.print(now.month()); 
  Serial.print('/');
  Serial.print(now.day()); 
  Serial.print(' ');
  Serial.print(now.dayOfTheWeekName());
  Serial.print(' ');
  Serial.print(now.hour()); 
  Serial.print(':');
  Serial.print(now.minute()); 
  Serial.print(':');
  Serial.println(now.second());

  delay(1000);
}
