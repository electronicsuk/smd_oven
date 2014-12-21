/*

  Toaster Oven SMT soldering control
  
  Adrian Bowyer
  
  2 November 2011
  
  Licence: GPL
  
*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include "lcd.h"
#include "thermocouple.h"

const int heatPin =  11;     // the number of the LED pin.  This also controls the heater
int heatState = LOW;         // heatState used to set the LED and heater
long previousMillis = 0;     // will store last time LED/heater was updated
const long interval = 1000;  // interval at which to sample temperature (milliseconds)
long time = 0;               // Time since start in seconds
bool done=false;             // Flag to indicate that the process has finished
bool initialized             // flag is thermocouple initialized

// The temperature/time profile as {secs, temp}
// This profile is linearly interpolated to get the required temperature at any time.
// PLEN is the number of entries
#define PLEN 6
long profile[PLEN][2] = { {0, 15}, {120, 150}, {220, 183}, {280, 215}, {320, 183}, {350, 100} };

// Linearly interpolate the profile for the current time in secs, t

int target(long t)
{
  if(t <= profile[0][0])
   return profile[0][1];
  if(t >= profile[PLEN-1][0])
  {
   done = true; // We are off the end of the time curve
   return profile[PLEN-1][1];
  }
  for(int i = 1; i < PLEN-1; i++)
  {
     if(t <= profile[i][0])
       return (int)(profile[i-1][1] + ((t - profile[i-1][0])*(profile[i][1] - profile[i-1][1]))/
         (profile[i][0] - profile[i-1][0]));
  }
  return 0;
}

// Measure the actual temperature from the thermocouple

float temperature(DeviceAddress deviceAddress)
{
  return GetTemperature();
}

void setup() {
  pinMode(heatPin, OUTPUT); 
  Serial.begin(9600);
  InitialiseLCD();
  initialized = ConfigureThermocouple();
  
  if(initialized == false)
  {
    ClearLCD();    
    PrintMessage("No Thermocouple");
  }
}


// Go round and round

void loop()
{
  float t;
  unsigned long currentMillis = millis();
 
 if(currentMillis - previousMillis > interval && initialized == true) 
  {
    previousMillis = currentMillis; // set next time 
    
    // Get the actual temperature
    
    t = temperature(insideThermometer);
    
    // One second has passed
    
    time++;   
    
    // Find the target temperature
    
    int tg = target(time);
    
    // Simple bang-bang temperature control
    
    if (t < tg)
    {
      heatState = HIGH;
    } else
    {
      heatState = LOW;
    }

    // Turn the heater on or off (and the LED)
    digitalWrite(heatPin, heatState);
    
    // Keep the user amused
    if(done)
    {
      Serial.print((char)0x07);  // Bell to wake the user up...
      Serial.print((char)0x07);
      Serial.print("FINISHED ");
    }
    
    Serial.println("Current Temp Kelvin:");

    float tempKelvin = t + 273.15;
    Serial.println(tempKelvin);
    
   Serial.print(time);
    Serial.print(", ");
    Serial.print(tg);
    Serial.print(", ");
    Serial.println(t);
    OvenStateMachine(t,done);
  }
}


