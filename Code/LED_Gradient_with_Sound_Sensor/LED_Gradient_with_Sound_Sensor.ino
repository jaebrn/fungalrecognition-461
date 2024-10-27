
/* 
TITLE: LED Gradient pattern with sound Sensor Module
DATE: October 25th 2024
Fungal Re-Cognition: Created by Mariam Aoune, Jennifer Brown, 
Leonardo Morales Vega, Francis Ouellette, Negar Roofigariesfahani, 
Jaden Thompson, and Jiahao Wu for CART 461 in Fall 2024

This example shows how to display a moving gradient pattern on
 * an APA102-based LED strip. */

/* By default, the APA102 library uses pinMode and digitalWrite
 * to write to the LEDs, which works on all Arduino-compatible
 * boards but might be slow.   */


//to support faster update rates

#include <APA102.h>

int digitalPin = 7;   // KY-037 digital interface
int analogPin = A0;   // KY-037 analog interface
int ledPin = 13;      // Arduino LED pin
int digitalVal;       // digital readings
int analogVal;        // analog readings

// Define which pins to use.
const uint8_t dataPin = 11;
const uint8_t clockPin = 12;

// Create an object for writing to the LED strip.
APA102<dataPin, clockPin> ledStrip;

// Set the number of LEDs to control.
const uint16_t ledCount = 60;

// Create a buffer for holding the colors (3 bytes per color).
rgb_color colors[ledCount];

// Set the brightness to use (the maximum is 31).
const uint8_t maxBrightness = 30;

void setup()
{
  pinMode(digitalPin,INPUT); 
  pinMode(analogPin, INPUT);
  pinMode(ledPin,OUTPUT);      
  Serial.begin(9600);
}

void loop()
{

   // Read the sound level and store it in valueSound
    int valueSound = sound();
    // Print the sound level to the serial monitor

    // Update the LED colors based on the sound level
    color(valueSound);
}

void color(int valueSound){
  uint8_t time = millis() >> 2;
  uint8_t brightness = map(valueSound, 0, 1023, 0, maxBrightness);
  Serial.println(brightness);
  for(uint16_t i = 0; i < ledCount; i++)
  {
   uint8_t x = time - i * 8.5;  // Determine color value based on time
        // Set each LED to a color based on the gradient calculation
        colors[i] = rgb_color(x, 255 - x, x);
  }

     // Write the color data to the LED strip
    ledStrip.write(colors, ledCount, brightness);
   

  delay(10);

}

int sound(){
  
  // Read the digital inteface
  digitalVal = digitalRead(digitalPin); 
  
  if(digitalVal == HIGH) 
  {
    digitalWrite(ledPin, HIGH); // Turn ON Arduino's LED
  }
  else
  {
    digitalWrite(ledPin, LOW);  // Turn OFF Arduino's LED
  }

  // Read analog interface
  analogVal = analogRead(analogPin);
  // Print analog value to serial

  return analogVal; 
}
