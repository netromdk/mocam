mocam
=====

The intention of this project is to interact and stream frames from
video devices and do facial analysis on them.

Requirements
============

A C++11 compliant compiler (GCC 4.7+, Clang 3.2+ etc.), CMake 2.8.8+,
Qt 5.2+ and OpenCV 2.4+.

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
Usage:  ./bin/snapshot  (<options>) <output filename>

The format of the snapshot is dictated by the extension of <output filename>.
Supported extensions: jpg, jpeg, and png.

Options:
  --help | -h          Shows this message.
  --list | -l          List all available video devices on the system.
  --device | -d <str>  The device to take a snapshot from.
  --quality | -q <n>   Quality of the snapshot in percentage (1-100).
  --scale | -s <str>   Scales to the defined size given 'WidthxHeight'
                       in pixels. Or scale keeping the size-ratio by
                       'Xw' or 'Xh', where 'X' is the size to scale to
                       in width or height respectively.
```
