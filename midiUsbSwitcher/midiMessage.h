
#ifndef MIDIMESSAGE_H
#define MIDIMESSAGE_H
#include "Arduino.h"

enum MidiMessageType {
  Control = 0, NoteOn = 1, NoteOff = 2
};

struct MidiMessage {
  MidiMessageType Type;
  byte Channel;
  byte Data_1;
  byte Data_2;
};

#endif // MIDIMESSAGE_H
