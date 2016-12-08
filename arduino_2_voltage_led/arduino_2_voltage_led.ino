/*
arduino_power_2_voltage_led

Copyright 2016 Robert Jones  Craic Computing LLC

Freely distributed under the terms of the MIT open source license

Uses the Adafruit PowerBoost 500C arduino shield

Measure voltage and display using an RGB led
*/


// Global variables for Arduino Power
const int arduinoPowerEnablePin   = 8;
const int arduinoPowerButtonPin   = 2;
const int arduinoPowerGreenLedPin = 6;
const int arduinoPowerRedLedPin   = 7;

const int arduinoPowerVoltagePin  = A0;

float arduinoPowerVoltage = 0.0;


// Note the volatile type - used to read the button state (not used here)
volatile int arduinoPowerButtonState = 0;

void arduinoPowerSetup() {
  
  // Setup the pins
  pinMode(arduinoPowerRedLedPin, OUTPUT);
  pinMode(arduinoPowerGreenLedPin, OUTPUT);
 
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

// Set an RGB LED - this version assumes a common anode
void arduinoPowerLed(String color) {
  if(color == "red") {
    digitalWrite(arduinoPowerGreenLedPin, HIGH);
    digitalWrite(arduinoPowerRedLedPin,   LOW);    
  } else if(color == "yellow") {
    digitalWrite(arduinoPowerGreenLedPin, LOW);
    digitalWrite(arduinoPowerRedLedPin,   LOW);    
  } else if(color == "green") {
    digitalWrite(arduinoPowerGreenLedPin, LOW);
    digitalWrite(arduinoPowerRedLedPin,   HIGH);
  } else {
    // turn the LED off
    digitalWrite(arduinoPowerGreenLedPin, HIGH);
    digitalWrite(arduinoPowerRedLedPin,   HIGH);
  }      
}

void arduinoPowerMonitor() {

  float maxVoltage = 4.25;
  float minVoltage = 3.75;
 
  arduinoPowerVoltage = float(analogRead(arduinoPowerVoltagePin)) / 1024.0 * 5.0;
  
  float fractionalVoltage = (arduinoPowerVoltage - minVoltage) / (maxVoltage - minVoltage);
  
  if(fractionalVoltage > 1.0) {
    fractionalVoltage = 1.0;
  } else if(fractionalVoltage < 0.0) {
    fractionalVoltage = 0.0;
  }

  // Modify the cutoff values to suit
  if (fractionalVoltage > 0.5) {
    arduinoPowerLed("green");
  } else if (fractionalVoltage > 0.25) {
    arduinoPowerLed("yellow");
  } else {
    arduinoPowerLed("red");
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

