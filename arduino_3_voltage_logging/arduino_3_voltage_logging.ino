/*
arduino_3_voltage_logging

Copyright 2016 Robert Jones  Craic Computing LLC
Data logging code is adapted from https://github.com/adafruit/Light-and-Temp-logger

Freely distributed under the terms of the MIT open source license

Uses the Adafruit PowerBoost 500C arduino shield and
the Adafruit Data Logging Shield

Measure voltage and record it with a timestamp in a CSV file on
the SD card in the Data Logging Shield
*/


// Includes and Variables for Data Logging

#include <SPI.h>
#include <SD.h>

#include <Arduino.h>
#include <Wire.h>         // this #include still required because the RTClib depends on it
#include "RTClib.h"

// used for debugging on the Serial Monitor
// comment out/remove all references to Serial if not using
#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

RTC_Millis rtc;

// On the Ethernet Shield, CS is pin 4. Note that even if it's not used 
// as the CS pin, the hardware CS pin (10 on most Arduino boards, 53 on the Mega) 
// must be left as an output or the SD library functions will not work.
const int chipSelect = 4;

File logFile;
// Filename must follow 8.3 format - name is up to 8 characters, suffix up to 3 characters
// https://en.wikipedia.org/wiki/8.3_filename
char *logFilename = "voltage.csv";
String logFileHeader = "Timestamp,Voltage,Message";
String logMessage = "";
String dataString = "";

// used for debugging on the Serial Monitor
// comment out/remove all references to Serial if not using
int serialFlag = 0;


// Global variables for Arduino Power
const int arduinoPowerEnablePin   = 8;
const int arduinoPowerButtonPin   = 2;
const int arduinoPowerGreenLedPin = 6;
const int arduinoPowerRedLedPin   = 7;

const int arduinoPowerVoltagePin  = A0;

unsigned long arduinoPowerCurrentTime  = 0;
unsigned long arduinoPowerPreviousTime = 0;
//unsigned long arduinoPowerLogInterval = 600000;  // 10 minutes in milliseconds
unsigned long arduinoPowerLogInterval = 30000;  // 30s in milliseconds

float arduinoPowerVoltage = 0.0;
// minimum fractional voltage at which to shut down the PowerBoost 
float arduinoPowerMinFractionalVoltage = 0.05;

// Note the volatile type - used to read the button state (not used here)
volatile int arduinoPowerButtonState = 0;

void arduinoPowerSetup() {
  
  // Setup the LED pins
  pinMode(arduinoPowerRedLedPin, OUTPUT);
  pinMode(arduinoPowerGreenLedPin, OUTPUT);

  // Setup the Power On / Power Off pins
  pinMode(arduinoPowerButtonPin, INPUT);
  pinMode(arduinoPowerEnablePin, OUTPUT);
  // On Arduino boot, set the Enable Pin high - this keeps the PowerBoost On
  digitalWrite(arduinoPowerEnablePin, HIGH);

  // need to delay for three seconds to allow the Arduino to 'boot'
  // before attaching the interrupt, otherwise it will power off
  delay(3000);

  // Attach an interrupt to the ISR vector
  // pin 2 is linked to Interrupt 0 (INT0)
  attachInterrupt(0, arduinoPowerInterrupt, RISING);
}

void arduinoPowerInterrupt() {
  // shutdown the PowerBoost when the button is pressed while Arduino 
  // is running - this only works when the button is released...
  // This is because the power ON circuit is also activated when the button is down
  
  // Log a message to the SD card that the user has shut it down
  logMessage = "User shutdown";
  logDataToFile();
  logFile.close();
  arduinoPowerShutdown();
}

