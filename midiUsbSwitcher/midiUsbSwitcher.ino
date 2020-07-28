
#include "MIDIUSB.h"
#include "midiMessage.h"

#define DEBUG

/*
#define switch_1 1
#define switch_2 2
#define switch_3 3
#define switch_4 4
#define switch_5 5
#define switch_6 5*/
#define num_switches 6

int switches[num_switches] = {1, 2, 3, 4, 5, 5} // switch pins, two per tele-jack

#define isNEx 7 // on/off switch pin

#define ExP A0 // expression pedal on analog 0

/*
int lastSwitchState_1 = 0; // last switch state for switch 1
int lastSwitchState_2 = 0;
int lastSwitchState_3 = 0;
int lastSwitchState_4 = 0;
int lastSwitchState_5 = 0;
int lastSwitchState_6 = 0;*/
int switchStates[num_switches]; // store last read value of pins

int lastPedalValue = 0;
int largestPedalValue = 0;

//int pedalValue = 0;
//int tempAnalogValue = 0;



// the setup routine runs once when you press reset:
void setup() {

  Serial.begin(9600);

  // setup switches as inputs
  /*
  pinMode(switch_1, INPUT_PULLUP);
  pinMode(switch_2, INPUT_PULLUP);
  pinMode(switch_3, INPUT_PULLUP);
  pinMode(switch_4, INPUT_PULLUP);
  pinMode(switch_5, INPUT_PULLUP);
  pinMode(switch_6, INPUT_PULLUP);
  */
  resetSwitchStates();
  setSwitchInputs();

  // expression on/off switch
  pinMode(isNEx, INPUT_PULLUP);

}

void resetSwitchStates() {
  for (int i = 0; i < num_switches; i++) {
    switchStates[i] = 0;
  }
}

void setSwitchInputs() {
  for (int i = 0; i < num_switches; i++) {
    pinMode(switches[i], INPUT_PULLUP);
  }
}


// fix loop please
void loop() {

  /*

  should be as simple as:

  for (int i = 0; i < num_switches; i++) {
    handleSwitch(i)
  }


  */

  MidiMessage msg = {0, 1, 3, 1};
  int temp_switch_value = digitalRead(switches[0]);

  handleSwitch(switches[0], temp_switch_value, switchStates[0], msg);

  temp_switch_value = digitalRead(switches[1];
  msg.Data_1 = 9;
  handleSwitch(switches[1], temp_switch_value, switchStates[1], msg);

  temp_switch_value = digitalRead(switches[2]);
  msg.Data_1 = 3;
  msg.Data_2 = temp_switch_value;
  handleSwitch(switches[2], temp_switch_value, switchStates[2], msg);

  temp_switch_value = digitalRead(switches[3]);
  msg.Data_2 = temp_switch_value;
  handleSwitch(switches[3], temp_switch_value, switchStates[3], msg);

  temp_switch_value = digitalRead(isNEx); // switch in pedal on/off
  /*
    if switch is off, then handle input 5 and 6 as a expression pedal
    if switch is on, then handle inpu 5 and 6 as seperate switches
  */


  if (temp_switch_value == 0) {
    handlePedal();
  } else {
    temp_switch_value = digitalRead(switches[4]);
    msg.Data_1 = 14;
    msg.Data_2 = temp_switch_value;
    handleSwitch(switches[4], temp_switch_value, switchStates[4], msg);

    temp_switch_value = digitalRead(switches[5]);
    msg.Data_1 = 15;
    msg.Data_2 = temp_switch_value;
    handleSwitch(switches[5], temp_switch_value, switchStates[5], msg);
  }

  delay(2);
}

/*


  Todo: Generalize function!
*/
bool handlePedal() {
  //pedalValue = 0;
  int tempAnalogValue = analogRead(ExP);

  if (tempAnalogValue > largestPedalValue) {
    largestPedalValue = tempAnalogValue;
  }

  #ifdef DEBUG
  Serial.println(tempAnalogValue);
  #endif

  tempAnalogValue = constrain(tempAnalogValue, 0, largestPedalValue);
  tempAnalogValue = map(tempAnalogValue, 0, largestPedalValue, 0, 127);
  //tempAnalogValue = map(tempAnalogValue, 0, 1023, 0, 127);
  //tempAnalogValue = constrain(tempAnalogValue, 0, 127);

  if (tempAnalogValue != lastPedalValue) {
    controlChange(1, 11, tempAnalogValue);
    MidiUSB.flush();
    lastPedalValue = tempAnalogValue;
  }

}



/*


  Todo: fix switchnr
*/
bool handleSwitch(int switchnr, int state, int &lastState, MidiMessage msg) {
  if (state != lastState) {

    switch (msg.Type) { // see midiMessage.h for types
      case (0):
      controlChange(msg.Channel, msg.Data_1, msg.Data_2);
      break;

      case (1):
      noteOn(msg.Channel, msg.Data_1, msg.Data_2);
      break;

      case (2):
      noteOff(msg.Channel, msg.Data_1, msg.Data_2);
      break;
    }

    MidiUSB.flush();

    lastState = state;
    return true;
  }
  return false;
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
