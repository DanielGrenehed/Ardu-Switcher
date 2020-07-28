
#ifndef MIDIMESSAGE_H
#define MIDIMESSAGE_H
#include "Arduino.h" // to use in arduino programs
#include "MIDIUSB.h"

enum MidiMessageType {
  Control = 0, NoteOn = 1, NoteOff = 2
};

byte typeBytes[] = {0xB0, 0x90, 0x80}; // type of message in byte representation
byte channelBytes[] = {0x0B, 0x09, 0x08}; // byte for channel or operation

/*
  A simpler representation of a midi message,
*/
struct MidiMessage {
  MidiMessageType Type;
  byte Channel;
  byte Data_1;
  byte Data_2;
};

/*
  returns 1 if message type exists and is fully implemented
  returns 0 on fail.
*/
int validMidiMessageType(int msgType) {
  int valid = 0;

  switch(msgType) { // case for all fully implemented midi message types
    case MidiMessageType.Control:
    case MidiMessageType.NoteOn:
    case MidiMessageType.NoteOff:
      valid = 1;
  };

  return valid;
}

/*

  Convert MidiMessage to packet and sends midi

  Todo:
    add send to other midi destinations.
*/
void sendMidi(MidiMessage msg) {
  midiEventPacket_t packet = createMidiPacket(msg);
  sendMidiUsb(packet);
}

/*
  Converts a MidiMessage to a packet
*/
midiEventPacket_t createMidiPacket(MidiMessage msg) {
  midiEventPacket_t packet = {typeBytes[msg.Type], channelBytes[msg.Type] | msg.Channel, msg.Data_1, msg.Data_2};
  return packet
}

/*
  Send midi packet over usb.
*/
void sendMidiUsb(midiEventPacket_t packet) {
  MidiUSB.sendMIDI(packet);
  MidiUSB.flush();
}

#endif // MIDIMESSAGE_H
