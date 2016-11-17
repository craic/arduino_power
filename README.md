# Arduino Power

** As of 2016/11/15 this is a work in progress - not quite ready for release **

Projects involving the [Arduino](https://www.arduino.cc) family of microcontroller boards typically don't
need any special power management circuitry. You connect a power source, the device boots,
does it's job and then stops when the power is disconnected. Unlike a Raspberry Pi system, there is
no risk of corrupting an SD memory card.

But for some applications you may want to monitor power status and respond in a more managed way, instead of just shutting down.


Consider a remote sensing device that logs temperature data, for example. If the battery runs out
while it is unattended the logs simply stop. There is no way to tell immediately that low
battery was the problem. It could have been software or hardware failure.

Likewise, for a portable system, it would be helpful to know how much battery life remains and when it needs
recharging.


This project presents several approaches to Arduino power management with circuits, software and descriptions for
each of them. Starting with a simple power-on/power-off switch all the way to a data logging system that includes battery status
which is intended for unattended environment monitoring.



The project is built on the ideas in the [LiPoPi](https://github.com/NeonHorizon/lipopi) and [Pi Power](https://github.com/craic/pi_power)
projects, which provides similar functionality for Raspberry Pi systems.

## Overview

The system provides several functions:

- Power On / Power Off using a push button
- Monitor battery voltage using the Arduino ADC
- Shutdown the system on low voltage
- Record the battery voltage and a shutdown message to file
- Display battery status using a red/green LED

I will introduce each of these in turn, but the common thread to all of them is combination of the Adafruit PowerBoost and the Arduino.


## Adafruit PowerBoost Shield

The system uses a rechargeable LiPoly Battery and an [Adafruit PowerBoost Charger Shield](https://www.adafruit.com/products/2078),
which can both charge the battery and boost its output to 5V.

[Adafruit](https://www.adafruit.com) makes three versions of the Power Boost - two breakout boards and an Arduino Shield.
Most of the work described here uses the Power Shield, but I'll talk about using the breakout boards later on.

**Please read [This Page](PowerBoostShield.md) on how to configure the PowerBoost Shield.**

**TLDR; to prepare the Shield kit you need to:**

1. Solder on the stacking header strips
2. But do not attach the switch
3. Solder on a length of wire (4 inches, 22AWG solid core) to the Enable Pin
4. Bridge the pads for Analog pin A0 on the underside of the shield



##Power On / Power Off circuit

This basic circuit uses the press of a momentary pushbutton switch to power up the system and, when running, to power it down.

When you press and hold on a powered down system, the PowerBoost is turned on and supplies power to the Arduino which then boots up
and runs the program that you have loaded onto it.

When you press the button again, and release it, the Arduino disables the PowerBoost and, because it is no longer supplying power,
the Arduino shuts down.

With the more complex versions of this, the system log a shutdown message, etc., before actually shutting down.

The PowerBoost has an **Enable pin** which is held High by default. In this state, the PowerBoost is enabled and supplies power
via the 5V pin on the Arduino headers.

If that pin is connected to ground then the PowerBoost is disabled and does not provide power.













This example program simply blinks the Arduino on board yellow LED which is attached to pin 13.

```arduino
// dummy text

```

## operation

The system has three components with hardware and software for each of them:

- Power on by pressing a pushbutton for a few seconds
- Power off by pressing and releasing the same pushbutton
- Automatic shutdown, with a log message, on a low battery signal


The software is intended to be incorporated in your Arduino programs. Simple examples that demonstrate
how it works are included here.





