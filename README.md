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

With each of these you add and customize two functions, *arduinoPowerSetup() and arduinoPowerMonitor()*, to the standard *setup() and loop()*
functions in your code.

## Adafruit PowerBoost Shield

The system uses a rechargeable LiPoly Battery and an [Adafruit PowerBoost Charger Shield](https://www.adafruit.com/products/2078),
which can both charge the battery and boost its output to 5V.

[Adafruit](https://www.adafruit.com) makes three versions of the Power Boost - two breakout boards and an Arduino Shield.
Most of the work described here uses the Power Shield, but I'll talk about using the breakout boards later on.

**Please read [This Page](PowerBoostShield.md) on how to configure the PowerBoost Shield.**

**TL;DR to prepare the Shield kit you need to:**

1. Solder on the stacking header strips
2. But do not attach the switch
3. Solder on a length of wire (4 inches, 22AWG solid core) to the Enable Pin
4. Bridge the pads for Analog pin A0 on the underside of the shield



##Power On / Power Off Circuit

This basic circuit uses the press of a momentary pushbutton switch to power up the system and, when running, to power it down.

When you press and hold on a powered down system, the PowerBoost is turned on and supplies power to the Arduino which then boots up
and runs the program that you have loaded onto it.

When you press the button again, and release it, the Arduino disables the PowerBoost and, because it is no longer supplying power,
the Arduino shuts down.

**Please read [This Page](PowerOnPowerOff.md) for the details.**

**TL;DR  &nbsp; Here is the schematic :**

![Power On / Power Off Circuit](/images/power_on_power_off_schematic.png)




##Monitor Battery Voltage

The Arduino has 6 **Analog to Digital Converters** (ADCs) on analog pins 0-5, which makes voltage measurement
straightforward. The PowerBoost shield allows you to connect the Battery voltage (**Vbat**) to one of these by
bridging the appropriate solder pads as described on the [PowerBoost Shield page](PowerBoostShield.md). In this
example I have linked it to **Analog pin 0**.

The voltage of LiPoly batteries varies from around **4.25V** when fully charged to around **3.7V** when discharged.

The ADC converts the voltage range of **0V to 5V** into the integer range of **0 to 1023**, and we access this value
by calling **analogRead()**.

This code snippet shows how to do this as well as calculating the relative battery state using the min and max
battery voltages.

```arduino
  float maxVoltage = 4.25;
  float minVoltage = 3.75;

  arduinoPowerVoltage = float(analogRead(arduinoPowerVoltagePin)) / 1024.0 * 5.0;

  float fractionalVoltage = (arduinoPowerVoltage - minVoltage) / (maxVoltage - minVoltage);
```

LiPoly batteries do not lose voltage linearly as they discharge, but it's *close enough* that we can
estimate the remaining battery life using the fractional voltage.

So we can get the battery voltage and estimate the life remaining, but we need a way to communicate
that to the user. We could use an alphanumeric display and in the next section I will show how to
log this to a file on a SD card. But those can be overkill for many projects.

A simpler approach is to use an RGB and change the color to reflect battery life. For example:
- Green - more than 50%
- Yellow - 20% to 50%
- Red - less than 20% - time to recharge

To implement that you just add an RGB Led and two 1K resistors to the circuit and
add a bit of code to **arduinoPowerMonitor()**.

RGB Leds typically use a **Common Anode** so this circuit works with that:


![RGB Led schematic](images/RGB_led_schematic.png)

**NOTE** As I will show later on, there can be a significant 'oscillation' in the measured voltage, for reasons that I don't understand.
If the battery is close to one of the values where the Led color changes, you may see it switch back and forth
between the two states - very annoying. A quick fix is to place an **0.1uF capacitor** between **analog pin A0** and **Ground**.

![RGB Led breadboard](images/p_on_p_off_led_breadboard.png)

This image does not show the battery or the Arduino.



The sketch [arduino_2_voltage_led](arduino_2_voltage_led) implements Power On/Power Off and Led voltage display.

The code sets the green or red pin to **low** to turn the led on and **high** to turn it off.











