#include <SevSeg.h>
#include "Button.h"

/* button pins */
#define ADD_CLICK_BP 13
#define REMOVE_CLICK_BP 12

/* RGB pins */
#define RGB_RED 11
#define RGB_BLUE 10

/* seven segment pins */
#define SEG_A 6
#define SEG_B 7
#define SEG_C 9
#define SEG_D 2
#define SEG_E 3
#define SEG_F 5
#define SEG_G 4
#define SEG_DP 8

SevSeg sevseg;
Button addClickButton;
Button removeClickButton;

void setup() {

  /* setup RGB LED */
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  analogWrite(RGB_BLUE, 255); // corp goes first

  /* setup buttons */
  addClickButton.assignPin(ADD_CLICK_BP);
  removeClickButton.assignPin(REMOVE_CLICK_BP);

  /* setup the seven segment display */
  byte numDigits = 1;
  // no digit pins since this is a single 7-segment digit
  byte digitPins[] = {};
  byte segmentPins[] = {SEG_A, SEG_B, SEG_C,
                        SEG_D, SEG_E, SEG_F,
                        SEG_G, SEG_DP};
  bool resistorsOnSegments = true;
  byte hardwareConfig = COMMON_CATHODE;
  sevseg.begin(hardwareConfig, numDigits,
               digitPins, segmentPins,
               resistorsOnSegments);
  sevseg.setBrightness(90);
}

/* code for the seven segment display */

// figure 8 pattern through middle
int patternLength = 8;
int figureEightSegs[] = {SEG_A, SEG_B, SEG_G, SEG_E,
                         SEG_D, SEG_C, SEG_G, SEG_F};

int patternIndex = 0;
int delayCount = 15;
void updateSevSegFigureEight() {
  if (delayCount == 0) {
    digitalWrite(figureEightSegs[patternIndex], LOW);
    patternIndex = (patternIndex + 1) % patternLength;
    digitalWrite(figureEightSegs[patternIndex], HIGH);
    delayCount = 15;
  } else {
    delayCount--;
    delay(1);
  }
}

/* code for the RGB LED */

int oldTurnPin = RGB_BLUE;
int newTurnPin = RGB_RED;
int fadeStep = 1;
bool transitioningTurns = false;

// NOTE this is function is the master timer,
// since it has control over when the turn end
// sequence is finished.
void updateEndTurnPinFade() {
  analogWrite(oldTurnPin, 255 - fadeStep);
  analogWrite(newTurnPin, fadeStep);
  delay(5);
  fadeStep++;
  if (fadeStep == 256) {
    transitioningTurns = false;
    fadeStep = 1;
    if (oldTurnPin == RGB_BLUE) {
      oldTurnPin = RGB_RED;
      newTurnPin = RGB_BLUE;
    } else {
      oldTurnPin = RGB_BLUE;
      newTurnPin = RGB_RED;
    }
  }
}

/* master loop */
int turnsTaken = 1;
bool corpTurn = true;
int clicksLeft = 3;

void loop() {
  if (transitioningTurns) {
    updateEndTurnPinFade();
    updateSevSegFigureEight();
  } else {
    sevseg.setNumber(clicksLeft);
    sevseg.refreshDisplay();
    if (addClickButton.uniquePress()) {
      clicksLeft++;
    }
    if (removeClickButton.uniquePress()) {
      clicksLeft--;
      if (clicksLeft == 0) {
        transitioningTurns = true;
        clicksLeft = (corpTurn) ? 4: 3;
        corpTurn = !corpTurn;
        sevseg.blank();
      }
    }
  }
}
