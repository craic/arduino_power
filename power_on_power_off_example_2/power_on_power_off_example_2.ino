/*
  An example that demonstrates the arduino_power power on/power off functions
  in the context of a simple LED blink program

  This blinks the onboard yellow LED

  Based on the Blink example program by Scott Fitzgerald
 */

const int ledPin    = 13;

const int enablePin   = 12;
const int buttonPin   = 11;
const int redLedPin   = 10;
const int greenLedPin =  9;

const int voltagePin = A0;


int buttonState = 0;
int batteryVoltage = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  
  
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);   // Set pin 12 HIGH and leave it

  pinMode(buttonPin, INPUT);
//  pinMode(buttonPin, INPUT_PULLUP);

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
  
  // using a voltage divider so the real voltage is twice the measured
  
  batteryVoltage = analogRead(voltagePin) * 2;
  
  // need to average this over some number of readings to avoid 'blinking'
  // when the system is near a transition point...
  
//  if (batteryVoltage <= 768) {
//    // shutdown the PowerBoost
//    digitalWrite(enablePin, LOW);
//  }  

  // max battery voltage is 4.25 -> 870
  // min battery voltage is 3.75 -> 768
  // 50% is 819
  // 25% is 793
  // 10% is 778
  if (batteryVoltage > 818) {
    // > 50% green
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
  } else if (batteryVoltage > 793) {
    // > 25% yellow
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, LOW);
  } else {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
  } 

  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
 
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(250);              // wait for a second
}

