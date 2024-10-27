/*TITLE: LED Gradient pattern with sound Sensor Module
DATE: October 25th 2024
Fungal Re-Cognition: Created by Mariam Aoune, Jennifer Brown, 
Leonardo Morales Vega, Francis Ouellette, Negar Roofigariesfahani, 
Jaden Thompson, and Jiahao Wu for CART 461 in Fall 2024
*/

//Libraries:
#include <CapacitiveSensor.h>
#include <APA102.h>

//Capacitive Sensor:
CapacitiveSensor sensorA = CapacitiveSensor(4,2);  // 1M resistor between pins 4 & 2, pin 2 is sensor pin
long previousReading = 0;
int cycles = 0; // count of readings 
bool touchState = false; // false when no touch, true when touched
bool previousState = false;

//LEDs:
APA102<dataPin, clockPin> ledStrip; // led strip object
const uint8_t dataPin = 11;
const uint8_t clockPin = 12;
rgb_color colors[ledCount]; // Create a buffer for holding the colors (3 bytes per color)
const uint16_t ledCount = 60; // Number of LEDs in the strip
const uint8_t maxBrightness = 30; // Set the brightness to use (the maximum is 31)

void setup() {
  sensorA.set_CS_AutocaL_Millis(1000); // may want to play with this value 
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
}

void loop() {
  long measurementA =  sensorA.capacitiveSensor(30); // take measurement
  Serial.println(); //debug
  Serial.print("Sensor A: ");
  Serial.print(measurementA);

//periodic recalibration every x cycles
  if(cycles>=50){
    sensorA.set_CS_AutocaL_Millis(1000);
    cycles = 0; //reset cycles
    Serial.println("recalibrating");
  }

//State change based on relation to previous measurement
  if(measurementA>60+(previousReading * 2)){
    touchState = true;
    digitalWrite(LED_BUILTIN,HIGH);
  }else if(measurementA<(previousReading * 0.5)){
    touchState = false;
    digitalWrite(LED_BUILTIN,LOW);
  }

  if(previousState!= touchState){
    Serial.println("STATE CHANGED: TouchState = ");
    Serial.print(touchState);

  previousState = touchState;
  }

  delay(200);
  previousReading = measurementA;
  cycles++;
}

void color(bool touchState){
  uint8_t time = millis() >> 2;
  // uint8_t brightness = map(valueSound, 0, 1023, 0, maxBrightness);
  // Serial.println(brightness);

  if(touchState){
    brightness = 20;
  }else{
    brightness = 1;
  }
  
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