// Shutdown the PowerBoost by pulling the Enable Pin low
void arduinoPowerShutdown() {
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

  // limits for a LiPoly battery
  float maxVoltage = 4.25;
  float minVoltage = 3.75;
 
  arduinoPowerVoltage = float(analogRead(arduinoPowerVoltagePin)) / 1024.0 * 5.0;
  
  float fractionalVoltage = (arduinoPowerVoltage - minVoltage) / (maxVoltage - minVoltage);
  
  if(fractionalVoltage > 1.0) {
    fractionalVoltage = 1.0;
  } else if(fractionalVoltage < 0.0) {
    fractionalVoltage = 0.0;
  }

  // comment out this section if you are not using an LED
  // Modify the cutoff values to suit
  if (fractionalVoltage > 0.5) {
    arduinoPowerLed("green");
  } else if (fractionalVoltage > 0.25) {
    arduinoPowerLed("yellow");
  } else {
    arduinoPowerLed("red");
  }   

  // shutdown the powerboost if less than 5% battery life left
  if (fractionalVoltage <= arduinoPowerMinFractionalVoltage) {
    logMessage = "Low battery - shutting down";
    logDataToFile();
    logFile.close();
    arduinoPowerShutdown();
  }  

}


// Add a leading zero to the string representation of a number if less than 10
// used by buildTimestamp()
String zeropadString(uint8_t number) {
  String tmpString = "";
  if(number < 10) {
    tmpString = "0" + String(number);
  } else {
    tmpString = String(number);
  }
  return(tmpString);
}


// Construct a timestamp string in ISO8601 format
// for example: 2016-12-02T15:02:31
// RTC library does not pass the time zone
// Also, possible issues with Leap years and Daylight Savings Time
String buildTimestamp() {
    DateTime now = rtc.now();      
    String timestamp = "";
    timestamp += String(now.year());
    timestamp += '-';
    timestamp += zeropadString(now.month());
    timestamp += '-';
    timestamp += zeropadString(now.day());
    timestamp += 'T';
    timestamp += zeropadString(now.hour());
    timestamp += ':';
    timestamp += zeropadString(now.minute());
    timestamp += ':';
    timestamp += zeropadString(now.second());  
    return(timestamp);
}


void addFieldToDataString(String string) {
  if(dataString != "") {
    dataString += ',';
  }
  dataString += string;
}


// Log Time, voltage and any other data to the SD file in CSV format
void logDataToFile() {
  
  arduinoPowerCurrentTime = millis();
  
  if(arduinoPowerCurrentTime - arduinoPowerPreviousTime >= arduinoPowerLogInterval or logMessage != "") {

    arduinoPowerPreviousTime = arduinoPowerCurrentTime;

    addFieldToDataString(buildTimestamp());
    addFieldToDataString(String(arduinoPowerVoltage, 3));
    addFieldToDataString(logMessage);
   
    logFile.println(dataString);
    
    // If debugging, print to the serial port as well
    if(serialFlag == 1) {
      Serial.println(dataString);
    }

    logFile.flush();
    dataString = "";
    logMessage = "";
  }
}

// A hack to flash the red LED is there is an error, such as no SD card
void flashRedLed() {
  while(1) {
    arduinoPowerLed("red");
    delay(200);
    arduinoPowerLed("off");
    delay(200);
  }
}


// Regular Arduino setup() and loop() functions

void setup() {

  arduinoPowerSetup();

  // Setup code for the Data Logger, creates a SD card file for output
  
  // following line sets the RTC to the date & time this sketch was compiled
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  
  // Used for debugging on the Serial monitor - comment out all references to Serial if not using
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  if(Serial) {
    // serial port is connected
    serialFlag = 1;
    Serial.print("Initializing SD card...");
  }

  // make sure that the default chip select pin for the SD card is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    if(serialFlag == 1) {   
      Serial.println("Card failed, or not present");
    }
    // ** Flash the LED as an alert **
    flashRedLed();
  }
  if(serialFlag == 1) {
    Serial.println("card initialized.");
  }
  // Open up the file we're going to log to - Remove the file if it exists
  if(SD.exists(logFilename)) {
    SD.remove(logFilename);
  }

  logFile = SD.open(logFilename, FILE_WRITE);
  if (! logFile) {
    if(serialFlag == 1) {
       Serial.println("error opening datalog.txt");
    }
    // we can't write data - Flash the LED as an alert
    flashRedLed();
  }
  
  // Write the log file CSV format header
  logFile.println(logFileHeader);
  logFile.flush();
}


void loop() {
  // Monitor the power status
  arduinoPowerMonitor();
  
  logDataToFile();
}

