#include <AccelStepper.h>

#define EN        8
#define X_DIR     5
#define X_STP     2

#define FORWARD     10 // limit X
#define BACKWARD     9 // limit Y

#define LIMIT_PIN     11 // limit Z

#define SPEED_PIN    A0

char status = 'S'; // S:Stopped, F:Forward, B:Backward

AccelStepper stepper(AccelStepper::DRIVER, X_STP, X_DIR);

void setup()
{
  Serial.begin(115200);
   
  pinMode(FORWARD, INPUT_PULLUP);
  pinMode(BACKWARD, INPUT_PULLUP);

  pinMode(SPEED_PIN, INPUT);
  pinMode(LIMIT_PIN, INPUT_PULLUP);

  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

  stepper.setAcceleration(1500); // steps/sec^2 - Amount of steps the speed is adjusted every sec
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
  stepper.setCurrentPosition(0);
  Serial.println("Homing complete");
}

void readSpeed(){
  int raw_speed = analogRead(SPEED_PIN);
  int speed = map(raw_speed, 0, 1024, 1000, 200);
  stepper.setMaxSpeed(speed); // steps/sec
  Serial.print("Speed: ");
  Serial.println(speed);
}

void loop()
{
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

void stopped(){
  if(isPressed(FORWARD)){
      stepper.moveTo(10000);
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
