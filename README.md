# Walldraw 

A fork of shihaipeng03/Walldraw indended to fix GCode, and also include hardware. This has 3D printable motor mounts and pulleys. I'm testing out gondala designs at the moment

This is designed to work on a Raspberry Pi Pico programmed via Arduino. It might work on other boards. I've tested it on a couple of Unos (one was a clone, the other I'm sure) and it didn't run. Not sure of the problem.

## Current state:
* Gcode over serial is working
* SD card untested. This is still to try. Not sure of SD card support on RP2040 via Arduino?

## GCode generation:
* I've had most success using https://github.com/arpruss/gcodeplot
* Pen up and pen down require preceise values for Z. Can't remember them at the moment -- update this.
* I've been using universal Gcode sender to send the GCode. Probably could just stream it over serial
* Note has a ten second 'start up' You need to plug in the plotter and connect to it via UGS within this time period otherwise UGS won't recognise it. I plan to use the button to fix this.

## TODO
* Might be possible to use microswitches and bobbles on the string to 'home' it.
* or at least add a 'jog' ability with buttons to let you home it manually.

## Issues
* You need the extra RP2040 Arduino thing installed -- not just the official Arduino one, but the other one as well because the official Arduino one doens't include Servo support. -- update this when I remember the details





