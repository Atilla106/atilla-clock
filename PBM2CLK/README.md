PBM2CLK
=======

Converts a .PBM file into a .CLK format, transition file readable by the clock.

CLK Format
----------

+ 1 Byte: 1 bit for the type (0 for 48x8px frames, 1 for 48x16px frames). 7 bits for the number of frames.
+ 2 Bytes: Total time of the transition.
+ Number of Frames * 48 Bytes (or 96 if frames are 16px high) : all the frames.

The 48 (or 96) Bytes are output as follows:

+ Line per line (48 pixels per line)
+ 6 Bytes per line, each bit read from right to left (e.g: 01100000 will output 2 red pixels at position 5 and 6, indiced from 0).

Usage
-----

    ./pbm2clk <pbm_file> <type> <total_time>
