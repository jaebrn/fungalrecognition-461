/*Fungal Re-Cognition: Affinity with Non-Human Networks 
CONTRIBUTORS: Francis Ouellette, Jaden Thompson, Jennifer Brown, Leonardo Morales Vega, 
Mariam Aoune, Negar Roofigariesfahani
DESCRIPTION: Three Dotstar LED animations are triggered  MPR121 Touch sensor
Source: "How to animate a Dotstar LED", CHATGPT
*/

#include <Particle.h>
/* THESE ARE PARTICLE SPECIFIC PARAMETERS APPLIED AT CODE RUNTIME */
/* RUN ALL PARTICLE CLOUD COMMUNICATION IN SEPARATE THREAD */
SYSTEM_THREAD(ENABLED);
/* HOW TO CONNECT TO WiFi & INTERNET: AUTOMATIC, SEMI_AUTOMATIC, MANUAL */
SYSTEM_MODE(MANUAL);

#include <Adafruit_MPR121.h>
#include <dotstar.h>

#define NUMPIXELS 59 // Number of LEDs in strip
#define BREATH_SPEED 10 // Controls the speed of the breathing effect


#if (PLATFORM_ID == 32) // P2/Photon2
//-------------------------------------------------------------------
// P2/Photon2 must use dedicated SPI Interface API (Hardware SPI/SPI1):
// e.g. Adafruit_DotStar(NUMPIXELS, SPI_INTERFACE, DOTSTAR_BGR);
//-------------------------------------------------------------------
// SPI: MO (data), SCK (clock)
#define SPI_INTERFACE SPI
// SPI1: D2 (data), D4 (clock)
// #define SPI_INTERFACE SPI1
Adafruit_DotStar strip(NUMPIXELS, SPI_INTERFACE, DOTSTAR_BGR);

#else // Argon, Boron, etc..
//-------------------------------------------------------------------
// Here's how to control the LEDs from any two pins (Software SPI):
// e.g. Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
//-------------------------------------------------------------------

//green sticky note: strip2
//pink sticky note: strip
//orange sticky note: strip 3

#define DATAPIN   A5
#define CLOCKPIN  A4

#define DATAPIN2 D7
#define CLOCKPIN2 D6

#define DATAPIN1 D5
#define CLOCKPIN1 D4
//Adafruit_DotStar strip1(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
//Adafruit_DotStar strip2(NUMPIXELS, DATAPIN1, CLOCKPIN1, DOTSTAR_BGR);
Adafruit_DotStar strip(NUMPIXELS, DATAPIN1, CLOCKPIN1, DOTSTAR_BGR);
Adafruit_DotStar strip1(NUMPIXELS, DATAPIN2, CLOCKPIN2, DOTSTAR_BGR);
Adafruit_DotStar strip2(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN2, CLOCKPIN2);

//-------------------------------------------------------------------
// Here's how to control the LEDs from SPI pins (Hardware SPI):
// e.g. Adafruit_DotStar(NUMPIXELS, DOTSTAR_RGB);
//-------------------------------------------------------------------
// Hardware SPI is a little faster, but must be wired to specific pins
// (Core/Photon/P1/Electron = pin A5 for data, A3 for clock)
//Adafruit_DotStar strip(NUMPIXELS, DOTSTAR_BGR);

#endif // #if (PLATFORM_ID == 32)

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

// Timing variables for animation
unsigned long previousMillis[12];  // Track time for each touchpad (12 touchpads)
const long interval = 100;         // Interval between LED movements (ms)
int ledPositions[12];              // Position of the moving LED for each touchpad
bool moving[12];                   // If an LED is currently moving for each pad


void setup() {
 strip.begin();
strip1.begin();
 strip2.begin();

 strip.show();
 strip1.show();
 strip2.show();
  
  
  strip.setBrightness(72);
   strip1.setBrightness(72);
   strip2.setBrightness(72);
 
  
  Serial.begin(115200);
  waitFor(Serial.isConnected, 5000);
  
   Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  //Default address is 0x5A, if tied to 3.3V its 0x5B
  //If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  // Set the initial LED pattern
  for (int i = 0; i < NUMPIXELS; i++) {
 
      strip1.setPixelColor(i, 0, 0, 0); // light is off
      strip2.setPixelColor(i, 0, 0, 0); 
      strip.setPixelColor(i, 0, 0, 0); 
   
  }
   strip.show();
   strip1.show();
   strip2.show();

 
  
}
void loop() {
  // Get the currently touched pads
  currtouched = cap.touched();

  // Check for new touches and start animations
  for (int i = 0; i < 12; i++) { // Iterate over all touchpads
    if (i < 4) { // Touchpads 0 to 5 for strip1
      if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
        Serial.print(i);
        Serial.println(" touched ");

        if (!moving[i]) {
          moving[i] = true;
          ledPositions[i] = 0; // Start animation at the first pixel
        }
      }
    } else if(i >= 4 && i <= 7)  { // Touchpads 6 to 11 for strip2
      if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
        Serial.print(i);
        Serial.println(" touched");

        if (!moving[i]) {
          moving[i] = true;
          ledPositions[i] = 0; // Start animation at the first pixel
        }
      }
    } else if (i >= 8 && i <= 11){
        if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
        Serial.print(i);
        Serial.println(" touched ");

        if (!moving[i]) {
          moving[i] = true;
          ledPositions[i] = 0; // Start animation at the first pixel
        }
      }

    }
  }

