#include "ht1632c.h"

HT1632C::HT1632C(uint8_t pinWR, uint8_t pinDATA, uint8_t pinCS[], uint8_t numDisplays) {
  _pinWR = pinWR;
  _pinDATA = pinDATA;

  for (uint8_t i = 0; i < numDisplays; ++i)
    _pinCS[i] = pinCS[i];

  _numDisplays = numDisplays;

  _shadowRAM = (uint8_t*)malloc(96 * _numDisplays * sizeof(uint8_t));

  for (uint8_t i = 0; i < 96 * _numDisplays; ++i)
    _shadowRAM[i] = 0;
}

void HT1632C::setup(uint8_t brightness) {
  for (uint8_t disp = 0; disp < _numDisplays; ++disp) {
    pinMode(_pinCS[disp], OUTPUT);

    digitalWrite(_pinCS[disp], HIGH);

    pinMode(_pinWR, OUTPUT);
    pinMode(_pinDATA, OUTPUT);
    
    _send_cmd(disp, HT1632_CMD_SYSON);
    _send_cmd(disp, HT1632_CMD_LEDON);
    _send_cmd(disp, HT1632_CMD_COMS01);

    for (uint8_t i = 0; i < 128; ++i)
      _send_data(disp, i, 0);
  }

  _send_cmd(0, HT1632_CMD_PWM + brightness);
  _send_cmd(1, HT1632_CMD_PWM + brightness);
}

void HT1632C::clear() {
  uint8_t i;

  for (uint8_t disp = 0; disp < _numDisplays; ++disp) {
    _chip_select(_pinCS[disp]);

    _write_bits(HT1632_ID_WR, 1 << 2);
    _write_bits(0, 1 << 6);

    for (i = 0; i < 96 / 2; ++i)
      _write_bits(0, 1 << 7);

    _chip_free(_pinCS[disp]);

    for (i = 0; i < 96; ++i)
      _shadowRAM[96 * disp + i] = 0;
  }
}

void HT1632C::plot(uint8_t x, uint8_t y, bool value) {
  uint8_t addr, bitval;
  uint8_t d;

  d = x / 24;
  x %= 24;

  // Magic happens

  bitval = 8 >> (y & 3);
  addr = (x << 2) + (y >> 2);

  if (value)
    _shadowRAM[d * 96  + addr] |= bitval;
  else
    _shadowRAM[d * 96 + addr] &= ~bitval;
  
  _send_data(d, addr, _shadowRAM[d * 96 + addr]);
}

void HT1632C::_chip_select(uint8_t n) {
  digitalWrite(n, 0);
}

void HT1632C::_chip_free(uint8_t n) {
  digitalWrite(n, 1);
}

void HT1632C::_write_bits(uint8_t bits, uint8_t firstBit) {
  while (firstBit) {
    digitalWrite(_pinWR, LOW);

    if (bits & firstBit)
      digitalWrite(_pinDATA, HIGH);
    else
      digitalWrite(_pinDATA, LOW);

    digitalWrite(_pinWR, HIGH);

    firstBit >>= 1;
  }
}

void HT1632C::_send_cmd(uint8_t disp, uint8_t command) {
  _chip_select(_pinCS[disp]);

  _write_bits(HT1632_ID_CMD, 1 << 2);
  _write_bits(command, 1 << 7);
  _write_bits(0, 1);

  _chip_free(_pinCS[disp]);
}

void HT1632C::_send_data(uint8_t disp, uint8_t address, uint8_t data) {
  _chip_select(_pinCS[disp]);

  _write_bits(HT1632_ID_WR, 1 << 2);
  _write_bits(address, 1 << 6);
  _write_bits(data, 1 << 3);

  _chip_free(_pinCS[disp]);
}