
Atilla Clock
============

The Atilla team presents you here its handcrafted (well, almost) clock
! It's build upon an Arduino Uno with an additional network card and a
16*48 LED matrix.  We chose to code it in C instead of Python or any
other high-level language for after all, we're hackers.

Clock screen
------------

The idea is to output the time on the upper side of the clock (thus on
a 8*48 LED screen), and an animation on the lower side.

Every `x` minutes (can be parametrized), there is a transition on all
the upper side. This is exactly the same system with the animations at
the lower side.

Internals
---------

For the Arduino only allows us 2 kB of SRAM and 32 kB of flash memory,
we couldn't store all the transitions and animations at the same time.
We then chose to buffer from network the stuff we need.

In order to do that, we have a running Python server sending whatever
the clock asks, and choosing which transition or animation comes next
when demanded, all that over HTTP.

Scheduling and general algorithm
--------------------------------

This is pretty simple, and this applies (asynchronously) to both the
animations and the transitions. Let's call any of these a *run*.

We loop infinitely on this :

- We fetch some information packets from the server (for instance to
  change some constants)
- Then we receive an special information package telling us which
  *run* is getting to come (how many frames, the total running time,
  ...)
- Eventually we double-buffer the frames (which we shall talk about
  later), meaning that we fetch one, and we output one, always one
  frame ahead the stream.
  
Frame system
------------

Structure of a packet
---------------------
