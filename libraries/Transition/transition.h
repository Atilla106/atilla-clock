#include <Arduino.h>
#include <SD.h>

#include <ht1632c.h>

#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#define START_Y 8

class Header {
private:
  uint8_t _number_of_frames; /* 1 Byte for the number of frames */
  uint16_t _total_time; /* 2 Bytes for the total time of the transition */
  unsigned long _delay; /* Delay between 2 frames (unsigned long because Arduino's delay is unsigned long) */

public:
  Header(File &file);

  uint8_t get_number_of_frames();
  uint16_t get_total_time();
  unsigned long get_delay();
};

class Transition {
public:
  static bool transition(const char * filepath, HT1632C &ht1632c);
};

#endif