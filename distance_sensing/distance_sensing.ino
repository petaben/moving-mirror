#include <RunningMedian.h>

#define TRIG_PIN 4
#define ECHO_L_PIN 2
#define ECHO_C_PIN 3
#define ECHO_R_PIN 18

long lStart = 0;
float lDuration = 0;
float lDistance = 0;

long cStart = 0;
float cDuration = 0;
float cDistance = 0;

long rStart = 0;
float rDuration = 0;
float rDistance = 0;

int minDistance = 0;

RunningMedian lSamples = RunningMedian(10);
RunningMedian cSamples = RunningMedian(10);
RunningMedian rSamples = RunningMedian(10);
 
void setup() {
  Serial.begin(115200); // debug outputs
  Serial2.begin(115200); // send distance to LED control (as string)
  Serial3.begin(115200); // send distance to motor control (as byte)
 
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_L_PIN, INPUT);
  pinMode(ECHO_C_PIN, INPUT);
  pinMode(ECHO_R_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ECHO_L_PIN), lEcho, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ECHO_C_PIN), cEcho, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ECHO_R_PIN), rEcho, CHANGE);
}
 
void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIG_PIN, LOW);

  minDistance = min(min(lSamples.getMedian(),cSamples.getMedian()),rSamples.getMedian());

  Serial.print("  min: "); Serial.print(minDistance);
  Serial.print("  L: "); Serial.print(lSamples.getMedian());
  Serial.print("  C: "); Serial.print(cSamples.getMedian());
  Serial.print("  R: "); Serial.print(rSamples.getMedian());
  Serial.println("");

  Serial3.println(minDistance);
  sendDistanceByte(minDistance);

  delay(100);
}

void lEcho(){
  if(digitalRead(ECHO_L_PIN) == HIGH){
    lStart = micros();
  }else{
    lDuration = micros() - lStart;
    lDistance = (lDuration / 2) * 0.343;
    lSamples.add(lDistance);
  }
}

void cEcho(){
  if(digitalRead(ECHO_C_PIN) == HIGH){
    cStart = micros();
  }else{
    cDuration = micros() - cStart;
    cDistance = (cDuration / 2) * 0.343;
    cSamples.add(cDistance);
  }
}

void rEcho(){
  if(digitalRead(ECHO_R_PIN) == HIGH){
    rStart = micros();
  }else{
    rDuration = micros() - rStart;
    rDistance = (rDuration / 2) * 0.343;
    rSamples.add(rDistance);
  }
}

void sendDistanceByte(int distance){
  byte distanceAsByte;
  if(distance > 2550){
    distanceAsByte = 255;
  }else{
    distanceAsByte = distance / 10;
  }
  Serial2.write(distanceAsByte);
  Serial.print("Byte: "); Serial.println(distanceAsByte);
}