for (int i = 8; i <= 11; i++) {
  if (moving[i]) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis[i] >= interval) {
      previousMillis[i] = currentMillis;

      // Gradually update LED brightness
      if (ledPositions[i] > 0) {
        for (int j = ledPositions[i] - 4; j < ledPositions[i]; j++) {
          if (j >= 0) {
            strip.setPixelColor(j, 0, 0, 0); // Dim yellow for previous LEDs
          }
        }
      }

      // Set brightness for current animated pixels
      for (int j = ledPositions[i]; j < ledPositions[i] + 4 && j < NUMPIXELS; j++) {
        if (j == ledPositions[i]) {
          strip.setPixelColor(j, 255, 204, 0); // Bright yellow for the first pixel
        } else {
          strip.setPixelColor(j, 64, 51, 0); // Dim yellow for the following pixels
        }
      }
      strip.show();

      ledPositions[i]++; // Advance the animation

      // Stop animation if it reaches or exceeds the end of the strip
      if (ledPositions[i] >= NUMPIXELS) {
        moving[i] = false; // Mark this animation as complete
      }
    }
  } else if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
    // Restart animation if the touchpad is touched again
    moving[i] = true;
    ledPositions[i] = 0; // Reset animation position
    previousMillis[i] = millis(); // Reset timing
  }
}

for (int i = 0; i < 4; i++) {
  if (moving[i]) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis[i] >= interval) {
      previousMillis[i] = currentMillis;

      // Gradually update LED brightness
      if (ledPositions[i] > 0) {
        for (int j = ledPositions[i] - 4; j < ledPositions[i]; j++) {
          if (j >= 0) {
            strip1.setPixelColor(j, 0, 0, 0); // Dim yellow for previous LEDs
          }
        }
      }

      // Set brightness for current animated pixels
      for (int j = ledPositions[i]; j < ledPositions[i] + 4 && j < NUMPIXELS; j++) {
        if (j == ledPositions[i]) {
          strip1.setPixelColor(j, 255, 204, 0); // Bright yellow for the first pixel
        } else {
          strip1.setPixelColor(j, 64, 51, 0); // Dim yellow for the following pixels
        }
      }
      strip1.show();

      ledPositions[i]++; // Advance the animation

      // Stop animation if it reaches or exceeds the end of the strip
      if (ledPositions[i] >= NUMPIXELS) {
        moving[i] = false; // Mark this animation as complete
      }
    }
  } else if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
    // Restart animation if the touchpad is touched again
    moving[i] = true;
    ledPositions[i] = 0; // Reset animation position
    previousMillis[i] = millis(); // Reset timing
  }
}


  // Process animations for strip2 (touchpads 6 to 11)
  for (int i = 4; i < 8; i++) {
    if (moving[i]) {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis[i] >= interval) {
        previousMillis[i] = currentMillis;

        // Gradually increase brightness of the current yellow pixels
        if (ledPositions[i] > 0) {
          for (int j = ledPositions[i] - 4; j < ledPositions[i]; j++) {
            if (j >= 0) {
              // For the animated pixel, set to bright yellow
              if (j == ledPositions[i] - 1) {
                strip2.setPixelColor(j, 0, 0, 0); // Set current pixel to bright yellow
             
              } else {
                strip2.setPixelColor(j, 0, 0, 0); // Set the non-animated pixels to dim yellow
                //strip3.setPixelColor(j, 64, 51, 0); // Set the non-animated pixels to dim yellow
              }
            }
          }
        }
        // Increase brightness for the next 3 yellow pixels
        for (int j = ledPositions[i]; j < ledPositions[i] + 4 && j < NUMPIXELS; j++) {
          if (j == ledPositions[i]) {
            strip2.setPixelColor(j, 255, 204, 0); // Set the current pixel to bright yellow
          
          } else {
            strip2.setPixelColor(j, 64, 51, 0); // Set following pixels to dim yellow
            //strip3.setPixelColor(j, 64, 51, 0); // Set following pixels to dim yellow
          }
        }
        strip2.show();
        //strip3.show();

        ledPositions[i] += 4; // Advance by 6 pixels

        // Stop animation if it reaches or exceeds the end of the strip
        if (ledPositions[i] >= NUMPIXELS) {
          moving[i] = false; // Mark this animation as complete
        }
      }
    }
  }
  // Save the current touch state for comparison in the next loop
  lasttouched = currtouched;

  delay(10); // Small delay for smooth execution
 }

