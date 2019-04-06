
#include "MIDIUSB.h"

#define b1 1
#define b2 2
#define b3 3
#define b4 4
#define b5 5
#define b6 5
#define isNEx 7

#define ExP A0

int lb1 = 0;
int lb2 = 0;
int lb3 = 0;
int lb4 = 0;
int lb5 = 0;
int lb6 = 0;

int sVal = 0;
int lVal = 0;
int tempA = 0;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(b4, INPUT_PULLUP);
  pinMode(b5, INPUT_PULLUP);
  pinMode(b6, INPUT_PULLUP);
  pinMode(isNEx, INPUT_PULLUP);
  //pinMode(led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  // print out the state of the button:

  int tps = digitalRead(b1);
  if (tps != lb1) {
    Serial.println("Switch 1 tapped");
    controlChange(1, 3, 1);
    MidiUSB.flush();
    lb1 = tps;
  }
  tps = digitalRead(b2);
  if (tps != lb2) {
    Serial.println("Switch 2 tapped");
    controlChange(1, 9, 1);
    MidiUSB.flush();
    lb2 = tps;
  }
  tps = digitalRead(b3);
  if (tps != lb3) {
    Serial.println("Switch 3 tapped");
    controlChange(1, 3, tps);
    MidiUSB.flush();
    lb3 = tps;
  }
  tps = digitalRead(b4);
  if (tps != lb4) {
    Serial.println("Switch 4 tapped");
    controlChange(1, 9, tps);
    MidiUSB.flush();
    lb4 = tps;
  }
  
  tps = digitalRead(isNEx);
  //4Serial.println(tps);

  if (tps == 0) {
    sVal = 0;
    tempA = analogRead(ExP);
    Serial.println(tempA);
    tempA = map(tempA, 0, 1023, 0, 127);
    tempA = constrain(tempA, 0, 127);
    if (tempA != lVal) {
      controlChange(1, 11, tempA);
      MidiUSB.flush();
      lVal = tempA;
    }
  } else {
    tps = digitalRead(b5);
    if (tps != lb5) {
      Serial.println("Switch 5 tapped");
      controlChange(1, 14, tps);
      MidiUSB.flush();
      lb5 = tps;
  }
    tps = digitalRead(b6);
    if (tps != lb6) {
      Serial.println("Switch 6 tapped");
      controlChange(1, 15, tps);
      MidiUSB.flush();
      lb6 = tps;
  }
  }
  
  
  
  delay(2);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
