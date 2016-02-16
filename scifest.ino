/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
 This example code is in the public domain.
 */

#define CUSTOM_SETTINGS
#define INCLUDE_DATA_LOGGER_SHIELD
/* Include 1Sheeld library. */
#include <OneSheeld.h>

/* A name for the LED on pin 13. */
int ledPin = 13;

// the setup routine runs once when you press reset:
void setup() {
  /* Start communication. */
  OneSheeld.begin();
  /* Set a delay. */
  OneSheeld.delay(500);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // Use an external analog reference
  analogReference(EXTERNAL);
}

// the loop routine runs over and over again forever:
void loop() {
  static double avg = 0.0;
  static int count = 0;
  static int log_count = 0;
  static int led_on = 0;
  
  if (led_on) {
      led_on++;
      if (led_on == 5) {
        /* Turn off the LED. */
        digitalWrite(ledPin, LOW);
        led_on = 0;
      }
  }
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
    Serial.print(avg_temp);
    Serial.print(" K ");
    Serial.print(celsius);
    Serial.println(" C ");
    
    if (celsius >= 30 && !led_on) {
      /* Turn on the LED. */
      digitalWrite(ledPin, HIGH);
      led_on = 1;
    }
    count = 0;
    avg = 0;
  }
  OneSheeld.delay(1000);   // delay in between reads for stability
}
