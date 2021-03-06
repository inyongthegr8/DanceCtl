// Simple example application that shows how to read four Arduino
// digital pins and map them to the USB Joystick library.
//
// Ground digital pins 2-9 to press the joystick 
// buttons 0, 1, 2, 3, 4, 5, 6 and 7 respectively.
//
// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// For Doubles Use by In Yong Lee
// 2021-01-08
// Orig by Matthew Heironimus
// 2015-11-20
// Edits for Ltek Dance pad: 
// Todd Oakes
// 2020-06-14
//--------------------------------------------------------------------

#include <Joystick.h>

Joystick_ Joystick;
#define DEBOUNCE_DELAY 5
#define NBUTTONS 8
static const int buttonPins[NBUTTONS] = {2,3,4,5,6,7,8,9};
static unsigned long lastButtonState[NBUTTONS]; 

void setup() {
  
  //pinMode(LED_BUILTIN, OUTPUT); //whats this do? from DanceCtl
  // Initialize Button Pins 4-11
  for (int i = 0; i < NBUTTONS; i++)
    pinMode(buttonPins[i], INPUT_PULLUP);
  
  // Initialize Joystick Library
  Joystick.begin();

  // Last state of the button
  
  for (int i = 0; i < NBUTTONS; i++)
  { 
   lastButtonState[i] = 0;  
  }
  
}

// Constant that maps the phyical pin to the joystick button.
//const int pinToButtonMap = 4;


void loop() {
  unsigned long now = millis();
  static unsigned long lastButtonChange[NBUTTONS];
  
  // Read pin values
  for (int index = 0; index < NBUTTONS; index++)
  {
    int currentButtonState = !digitalRead(buttonPins[index]);
    if (currentButtonState != lastButtonState[index] && (lastButtonChange[index] + DEBOUNCE_DELAY) < now)
    {
      Joystick.setButton(index, currentButtonState);
      lastButtonState[index] = currentButtonState;
      lastButtonChange[index] = now;
    }
  }

  //delay(50); //maybe change to DEBOUNCE_DELAY if it's still needed.
}
