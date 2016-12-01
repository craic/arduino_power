/*
arduino_power_1_power_on_power_off

Copyright 2016 Robert Jones  Craic Computing LLC

Freely distributed under the terms of the MIT open source license

Uses the Adafruit PowerBoost 500C arduino shield

This gets Vbat directly from the battery for the pushbutton
 */


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


// Regular Arduino setup() and loop() functions

void setup() {

  arduinoPowerSetup();

  // Add any other setup here
  // In this example pin 13 is set as an output
  // this has an on-board LED attached
  pinMode(13, OUTPUT);

}


void loop() {

  // Monitor the power status
  arduinoPowerMonitor();
  
  // Flash the on-board LED attached to Pin 13 just to show some activity

  digitalWrite(13, HIGH);
  delay(250);
  digitalWrite(13, LOW);
  
  // Delay 1 second
  delay(1000);
}

