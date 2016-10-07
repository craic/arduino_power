# Arduino Power

** As of 2016/10/07 this is a work in progress - not quite ready for release **

The [Arduino](https://www.arduino.cc) family of microcontroller boards are incredibly versatile devices for
building sensors, robots, and many other uses.

By powering these from rechargeable LiPoly batteries
we can build small self-contained systems for remote monitoring, portable projects, etc.

In most cases power management is trivial. When you connect power to the Arduino it
boots up right away and the loaded software runs in a loop until the power is
disconnected.

Unlike a Raspberry Pi system, there is no concern about the system software being
corrupted by a sudden removal of power.

However, there are cases where you want more control over the power source and, in particular,
you may want to log certain events related to that.

Consider a remote sensing device that logs temperature data, for example. If the battery runs out
while it is unattended the logs simply stop. There is no way to tell immediately that low
battery was the problem. It could have been software or hardware failure.

arduino_power is a way to manange and monitor power from a LiPoly battery that provides
more control than a simple on/off switch.

The project is built on the ideas in the [LiPoPi project](https://github.com/NeonHorizon/lipopi) that provides similar functionality for
Raspberry Pi systems.


The system uses an Adafruit PowerBoost charger to boost the output voltage from a LiPoly battery to
the 5V needed to power the Arduino via USB and to recharge the battery.

The system has three components with hardware and software for each of them:

- Power on by pressing a pushbutton for a few seconds
- Power off by pressing and releasing the same pushbutton
- Automatic shutdown, with a log message, on a low battery signal


The software is intended to be incorporated in your Arduino programs. Simple examples that demonstrate
how it works are included here.


# Power On / Power Off

![power on / power off breadboard](https://github.com/craic/arduino_power/blob/master/images/arduino_power_breadboard_1.png)



This example program simply blinks the Arduino on board yellow LED which is attached to pin 13.




```arduino
/*
  An example that demonstrates the arduino_power power on/power off functions
  in the context of a simple LED blink program

  This blinks the onboard yellow LED

  Based on the Blink example program by Scott Fitzgerald
 */

const int buttonPin = 2;
const int enablePin = 12;
const int ledPin    = 13;

int buttonState = 0;
int lowBatteryState = HIGH;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);   // Set pin 12 HIGH and leave it

  pinMode(buttonPin, INPUT);

  // need to delay for three seconds to allow the Arduino to 'boot'
  // before starting to monitor the pushbutton in shutdown mode
  delay(3000);
}

// the loop function runs over and over again forever
void loop() {

  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    // shutdown the PowerBoost
    digitalWrite(enablePin, LOW);
  }


  digitalWrite(ledPin, HIGH);   // turn the LED on
  delay(500);
  digitalWrite(ledPin, LOW);    // turn the LED off
  delay(250);
}

```





# Low Battery Shutdown

** to be added **

![power on / power off breadboard photo](https://github.com/craic/arduino_power/blob/master/images/breadboard_2_photo.png)


