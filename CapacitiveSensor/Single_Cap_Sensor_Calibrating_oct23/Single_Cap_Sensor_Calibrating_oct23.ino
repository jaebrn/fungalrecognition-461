//capacitive sensor prototype w/ LED indicator
// supports one cap sensor & what should be an autocalibrating threshold
// currently this is working with USB power grounded & ungrounded and with battery power, but not with wall power

// capacitive sensing includes
#include <CapacitiveSensor.h>

CapacitiveSensor sensorA = CapacitiveSensor(4,2);  // 1M resistor between pins 4 & 2, pin 2 is sensor pin
long previousReading = 0;
int cycles = 0; // count of readings 
bool touchState = false; // false when no touch, true when touched
bool previousState = false;
uint8_t LED = 8; // indicator led (on when touched, off when no touch)

void setup() {
  sensorA.set_CS_AutocaL_Millis(1000); // may want to play with this value 
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);
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
  if(measurementA>10+(previousReading * 2)){
    touchState = true;
    digitalWrite(LED,HIGH);
  }else if(measurementA<(previousReading * 0.5)){
    touchState = false;
    digitalWrite(LED,LOW);
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


