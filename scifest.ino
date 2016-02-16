/*
  Reads voltage on pin A0 frm a temperature sensor and converts it to
  a temperature value in Celsius. Triggers PIN 13 if temperature is above
  30 degrees. Logs to 1 sheeld terminal. Uses voice recognition to
  switch system on or off
 */

#define CUSTOM_SETTINGS
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_TERMINAL_SHIELD
// Include 1Sheeld library
#include <OneSheeld.h>

// A name for the LED on pin 13.
int ledPin = 13;
// Used to average out measurements
double avg = 0.0;
int count = 0;
// Flag to switch system on or off. Controlled by voice recognition
int on = 0;
// Recognised voice commands
const char onCommand[] = "on";
const char offCommand[] = "off";

// The setup routine runs once when you press reset
void setup() {
  // Start communication
  OneSheeld.begin();
  // Initialize serial communication at 9600 bits per second:
  //Serial.begin(9600);
  // Use an external analog reference
  analogReference(EXTERNAL);
  // Voice recognition setup
  VoiceRecognition.setOnError(&errorFunction);
  VoiceRecognition.setOnNewCommand(&cmdFunction);
}

// The loop routine runs over and over again forever:
void loop() {
  if (on) {
    turnOn();
  } else {
    turnOff();
  }
  OneSheeld.delay(1000);
}

// Function called from the main loop while status is on
void turnOn() {
  count++;
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  //Serial.print(sensorValue);
  double temperature = sensorValue * (293.4/59.5);
  //Serial.print(" ");
  //Serial.println(temperature);
  avg += temperature;
  // print out the value you read:
  if (count == 10) {
    int avg_temp = avg/count;
    int celsius = avg_temp - 273;
    
    Terminal.print(avg_temp);
    Terminal.print(" K ");
    Terminal.print(celsius);
    Terminal.println(" C ");
//    Serial.print(avg_temp);
//    Serial.print(" K ");
//    Serial.print(celsius);
//    Serial.println(" C ");
    
    if (celsius >= 30) {
      /* Turn on the LED. */
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
    count = 0;
    avg = 0;
  }
}

// Function called from the main loop while status is off
void turnOff() {
  count = 0;
  avg = 0;
  // Make LED blink
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
}

// This function will be invoked each time a new command is given
void cmdFunction (char *commandSpoken)
{
  // Print out the command spoken
  Terminal.println(commandSpoken);
  //Serial.println(commandSpoken);
  if (!strcmp(commandSpoken, onCommand)) {
    on = 1;
  }
  else if (!strcmp(commandSpoken, offCommand)) {
    off = 1;
  }
}

// Error callback
void errorFunction (byte errorType)
{
  switch(errorType)
  {
     case NETWORK_TIMEOUT_ERROR: Terminal.println("Network timeout");break;
     case NETWORK_ERROR: Terminal.println("Network Error");break;
     case AUDIO_ERROR: Terminal.println("Audio error");break;
     case SERVER_ERROR: Terminal.println("No Server");break;
     case SPEECH_TIMEOUT_ERROR: Terminal.println("Speech timeout");break;
     case NO_MATCH_ERROR: Terminal.println("No match");break;
     case RECOGNIZER_BUSY_ERROR: Terminal.println("Busy");break;
  }
}
