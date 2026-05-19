# NezaRTC

Arduino RTC library for DS1307 and DS3231.

## Day name usage

You do not need to define your own `daysOfTheWeek` array in your `.ino` file.

Use:

```cpp
Serial.println(now.dayOfTheWeekName());
```

The built-in day names are:

```text
Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday
```

Number format:

```cpp
now.dayOfTheWeek();
```

```text
1 = Monday
2 = Tuesday
3 = Wednesday
4 = Thursday
5 = Friday
6 = Saturday
7 = Sunday
```

Examples are named:

```text
examples/DS3231/DS3231.ino
examples/D1307/D1307.ino
```
