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
  /* Save any previous logged values. */
  //Logger.stop();
  /* Set a delay. */
  OneSheeld.delay(500);
  /* Start logging in a new CSV file. */
  //Logger.start("Temp values");
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
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
  //Serial.println(sensorValue);
  double temperature = sensorValue * (273.0/56.0);
  avg += temperature;
  // print out the value you read:
  if (count == 5) {
    int avg_temp = avg/count;
    int celsius = avg_temp - 273;
    Serial.print(avg_temp);
    Serial.print(" K ");
    Serial.print(celsius);
    Serial.println(" C ");
    
    /* Add temperature values as a column in the CSV file. */
    //Logger.add("Temperature",celsius);
    /* Log the row in the file. */
    //Logger.log();
    log_count++;
    /* Stop logging after 20 readings and save the CSV file. */
    if(log_count == 20)
    {
      /* Save the logging CSV file. */
      //Logger.stop();
      /* Reset counter. */
      log_count = 0;
      /* Start Logging again. */
      //Logger.start("Temp values");
    }  
    
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
