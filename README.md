# Ardu-Switcher
A footswitcher to midi controller arduino program

## Why

I built a little device that takes input from jack connected footswitches and pedals and sends it as midi messages over usb.

It is based on the ardunio Leonardo for its native usb functionality!

## Components
Component list:

* Arduino Leonardo (since it is capable as running as a usb midi device)
* TRS connectors (3)
* Two way switch
* USB 2.0 type B connector
* Micro USB 2.0 cable with cut opened
* Wire

## Setup
To interface with device there is a USB type B connector is connected to the loose wires of the micro usb cable. The micro usb connector is connected to the Arduino.

From the Arduino, ground is running to all the TRS jack sleeves in series.

The TRS connectors are connected to digital pins 1 -> 6 on the Arduino, with the TRS tips connected at 1, 3 and 5 and the TRS rings connected at ping 2, 4 and 6. Analog in A0 is also connected to TRS 3 ring.

The switch is connecting digital pin 7 to 5v when in the "off" position, and 5v to TRS 3 tip (same as digital pin 6).

The TRS jacks are used to plug in external switches, allowing up to 6 footswitches connected at once, or 4 switches and 1 expression pedal.

## Building a switch-box
A connected switch-box would be built by simply connecting ground(sleeve) to the input of the two footswitches and the other (ring and tip) to the output of the footswitches(Could be any switch connecting/disconnecting two pins).

## What external switches and pedals can i use?
I have tested this with a 2 switch enclosure(the one i used was made by Vox and had momentary switches) and a Roland DP-10 sustain pedal(Sleeve -> ground, ring -> analog-in, tip -> 5v).
