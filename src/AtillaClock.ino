#include <Wire.h>

#include <ht1632c.h>
#include <font.h>

#include <RTClib.h>

#define NUM_DISPLAYS 2
#define HT1632C_BRIGHTNESS 15
#define HT1632C_DATA 10
#define HT1632C_WRCLK 11

static uint8_t HT1632C_CS[NUM_DISPLAYS] = { 4, 5 };

HT1632C ht1632c = HT1632C(HT1632C_WRCLK, HT1632C_DATA, HT1632C_CS, NUM_DISPLAYS);
Font font = Font(ht1632c);

struct Clock {
  byte hour, minute;
};

RTC_DS1307 ds1307;
Clock clock;

struct Cock {
  uint8_t size;
  uint8_t min_size, max_size;
  bool increase;
};

Cock cock = { 7, 7, 35, true };

void setup() {
  Serial.begin(9600);
  Wire.begin();

  randomSeed(analogRead(1));

  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);

  ht1632c.setup(HT1632C_BRIGHTNESS);

  ds1307.begin(); //start RTC Clock

  if (!ds1307.isrunning()) {
    Serial.println("RTC is NOT running!");
    ds1307.adjust(DateTime(__DATE__, __TIME__));
  }

  DateTime now = ds1307.now();
  clock.hour = now.hour();
  clock.minute = now.minute();

  font.print_large_clock_number(11, 0, clock.hour);
  font.print_large_colon(23, 0);
  font.print_large_clock_number(27, 0, clock.minute);
}

void loop() {
  DateTime now = ds1307.now();

  if (now.minute() != clock.minute) {
    clock.minute = now.minute();
    font.print_large_clock_number(27, 0, clock.minute);
  }

  if (now.hour() != clock.hour) {
    clock.hour = now.hour();
    font.print_large_clock_number(11, 0, clock.hour);
  }

  if (cock.increase) {
    if (cock.size == cock.max_size)
      cock.increase = false;
    else
      cock.size++;
  }
  else {
    if (cock.size == cock.min_size)
      cock.increase = true;
    else
      cock.size--;
  }

  font.draw_cock(1, 9, cock.size);
}
