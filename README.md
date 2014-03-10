mocam
=====

The intention of this project is to interact and stream frames from
video devices and do facial analysis on them.

Requirements
============

A C++11 compliant compiler (GCC 4.7+, Clang 3.2+ etc.), CMake 2.8+,
and OpenCV library 2.4+.

snapshot
========

This utility is used to snap an image from a video device.

1. Extract source and go into the diretory.
2. mkdir build
3. cd build
4. cmake ..
5. make

Now you can run the program: ./bin/snapshot

```
Usage: ./bin/snapshot (options) <output filename>
Options:
  --help | h            Shows this message.
  --device | -d <id>    The device to take a snapshot from.
  --list | -l           List all available video devices on the system.
```
