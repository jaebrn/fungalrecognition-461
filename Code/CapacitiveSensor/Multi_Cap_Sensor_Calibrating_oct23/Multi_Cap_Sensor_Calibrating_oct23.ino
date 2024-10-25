//capacitive sensor prototype w/ LED indicator
// supports one cap sensor & what should be an autocalibrating threshold
// currently this is working with USB power grounded & ungrounded and with battery power, but not with wall power

// capacitive sensing includes
#include <CapacitiveSensor.h>

CapacitiveSensor sensorA = CapacitiveSensor(4,2);  // 1M resistor between pins 4 & 2, pin 2 is sensor pin
CapacitiveSensor sensorB = CapacitiveSensor(12,13);  // 1M resistor between pins 4 & 2, pin 2 is sensor pin
long previousReadingA = 0;
long previousReadingB = 0;
int cycles = 0; // count of readings 
bool touchStateA = false; // false when no touch, true when touched
bool previousStateA = false;
bool touchStateB = false; // false when no touch, true when touched
bool previousStateB = false;
uint8_t LED1 = 8; // sensor A indicator led
uint8_t LED2 = 10; // sensor B indicator led

void setup() {
  sensorA.set_CS_AutocaL_Millis(1000); // may want to play with this value 
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
}

void loop() {
  long measurementA =  sensorA.capacitiveSensor(30); // take measurement
  Serial.println(); //debug
  Serial.print("Sensor A: ");
  Serial.print(measurementA);

  long measurementB =  sensorB.capacitiveSensor(30); // take measurement
  Serial.println(); //debug
  Serial.print("Sensor B: ");
  Serial.print(measurementB);

//periodic recalibration every x cycles
  if(cycles>=50){
    sensorA.set_CS_AutocaL_Millis(1000);
    sensorB.set_CS_AutocaL_Millis(1000);
    cycles = 0; //reset cycles
    Serial.println("recalibrating");
  }

//State change based on relation to previous measurement
  if(measurementA>10+(previousReadingA * 2)){
    touchStateA = true;
    digitalWrite(LED1,HIGH);
  }else if(measurementA<(previousReadingA * 0.5)){
    touchStateA = false;
    digitalWrite(LED1,LOW);
  }

  if(measurementB>10+(previousReadingB * 2)){
    touchStateB = true;
    digitalWrite(LED2,HIGH);
  }else if(measurementB<(previousReadingB * 0.5)){
    touchStateB = false;
    digitalWrite(LED2,LOW);
  }

  if(previousStateA!= touchStateA){
    Serial.println("STATE A CHANGED: TouchState = ");
    Serial.print(touchStateA);

  previousStateA = touchStateA;
  }

  if(previousStateB!= touchStateB){
    Serial.println("STATE B CHANGED: TouchState = ");
    Serial.print(touchStateB);

  previousStateB = touchStateB;
  }

  delay(200);
  previousReadingA = measurementA;
  previousReadingB = measurementB;
  cycles++;
}


