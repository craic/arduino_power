#Power On / Power Off Circuit

This basic circuit uses the press of a momentary pushbutton switch to power up the system and, when running, to power it down.

When you press and hold on a powered down system, the PowerBoost is turned on and supplies power to the Arduino which then boots up
and runs the program that you have loaded onto it.

When you press the button again, and release it, the Arduino disables the PowerBoost and, because it is no longer supplying power,
the Arduino shuts down.

With the more complex versions of this, the system logs a shutdown message, etc., before actually shutting down.

**NOTE** This example uses an arduino **Interrupt** to detect the *power Off* button press. This frees up the rest of the
code to do its thing. For example, you might have it take a sensor measurement every 30 minutes and sleep during the interval.
Interrupts on Arduino can be quite complicated but this version is straightforward.


###How this works

The PowerBoost has an **Enable pin** which is held HIGH by default. In this state, the PowerBoost is enabled and supplies power
via the 5V pin on the Arduino headers. The Blue LED on the shield is lit when it is supplying power.

If that pin is connected to ground, through **R1** in the schematic below, then the PowerBoost is disabled and does not provide power.

You add two functions, **ArduinoPowerSetup** and **ArduinoPowerMonitor**, to your code which configure **digital pins 8 and 2**
as input and output, respectively.

![Power On / Power Off Circuit](/images/power_on_power_off_schematic.png)

####Power On

When the pushbutton is pressed, the battery voltage is connected to the **Enable pin** and pulls it HIGH, which turns on the
PowerBoost output and which, in turn, boots the Arduino.

The **ArduinoPowerSetup** function, included in your program, sets Arduino **digitial pin 8** to **output** and HIGH.
This ensures that the **Enable pin** remains high when the pushbutton is released, ensuring that the PowerBoost remains on.

####Power Off
The **ArduinoPowerSetup** function has configured Arduino **digital pin 2** as an input and resistor **R3** pulls this LOW.

On a running system, pressing the pushbutton again, pulls **pin 2** HIGH. The **ArduinoPowerMonitor** function
in your program monitors this and sets **pin 8** to LOW.

This should set the **Enable pin** to LOW and turn off the PowerBoost. However, because the pushbutton is **down**, the
**Enable pin** remains HIGH given the voltage from the battery. It is only when the pushbutton is **released** that voltage is removed
and the **Enable pin** is pulled LOW, turning off the PowerBoost and Arduino.

####The Hack...

In essence, there are two separate circuits connected to the pushbutton - the **power on** path and the **power off** path.

The role of the two 1N4001 diodes is to direct the current flow from the battery when the pushbutton is pressed and to avoid
any unwanted current paths.

When you **power on** the device, you also set up the **power off** function - and *if you keep the pushbutton pressed for
too long then the Arduino will power off the PowerBoost immediately.*

To handle this, the **ArduinoPowerSetup** function calls a **3 second delay()** before entering the main program loop.

Here are the current paths in the four states:

![Power On / Power Off Cycle](/images/power_on_power_off_cycle.png)

####The Circuit

To wire up this circuit, using the PowerBoost Shield, we need another hack as the board does not provide
a direct connection to the Battery positive lead. You can access this via one of the **analog pins** as described on
the [PowerBoost](PowerBoostShield.md) page, **BUT** that pin has 2K resistor in series to limit current. In its powered down state
there is only around 1.5 V on this pin, which is not enough to pull the **Enable pin** HIGH.

The workaround is to not plug the LiPo battery directly into the PowerBoost Shield. Instead use a
[Switched JST-PH 2-Pin SMT Right Angle Breakout Board](https://www.adafruit.com/products/1863) ($2.50) and a
[JST 2-pin cable](https://www.adafruit.com/products/261) ($0.75) and take off the Battery voltage from the positive lead
on a breadboard.

**Note** This layout leaves out the actual Arduino as that is part of your stack with the PowerBoost and you would mount the battery
in the space on the PowerShield, with a bit of double-sided tape. I've kept it separate here for clarity.


![Power On / Power Off Breadboard](images/power_on_power_off_breadboard.png)

**Note** the switched JST breakout board is useful as you can switch off the battery when you are uploading your code via USB, which
itself provides power to the Arduino. Not necessary, but I prefer to isolate the two power sources.


####Example code

[arduino_1_power_on_power_off](/arduino_1_power_on_power_off) is the example application for these functions. Aside from power control, all it does is
blink the **onboard** LED attached to **pin 13**.

There are two global variables and two functions, **arduinoPowerSetup() and arduinoPowerMonitor()** that you can
add to your code. Here is the arduinoPower-*specific* code:


```arduino
// Global variables for Arduino Power
const int arduinoPowerEnablePin   =  9;
const int arduinoPowerButtonPin   =  8;

void arduinoPowerSetup() {
  // When the Arduino boots, set the Enable Pin high and leave it
  // This keeps the Powerboost Enable pin High and therefore running
  pinMode(arduinoPowerEnablePin, OUTPUT);
  digitalWrite(arduinoPowerEnablePin, HIGH);

  // Set up the Button Pin to act as an input
  pinMode(arduinoPowerButtonPin, INPUT);

  // need to delay for three seconds to allow the Arduino to 'boot'
  // before starting to monitor the pushbutton in shutdown mode
  delay(3000);
}

void arduinoPowerMonitor() {
  // shutdown the PowerBoost when the button is pressed while Arduino
  // is running - this only works when the button is released...
  // This is because the power-up circuit is activated when the button is down

  if (digitalRead(arduinoPowerButtonPin) == HIGH) {
    digitalWrite(arduinoPowerEnablePin, LOW);
  }
}
```

**Note** you can use different Arduino pins but you might want to avoid pins 10-13 as these are used in the
Serial Peripheral Interface (SPI).


## Power Off using an Interrupt

The software component of the Power Off circuit I just described works fine if the cycle time of the *loop()*
function is short - say, around 1 second. But you might want a much longer cycle time if, for example, you
want to measure a sensor value every 30 minutes by adding a long *delay()* to the loop. It would be nearly
impossible to see the pushbutton press.

In this situation we need to **interrupt** the program flow when the button is pressed.

There are several ways to use **Interrupts** on the Arduino. I'm using a simple **External Interrupt* here.

**NOTE** As well as changing the code, you need to switch the Power Off wire from **Pin 8** to **Pin 2**.






*to be added*

##Circuit using a PowerBoost Breakout Board

*to be added*

&nbsp;
##[Back to the README](README.md)

