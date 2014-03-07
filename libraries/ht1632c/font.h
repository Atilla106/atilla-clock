#include "ht1632c.h"

#ifndef __HT1632C_FONT_H__
#define __HT1632C_FONT_H__

static const byte SMALL_DIGITS[10][5] = {
  { 0b111, 0b101, 0b101, 0b101, 0b111 }, /* 0 */
  { 0b001, 0b001, 0b001, 0b001, 0b001 }, /* 1 */
  { 0b111, 0b001, 0b111, 0b100, 0b111 }, /* 2 */
  { 0b111, 0b001, 0b111, 0b001, 0b111 }, /* 3 */
  { 0b101, 0b101, 0b111, 0b001, 0b001 }, /* 4 */
  { 0b111, 0b100, 0b111, 0b001, 0b111 }, /* 5 */
  { 0b111, 0b100, 0b111, 0b101, 0b111 }, /* 6 */
  { 0b111, 0b001, 0b001, 0b001, 0b001 }, /* 7 */
  { 0b111, 0b101, 0b111, 0b101, 0b111 }, /* 8 */
  { 0b111, 0b101, 0b111, 0b001, 0b111 }  /* 9 */
};

static const byte LARGE_DIGITS[10][7] = {
  { 0b01110, 0b10001, 0b10011, 0b10101, 0b11001, 0b10001, 0b01110 }, /* 0 */
  { 0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110 }, /* 1 */
  { 0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111 }, /* 2 */
  { 0b01110, 0b10001, 0b00001, 0b00110, 0b00001, 0b10001, 0b01110 }, /* 3 */
  { 0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010 }, /* 4 */
  { 0b11111, 0b10000, 0b11110, 0b00001, 0b00001, 0b10001, 0b01110 }, /* 5 */
  { 0b01110, 0b10001, 0b10000, 0b11110, 0b10001, 0b10001, 0b01110 }, /* 6 */
  { 0b11111, 0b00001, 0b00010, 0b00100, 0b00100, 0b00100, 0b00100 }, /* 7 */
  { 0b01110, 0b10001, 0b10001, 0b01110, 0b10001, 0b10001, 0b01110 }, /* 8 */
  { 0b01110, 0b10001, 0b10001, 0b01111, 0b00001, 0b10001, 0b01110 }  /* 9 */
};

static const byte COCK[2][6] = {
  { 0b00000, 0b00011, 0b10000, 0b10100, 0b10101, 0b01110 },
  { 0b1110, 0b1001, 0b0011, 0b1001, 0b1110, 0b0000 }
};

class Font {
private:
  HT1632C &_ht1632c;

public:
  Font(HT1632C &ht1632c);

  void print_small_digit(uint8_t x, uint8_t y, uint8_t digit);

  void print_large_digit(uint8_t x, uint8_t y, uint8_t digit);
  void print_large_clock_number(uint8_t x, uint8_t y, uint8_t n);

  void print_large_colon(uint8_t x, uint8_t y);

  void draw_cock(uint8_t x, uint8_t y, uint8_t size);
  void clearScreen(uint8_t minx,uint8_t maxx,uint8_t miny,uint8_t maxy);
};

#endif
