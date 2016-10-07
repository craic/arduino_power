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
