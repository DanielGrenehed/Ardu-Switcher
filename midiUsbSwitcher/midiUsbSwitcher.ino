
//#include "MIDIUSB.h"
#include "midiMessage.h"

#define DEBUG

/*

  Todo: move midi message creation to when it should be sent, to not have a kind of
  prototype message floating around the whole loop



  Move all switch handling to the handleSwitch function, removing the need to pass
  midi message, digital read value and switch state. Then maby subdevide that function into
  smaller functions

*/


#define num_switches 6
#define ExP A0 // expression pedal on analog 0
#define isNEx 7 // on/off switch pin

int switches[num_switches] = {1, 2, 3, 4, 5, 6} // switch pins, two per tele-jack

int switchStates[num_switches]; // store last read value of pins

int lastPedalValue = 0; // have the value changed
int largestPedalValue = 0; // max value for expression pedal, used to map to a 0->127 value


// the setup routine runs once when you press reset:
void setup() {

  Serial.begin(9600);


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

  MidiMessage msg = {0, 1, 3, 1}; // {midi_message_type, midi_channel, data_1, data_2 }
  int temp_switch_value = digitalRead(switches[0]);

  handleSwitch(switches[0], temp_switch_value, switchStates[0], msg);

  temp_switch_value = digitalRead(switches[1];
  msg.Data_1 = 9; // what is 9?
  handleSwitch(switches[1], temp_switch_value, switchStates[1], msg);

  temp_switch_value = digitalRead(switches[2]);
  msg.Data_1 = 3; // what is 3?
  msg.Data_2 = temp_switch_value; // what is temp_switch_value? 0 or 1, 0 -> 127?
  handleSwitch(switches[2], temp_switch_value, switchStates[2], msg);

  temp_switch_value = digitalRead(switches[3]);
  msg.Data_2 = temp_switch_value;
  handleSwitch(switches[3], temp_switch_value, switchStates[3], msg);

  temp_switch_value = digitalRead(isNEx); // switch in pedal on/off
  /*
    if switch is off, then handle input 5 and 6 as a expression pedal
    if switch is on, then handle inpu 5 and 6 as seperate switches
  */


  if (temp_switch_value == 0) { // make more explicit code and clearer variable names, temp_switch_value is overused
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
  int value = mapPedalToByte(analogRead(ExP));

  #ifdef DEBUG
  Serial.println(value);
  #endif

  if (value != lastPedalValue) {
    sendMidi({0, 1, 11, value}); // send control change
    lastPedalValue = value;
  }
}

int mapPedalToByte(int value) {
  if (value > largestPedalValue) largestPedalValue = value; // set record high value to constrain
  value = constrain(value, 0, largestPedalValue); // constrain to highest value recorded
  value = map(value, 0, largestPedalValue, 0, 127);// map to byte
}

/*


*/
bool handleSwitch(int switchnr, int state, int &lastState, MidiMessage msg) {
  if (state != lastState) {
    if (validMidiMessageType(msg.Type)) {
      sendMidi(msg);
    }

    lastState = state;
  }
}
