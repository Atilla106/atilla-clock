#include <transition.h>

Header::Header(File &file) {
  uint8_t tmp_time[2];

  _type_number_of_frames = file.read();

  tmp_time[0] = file.read();
  tmp_time[1] = file.read();

  _total_time = (uint16_t)tmp_time[0];
  _total_time <<= 8;
  _total_time += (uint16_t)tmp_time[1];

  _delay = (unsigned long)_total_time / (unsigned long)get_number_of_frames();
}

uint8_t Header::get_type() {
  return _type_number_of_frames & 1;
}

uint8_t Header::get_number_of_frames() {
  return _type_number_of_frames >> 1;
}

uint16_t Header::get_total_time() {
  return _total_time;
}

unsigned long Header::get_delay() {
  return _delay;
}

bool Transition::transition(const char * filepath, HT1632C &ht1632c) {
  File file = SD.open(filepath);

  if (!file)
    return false;

  /* HEADER */

  Header header = Header(file);
  uint8_t start_y = header.get_type() == TYPE_SMALL ? 8 : 0;

  /* DATA */

  uint8_t n = 0;
  uint8_t x = 0, y = 0, k = 0;
  uint8_t current_byte;

  unsigned long start_millis = 0;
  unsigned long elapsed = 0;

  while (n < header.get_number_of_frames()) {
    start_millis = millis();

    for (y = start_y; y < 16; ++y)
      for (x = 0; x < 48; x += 8) {
        current_byte = file.read();

        for (k = 0; k < 8; ++k, current_byte >>= 1)
          if (ht1632c.get_x_y(x + k, y) != (current_byte & 1))
            ht1632c.plot(x + k, y, current_byte & 1);
      }

    if (millis() < start_millis) { // There was an overflow
      elapsed = millis() + ((1 << 32) - 1) - start_millis; // Normalize
    }
    else
      elapsed = millis() - start_millis;

    if (elapsed < header.get_delay())
      delay(header.get_delay() - elapsed);

    ++n;
  }

  file.close();

  return header.get_type() == TYPE_BIG;
}