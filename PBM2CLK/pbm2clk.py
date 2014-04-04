#!/usr/bin/python

from sys import argv, stdout

def chunks(data):
  return [data[i:i+8] for i in xrange(0, len(data), 8)]

def bits2byte(bits):
  return sum(int(bits[i]) << i for i in xrange(len(bits)))

if __name__ == '__main__':
  with open(argv[1]) as f:
    lines = f.read().splitlines()

    w, h = [int(x) for x in lines[2].split()]

    assert w == 48 
    assert h % 8 == 0

    number_of_lines = h / 8
    total_time = int(argv[2])

    assert total_time < 2 ** 16 - 1 # total_time < ~65 sec
    
    output = []
    output.append(number_of_lines)
    output.append(total_time >> 8)
    output.append(total_time & 255)

    data = reduce(lambda x, y: x + y, lines[3:])
    output += map(bits2byte, chunks(data))

    stdout.write(''.join(map(chr, output)))