/*
  Reads voltage on pin A0 frm a temperature sensor and converts it to
  a temperature value in Celsius. Triggers PIN 13 if temperature is above
  30 degrees. Logs to 1 sheeld terminal. Uses voice recognition to
  switch system on or off
 */

#define CUSTOM_SETTINGS
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD
// Include 1Sheeld library
#include <OneSheeld.h>

// A name for the LED on pin 13.
int ledPin = 13;
// Used to average out measurements
double avg = 0.0;
int count = 0;
// Flag to switch system on or off. Controlled by voice recognition
int on = 0;
// Turn on debug mode
int debug = 0;
// Need to remember last status as text to speech does not work from callback
int last = 0;
// Recognised voice commands
const char onCommand[] = "on";
const char offCommand[] = "off";
const char debugCommand[] = "start";
const char silentCommand[] = "stop";
// Text to speech messages
const char systemOn[] = "System on";
const char systemOff[] = "System off";
const char temperatureCritical[] = "Temperature Critical";

// The setup routine runs once when you press reset
void setup() {
  // Start communication
  OneSheeld.begin();
  // Setup LED pin
  pinMode(ledPin, OUTPUT);
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
    if (last == 0) {
      TextToSpeech.say(systemOn);
    }
    last = 1;
  } else {
    turnOff();
    if (last == 1) {
      TextToSpeech.say(systemOff);
    }
    last = 0;
  }
  OneSheeld.delay(1000);
}

// Function called from the main loop while status is on
void turnOn() {  
  count++;
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Reference analog voltage
  double aref = 4.425;
  // Calibration offset
  double calibration = 15.17;
  double temperature = sensorValue * 1000.0 * (aref/1023.0) + calibration;
  if (debug == 1) {
    Terminal.print(sensorValue);
    Terminal.print(" ");
    Terminal.println(temperature);
  }
  avg += temperature;
  // print out the value you read:
  if (count == 10) {
    double avg_temp = avg/count;
    double celsius = avg_temp - 273.0;
    
    Terminal.print(avg_temp);
    Terminal.print(" K ");
    Terminal.print(celsius);
    Terminal.println(" C ");
    
    if (celsius >= 30) {
      /* Turn on the LED. */
      digitalWrite(ledPin, HIGH);
      TextToSpeech.say(temperatureCritical);
    } else {
      digitalWrite(ledPin, LOW);
    }
    count = 0;
    avg = 0.0;
  }
}

// Function called from the main loop while status is off
void turnOff() {
  count = 0;
  avg = 0;
  // Make LED blink
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
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
    on = 0;
  }
  else if (!strcmp(commandSpoken, debugCommand)) {
    debug = 1;
  }
  else if (!strcmp(commandSpoken, silentCommand)) {
    debug = 0;
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
