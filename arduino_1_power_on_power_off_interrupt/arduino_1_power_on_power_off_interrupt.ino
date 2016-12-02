/*
arduino_power_1_power_on_power_off_interrupt

Copyright 2016 Robert Jones  Craic Computing LLC

Freely distributed under the terms of the MIT open source license

Uses the Adafruit PowerBoost 500C arduino shield

This gets Vbat directly from the battery for the pushbutton

** This Version uses an External Interrupt
*/


// Global variables for Arduino Power
const int arduinoPowerEnablePin = 8;
const int arduinoPowerButtonPin = 2;

// Note the volatile type - used to read the button state (not used here)
volatile int arduinoPowerButtonState = 0;

void arduinoPowerSetup() {

  // Setup the pins
  pinMode(arduinoPowerButtonPin, INPUT);
  pinMode(arduinoPowerEnablePin, OUTPUT);
 
  // On Arduino boot, set the Enable Pin high - this keeps the PowerBoost On
  digitalWrite(arduinoPowerEnablePin, HIGH);
  
  // need to delay for three seconds to allow the Arduino to 'boot'
  // before attaching the interrupt
  delay(3000);

  // Attach an interrupt to the ISR vector
  // pin 2 is linked to Interrupt 0 (INT0)
  attachInterrupt(0, arduinoPowerInterrupt, RISING);
}

void arduinoPowerInterrupt() {
  // shutdown the PowerBoost when the button is pressed while Arduino 
  // is running - this only works when the button is released...
  // This is because the power ON circuit is also activated when the button is down
    digitalWrite(arduinoPowerEnablePin, LOW);
}

void arduinoPowerMonitor() {
  // in this example this function does nothing !
  // in other examples this would log the battery voltage, etc.
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

