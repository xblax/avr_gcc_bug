#include "Arduino.h"

static const PROGMEM float xyz_prog[] = { 123, 123, 123 };
float xyz[] = { 123, 123, 123 };

volatile int x = 0;

void setup() {
    Serial.begin(57600);
    Serial.print("X_prog: ");
    Serial.println(pgm_read_float_near(&xyz_prog[0]));
}

void loop() {
    Serial.print("X: ");
    Serial.println(xyz[x]);
}
