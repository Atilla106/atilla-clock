#include <Wire.h>

#include <ht1632c.h>
#include <font.h>
#include <affichage.h>
#include <RTClib.h>

#define NUM_DISPLAYS 2
#define HT1632C_BRIGHTNESS 15
#define HT1632C_DATA 10
#define HT1632C_WRCLK 11

static uint8_t HT1632C_CS[NUM_DISPLAYS] = { 4, 5 };

HT1632C ht1632c = HT1632C(HT1632C_WRCLK, HT1632C_DATA, HT1632C_CS, NUM_DISPLAYS);
Font font = Font(ht1632c);

struct Clock {
  byte hour, minute, day, year, month;
};

RTC_DS1307 ds1307;
Clock clock;

struct Cock {
  uint8_t size;
  uint8_t min_size, max_size;
  bool increase;
};
int i=0;
boolean penis=false;
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
  clock.year = now.year()%100;
  clock.day = now.day();
  clock.month = now.month();

  font.print_large_clock_number(11, 0, clock.hour);
  font.print_large_colon(23, 0);
  font.print_large_clock_number(27, 0, clock.minute);
  
}

void loop() {
  DateTime now = ds1307.now();
  if (now.minute() != clock.minute) {
    clock.minute = now.minute();
    font.print_large_clock_number(27, 0, clock.minute);
    Serial.println(i);
    i++;
    font.clearScreen(0,48,8,16);
  }

  if (now.hour() != clock.hour) {
    clock.hour = now.hour();
    font.print_large_clock_number(11, 0, clock.hour);
  }      
  if(i>3)
    i=0;
   switch(i){
      case 0:
         penis = false;
         if(now.day() != clock.day)
           clock.day = now.day();
         font.print_large_clock_number(2, 8, clock.day);
         font.print_small_letters(13,8,26);
         if(now.month() != clock.month)
            clock.month = now.month();
          font.print_large_clock_number(18, 8, clock.month);
          font.print_small_letters(29,8,26);
         
         if(now.year() % 100 != clock.year)
             clock.year = now.year() % 100;
         font.print_large_clock_number(34, 8, clock.year);
         break;
      case 1:
        penis=true;
        break;
      case 2:
        penis=false;
        font.print_large_clock_number(11,8,13);
        font.print_large_clock_number(24,8,37);
        break;
      case 3:
        font.print_small_letters(10,9,'a');
        font.print_small_letters(15,9,'t');
        font.print_small_letters(20,9,'i');
        font.print_small_letters(25,9,'l');
        font.print_small_letters(30,9,'l');
        font.print_small_letters(35,9,'a');
        break;
      default:
        penis=false;
        break;
    }
  if(penis){
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
  
}
