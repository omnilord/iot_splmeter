#include "SevSegShift.h"

/* macros in ALL CAPS are defined in config.h */
#include "config.h"

SevSegShift sevseg(SHIFT_PIN_DS, SHIFT_PIN_SHCP, SHIFT_PIN_STCP);

int prevNum = -1;

void setupDisplay() {
  byte digitPins[] = DIGIT_PINS;
  byte segmentPins[] = SEGMENT_PINS;
  
  sevseg.begin(CATHODE_OR_ANODE, DIGITS, digitPins, segmentPins, RESISTORS_ON_SEGMENTS, UPDATE_DELAY, LEADING_ZEROS, NO_DEC_POINT);

  Serial.println("display ready");
}

void displayNumber(int num) {
  if (num != prevNum) {
    prevNum = num;
    sevseg.setNumber(num);
  }
  sevseg.refreshDisplay();
}
