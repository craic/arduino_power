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

For this project we need access to 2 'pins' on the PowerBoost - One has the **Battery Voltage** and the other is the **Enable** pin that lets us turn
the unit on and off.

These are easy to access on the breakout board versions, less so in the case of the shield. I'll
explain how we use the boards below, but here I am going to focus solely on the **shield**.

I will assume that you have already soldered
on the Arduino headers onto the board. See the [Adafruit tutorial](https://learn.adafruit.com/adafruit-powerboost-500-shield-rechargeable-battery-pack)
for details - **do not install the switch**.


The **Enable** pin on the Shield is unmarked but if you find the location for the optional switch on the left edge of the board,
marked as 'S1', then the pin is the lowest of the 3 *small* holes (not the larger holes).

![PowerBoost Shield Enable Pin](/images/powerboost_shield_enable_pin.png)


The shield provides a way to access the battery voltage via one of the 6 **Analog In** pins. These each have
an Analog to Digital converter on the Arduino board and so they can be read by your software.

You need to choose which of these pins you want to read battery voltage on and you set this by
bridging the corresponding solder pads on the underside of the shield.

![PowerBoost Shield Voltage Pads](/images/powerboost_shield_voltage_pin.png)

If you have never done that before, you want to put a blob of solder across the 'V' gap in the appropriate pad.
This is reversible but it is a bit of a fiddle, so I sometimes solder a short loop of wire to bridge the
pads. That way I can remove it more easily if I change my mind.

In the example here I am going to use pin **A0**, which is the innermost of the six.

&nbsp;

**In summary, to prepare the Shield kit you need to:**

1. Solder on the stacking header strips
2. Do NOT attach the switch
3. Solder on a length of wire (4 inches, 22AWG solid core) to the Enable Pin
4. Bridge the pads for Analog pin A0 on the underside of the shield


&nbsp;

## Bonus tip: Using the PowerBoost shield with the Adafruit Metro

The standard Arduino Uno comes with headers already attached which means that you can only stack shields on top, not below.
Adafruit make an Arduino clone called the [Adafruit Metro](https://www.adafruit.com/products/2466) which works just
like the Arduino Uno but with a few nice tweaks, such as a micro USB connector.

You can get the Metro unassembled, but with supplied *non-stacking* headers.

What I have done on a couple of projects is to use the *non-stacking* headers on the PowerBoost shield and use
its *stacking* headers on the Metro. That way you can attach the PowerBoost below the Metro, making it easier
to wire up as you can see the pin labels on the Metro.

![Metro PowerBoost Stack](/images/metro_powerboost_stack.png)

