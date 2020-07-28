
//#include "MIDIUSB.h"
#include "midiMessage.h"

#define DEBUG

#define num_switches 6
#define ExP A0 // expression pedal on analog 0
#define isNEx 7 // on/off switch pin

int switches[num_switches] = {1, 2, 3, 4, 5, 6}; // switch pins, two per tele-jack

int switchStates[num_switches]; // store last read value of pins

/*
  Mapping switches to midi messages
*/
#define m_ch 1 // default midi channel
int default_types[] = {0, 0, 0, 0, 0, 0}; // default midi message types
int default_data1[] = {3, 9, 3, 3, 14, 15}; // notes or cc message
int default_data2[] = {1, 1, -1, -1, -1, -1}; // if negative, use digitalRead value


int lastPedalValue = 0; // have the value changed
int largestPedalValue = 0; // max value for expression pedal, used to map to a 0->127 value

/**/
void setup() {
  Serial.begin(9600);
  resetSwitchStates();

  setSwitchInputs();
  pinMode(isNEx, INPUT_PULLUP); // expression on/off switch
}

/**/
void resetSwitchStates() {
  for (int i = 0; i < num_switches; i++) switchStates[i] = 0;
}

/**/
void setSwitchInputs() {
  for (int i = 0; i < num_switches; i++) pinMode(switches[i], INPUT_PULLUP);
}


/*

*/
void loop() {
  for (int i = 0; i < num_switches; i++) {
    handleSwitch(i);

    if (i == num_switches -3 && digitalRead(isNEx) == 0) {
      handlePedal();
      break; // its set to use expression instead of last two switches
    }
  }

  delay(2);
}

/*

*/
bool handlePedal() {
  int value = mapPedalToByte(analogRead(ExP));

  #ifdef DEBUG
  Serial.println(value);
  #endif

  if (value != lastPedalValue) {
    sendMidi({0, m_ch, 11, value}); // send control change
    lastPedalValue = value;
  }
}

/*


*/
int mapPedalToByte(int value) {
  if (value > largestPedalValue) largestPedalValue = value; // set record high value to constrain
  value = constrain(value, 0, largestPedalValue); // constrain to highest value recorded
  value = map(value, 0, largestPedalValue, 0, 127);// map to byte
  return value;
}

/*


*/
bool handleSwitch(int switchnr, int state, int &lastState, MidiMessage msg) {
  if (state != lastState) {
    if (validMidiMessageType(msg.Type)) sendMidi(msg);
    lastState = state;
  }
}
