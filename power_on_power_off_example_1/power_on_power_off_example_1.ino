/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

const int buttonPin = 2;
const int enablePin = 12;
const int ledPin    = 13;

int buttonState = 0;

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

  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(250);              // wait for a second
}

