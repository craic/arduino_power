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

# Adafruit PowerBoost Shield

The system uses a rechargeable LiPoly Battery and an Adafruit PowerBoost Charger, which can both charge the battery and boost its output to 5V.
[Adafruit](https://www.adafruit.com) makes three versions of the Power Boost - two breakout boards and an Arduino Shield:
- [PowerBoost 500 Charger - Rechargeable 5V Lipo USB Boost @ 500mA+](https://www.adafruit.com/products/1944)
- [PowerBoost 1000 Charger - Rechargeable 5V Lipo USB Boost @ 1A - 1000C](https://www.adafruit.com/products/2465)
- [PowerBoost 500 Shield - Rechargeable 5V Power Shield](https://www.adafruit.com/products/2078)

Most of the work described here uses the Power Shield, but I'll talk about using the breakout boards later on.

The [PowerBoost 500 Shield](https://www.adafruit.com/products/2078) looks like an empty shield but it has the PowerBoost machinery
located around its edges, allowing you to place a rectangular LiPo battery, such as 2000mAh battery, in the middle of the shield.
The 5V output from the boost circuit is available on the standard Arduino shield 5V pin, which allows you to stack this with an Arduino and
other shields without any additional wires.

By default the shield will deliver power but you can add the optional switch that they provide to turn power on and off as desired. *In this
project you do not want to add that switch !* We are going to add some simple external circuitry which extends the
functionality of the board, at the expense of a few external wires.

For this project we need access to 2 'pins' on the PowerBoost - One has the Battery Voltage and the other is an Enable pin that lets us turn
the unit on and off.

These are easy to access on the breakout board versions, less so in the case of the shield.

I'll explain how we use these below, but here let me explain how we access them on the shield.


The Enable pin on the Shield is unmarked but if you find the location for the optional switch on the left edge of the board,
marked as 'S1', then the pin is the lowest of the 3 *small* holes (not the larger holes).

![PowerBoost Shield Enable Pin](/images/powerboost_shield_enable_pin.png){: width="200px"}







The PowerBoost has an Enable pin which is used to turn the unit on and off. This is easy to access in the breadboard versions of the
PowerBoost but is not explicitly available on the shield





# operation

The system has three components with hardware and software for each of them:

- Power on by pressing a pushbutton for a few seconds
- Power off by pressing and releasing the same pushbutton
- Automatic shutdown, with a log message, on a low battery signal


The software is intended to be incorporated in your Arduino programs. Simple examples that demonstrate
how it works are included here.


# Power On / Power Off


This example program simply blinks the Arduino on board yellow LED which is attached to pin 13.




```arduino
// dummy text

```





# Low Battery Shutdown

** to be added **



