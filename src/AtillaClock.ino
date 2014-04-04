#include <SD.h>
#include <Wire.h>

#include <ht1632c.h>
#include <font.h>

#include <RTClib.h>

#include <transition.h>

#define NUM_DISPLAYS 2
#define HT1632C_BRIGHTNESS 15
#define HT1632C_DATA 9
#define HT1632C_WRCLK 8

static uint8_t HT1632C_CS[NUM_DISPLAYS] = { 5, 6 };

HT1632C ht1632c = HT1632C(HT1632C_WRCLK, HT1632C_DATA, HT1632C_CS, NUM_DISPLAYS);
Font font = Font(ht1632c);

struct Clock {
  byte hour, minute;
};

RTC_DS1307 ds1307;
Clock clock;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Wire.begin();

  randomSeed(analogRead(1));

  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);

  /* LED MATRICES */
  ht1632c.setup(HT1632C_BRIGHTNESS);

  /* RTC MODULE */
  ds1307.begin(); //start RTC Clock

  if (!ds1307.isrunning()) {
    Serial.println(F("RTC is NOT running!"));
    ds1307.adjust(DateTime(__DATE__, __TIME__));
  }

  DateTime now = ds1307.now();
  clock.hour = now.hour();
  clock.minute = now.minute();

  /* SD CARD */

  pinMode(10, OUTPUT);

  if (!SD.begin(4)) {
    Serial.println(F("SD Card cannot be initialized!"));
    return; // We're fucked up
  }

  font.print_large_clock_number(11, 0, clock.hour);
  font.print_large_colon(23, 0);
  font.print_large_clock_number(27, 0, clock.minute);

  Serial.println("YOLOL");
}

void loop() {
  DateTime now = ds1307.now();

  if (now.minute() != clock.minute) {
    clock.minute = now.minute();
    font.print_large_clock_number(27, 0, clock.minute);
    Transition::transition("TEST.CLK", ht1632c);
  }

  if (now.hour() != clock.hour) {
    clock.hour = now.hour();
    font.print_large_clock_number(11, 0, clock.hour);
  }
}
