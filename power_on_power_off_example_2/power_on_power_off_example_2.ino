/*
  An example that demonstrates the arduino_power power on/power off functions
  in the context of a simple LED blink program

  This blinks the onboard yellow LED

  Based on the Blink example program by Scott Fitzgerald
 */

const int buttonPin = 2;
const int enablePin = 12;
const int ledPin    = 13;
const int voltagePin = A0;

int buttonState = 0;
int batteryVoltage = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);   // Set pin 12 HIGH and leave it

  pinMode(buttonPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

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
  
  batteryVoltage = analogRead(voltagePin);
  
  if (batteryVoltage <= 768) {
    // shutdown the PowerBoost
    digitalWrite(enablePin, LOW);
  }    
  
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(250);              // wait for a second
}

