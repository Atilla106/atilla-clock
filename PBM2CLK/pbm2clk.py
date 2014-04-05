#!/usr/bin/python

from sys import argv, stdout

def chunks(data):
  return [data[i:i+8] for i in xrange(0, len(data), 8)]

def bits2byte(bits):
  return sum(int(bits[i]) << i for i in xrange(len(bits)))

if __name__ == '__main__':
  with open(argv[1]) as f:
    type_ = int(argv[2])

    assert 0 <= type_ <= 1

    frame_height = 8 if type_ == 0 else 16

    lines = f.read().splitlines()

    w, h = [int(x) for x in lines[2].split()]

    assert w == 48
    assert h % frame_height == 0

    number_of_frames = h / frame_height
    total_time = int(argv[3])

    assert number_of_frames < 2 ** 7 - 1 # 7 bits for the number of frames, enough
    assert total_time <= 2 ** 16 - 1 # total_time < ~65 sec
    
    output = []
    output.append(type_ + (number_of_frames << 1))
    output.append(total_time >> 8)
    output.append(total_time & 255)

    data = reduce(lambda x, y: x + y, lines[3:])
    output += map(bits2byte, chunks(data))
    
    stdout.write(''.join(map(chr, output)))