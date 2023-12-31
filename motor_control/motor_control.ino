#include <AccelStepper.h>

#define EN        8
#define DIR_PIN     5
#define STP_PIN     2

#define FORWARD     10 // limit X
#define BACKWARD     9 // limit Y

#define LIMIT_PIN     11 // limit Z

#define MODE_PIN  A2 //Resume

#define SPEED_PIN    A0

#define FRONT_POSITION  2150 //steps

char status = 'S'; // S:Stopped, F:Forward, B:Backward
int speed;
int distance;

AccelStepper stepper(AccelStepper::DRIVER, STP_PIN, DIR_PIN);

void setup()
{
  Serial.begin(115200);
   
  pinMode(FORWARD, INPUT_PULLUP);
  pinMode(BACKWARD, INPUT_PULLUP);

  pinMode(SPEED_PIN, INPUT);
  pinMode(LIMIT_PIN, INPUT_PULLUP);
  pinMode(MODE_PIN, INPUT_PULLUP);

  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

  stepper.setAcceleration(700); // steps/sec^2 - Amount of steps the speed is adjusted every sec
  home();

  readSpeed();
}

void home(){
  Serial.println("Start homing");
  stepper.setMaxSpeed(200);
  stepper.moveTo(-10000);
  bool limitReached = !digitalRead(LIMIT_PIN);
  while(!limitReached){
    stepper.run();
    limitReached = !digitalRead(LIMIT_PIN);
  }
  for(int i=0;i<500;i++){ // just go a bit further to make sure all corners get to the back
    stepper.run();
    delay(1);
  }
  stepper.setCurrentPosition(0);
  Serial.println("Homing complete");
}

void readSpeed(){
  int raw_speed = analogRead(SPEED_PIN);
  speed = map(raw_speed, 0, 1024, 1000, 100);
  stepper.setMaxSpeed(speed); // steps/sec
  Serial.print("Speed: ");
  Serial.println(speed);
}

void loop()
{
  if(digitalRead(MODE_PIN)){
    if(isPressed(FORWARD) || isPressed(BACKWARD)){
      autoMode();
    }else{
      distanceMode();
    }
  }else{
    manualMode();
  }
}

void autoMode(){
  Serial.println("Auto mode");
  stepper.setAcceleration(speed/10);
  while(true){
    stepper.moveTo(FRONT_POSITION);
    while(stepper.currentPosition() != FRONT_POSITION){
      stepper.run();
    }
    stepper.moveTo(0);
    while(stepper.currentPosition() != 0){
      stepper.run();
    }
  }
}

void distanceMode(){
  Serial.println("Distance mode");
  while(true){
    handleDistance();
    stepper.run();
  }
}

void handleDistance(){
  if (Serial.available() > 0) {
    distance = Serial.read();
    Serial.print("Received distance: ");
    Serial.println(distance);
    if(distance < 35){
      distance = 35;
    }
    int targetPosition = map(distance, 35, 255, 0, FRONT_POSITION);
    int delta = abs(targetPosition-stepper.currentPosition());
    if(delta > 50){
      stepper.moveTo(targetPosition);
    }
  }
}

//mirror is moved using buttons
void manualMode(){
  Serial.println("Manual mode");
  while(true){
    switch(status){
      case 'S':
        stopped();
        break;
      case 'F':
        forward();
        break;
      case 'B':
        backward();
        break;
    }
  }
}

void stopped(){
  if(isPressed(FORWARD)){
      stepper.moveTo(FRONT_POSITION);
      status = 'F';
  }
  if(isPressed(BACKWARD)){
      stepper.moveTo(0);
      status = 'B';
  }
  stepper.run();
}

void forward(){
  if(!isPressed(FORWARD)){
      stepper.stop();
      status = 'S';
  }
  stepper.run();
}

void backward(){
  if(!isPressed(BACKWARD)){
      stepper.stop();
      status = 'S';
  }
  stepper.run();
}

bool isPressed(int pin){
  return !digitalRead(pin);
}
