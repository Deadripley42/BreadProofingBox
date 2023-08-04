/*
 * Author: Markus Tuzzolino (markustuzzo@gmail.com)
 * 
 * Description:
 * Implementation of a simple temperature controller for a bread proofing box.
 * Temperature and Humidity are measured using a DHT22-sensor by SEEED.
 * The heating element is controlled via a HLS8L-relais and only relies on the
 * measured temperature.
 * 
 * The actual controller is implemented in a "very" basic way including just 
 * two cases:
 *
 *    (1) measured temperature is below target-temperature:
 *        -> turn-on heating element
 *
 *    (2) measured temperature is above target-temperature:
 *        -> turn-off heating element
 *   
 * Temperature and Humidity are  measured every 50 ms. Action is taken only
 * if the threshold is being exceeded in either way. The measured data itself
 * is being printed only every 500 ms. This requires the use of two
 * counter-variables "COUNTER" and "SUBCOUNT".
 * The output is captured using CoolTerm.    
 *
 * The controller is subject to change and will be updated once basic
 * research is finished. Being implemented in its rudimentary way the
 * controller itself is doing a decent job as long as the breads being
 * subject to proof are heavy enough (m > 800 g).    
 * 
 * Pin connections:
 *        - Pin 2 <- DHT22 (Temperature and humidity sensor)
 *        - Pin 4 -> HLS8L (Relais connected to the heating element)
 */

// Setup of DHT22-sensor
#include "DHT.h"
#define DHTTYPE DHT22
#define DHTPIN 2

// Initialization of the target temperatur (at which bread is beeing proofed)
float TARGET_TEMP = 24;

// Initialization of counter variables for the main loop
int COUNTER = 0;
int SUBCOUNT = 0;

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(9600);
  // Initialization of the HLS8L-relais
  pinMode(4, OUTPUT);
}

void loop(){
  float temp_hum_val[2] = {0};
  
  if(!dht.readTempAndHumidity(temp_hum_val)){
    // Case (1) (see decription)
    if(temp_hum_val[1] < TARGET_TEMP){
        digitalWrite(4, HIGH);
    } 
    // Case (2) (see decription)
    if(temp_hum_val[1] > TARGET_TEMP){
      digitalWrite(4, LOW);
    }
    // Print measured data only every 500 ms ...
    // ... but check data every 50 ms (see end of loop).
    if(COUNTER == 500){
      COUNTER = 0;
      // SUBCOUNT indicates reading points
      Serial.print(SUBCOUNT);
      Serial.print(" ");
      Serial.print(temp_hum_val[1]);
      Serial.print(" ");
      Serial.print(temp_hum_val[0]);
      Serial.print('\n');
      SUBCOUNT += 1;
  }

  } else{
      Serial.println("Failed to get temprature and humidity value.");
  }
  
  COUNTER += 50;
  delay(50);
}