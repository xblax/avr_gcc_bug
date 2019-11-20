This example Arduino project for Atmel AVR highlighs a bug in avr-gcc 9.2.0 (presumably also present in older versions).

Depending on the optimization settings used, GCC merges progmem (.text) and data space (.data) symbols to a single progmem symbol. However, for AVR symbols from .text and .data may not be merged since they reside in independent memory spaces.

Due to this invalid optimization, if data is read from the symbol originally defined in data space, only arbitrary data is read (from data space at the address of the symbol in program space). See main.cpp for example code.

Build and flash example:

```
mkdir build && cd build
cmake ..
make build
make flash
```

Make sure to adapt the definitions in CMakeLists.txt to your Arduino platform (preconfigured for Arduino Nano). Connect to the serial monitor of your Arduino (baudrate 57600)

Expected output:

```
X_prog: 123
X: 123
X: 123
X: 123
...
```

Actual ouput (example):

```
X_prog: 123
X: -0.00
X: 0.00
X: 553676288.00
...
```

The example is compiled with GCC options `-Os -g -ffunction-sections -fdata-sections -flto -Wl,--gc-section`. I'm not entirely sure, but the bug appears to be triggered by link time optimization (-flto) in combination with optimization levels -Os or higher.

I found this bug while debugging a problem with my 3D printer firmware for Atmega 2560. I was wondering why my print head kept crashing into the side inexplicably.
