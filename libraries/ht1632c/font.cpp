#include "font.h"

Font::Font(HT1632C &ht1632c) : _ht1632c(ht1632c) { }

void Font::print_small_digit(uint8_t x, uint8_t y, uint8_t digit) {
  uint8_t i, j;

  if (digit > 9)
    return;

  for (i = 0; i < 5; ++i)
    for (j = 0; j < 3; ++j) {
      _ht1632c.plot(x + (2 - j), y + i, (SMALL_DIGITS[digit][i] >> j) & 1);
    }
}

void Font::print_large_digit(uint8_t x, uint8_t y, uint8_t digit) {
  uint8_t i, j;

  if (digit > 9)
    return;

  for (i = 0; i < 7; ++i)
    for (j = 0; j < 5; ++j)
      _ht1632c.plot(x + (4 - j), y + i, (LARGE_DIGITS[digit][i] >> j) & 1);
}

void Font::print_large_clock_number(uint8_t x, uint8_t y, uint8_t n) {
  if (n < 10)
    print_large_digit(x, y, 0);
  else
    print_large_digit(x, y, n / 10);

  print_large_digit(x + 6, y, n % 10);
}

void Font::print_large_colon(uint8_t x, uint8_t y) {
  _ht1632c.plot(x, y + 3, 1);
  _ht1632c.plot(x, y + 5, 1);
}

void Font::draw_cock(uint8_t x, uint8_t y, uint8_t size) {
  uint8_t i, j;

  for (i = 0; i < 6; ++i)
    for (j = 0; j < 5; ++j)
      _ht1632c.plot(x + (4 - j), y + i, (COCK[0][i] >> j) & 1);

  x += 5;

  for (i = 0; i < size; ++i) {
    _ht1632c.plot(x + i, y, 0);
    _ht1632c.plot(x + i, y + 1, 1);
    _ht1632c.plot(x + i, y + 2, 0);
    _ht1632c.plot(x + i, y + 3, 1);
    _ht1632c.plot(x + i, y + 4, 0);
    _ht1632c.plot(x + i, y + 5, 0);
  }

  x += size;

  for (i = 0; i < 6; ++i)
    for (j = 0; j < 4; ++j)
      _ht1632c.plot(x + (3 - j), y + i, (COCK[1][i] >> j) & 1);

  x += 4;

  for (i = 0; i < 6; ++i)
    _ht1632c.plot(x, y + i, 0);
}