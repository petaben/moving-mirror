#include <AccelStepper.h>

#define EN        8
#define X_DIR     5
#define X_STP     2

#define FORWARD     9 // limit X
#define BACKWARD     10 // limit Y

#define MAX_SPEED 2//rotations/s
#define ROTATION_STEPS 3200 // 400*8

char status = 'S'; // S:Stopped, F:Forward, B:Backward

AccelStepper stepper(AccelStepper::DRIVER, X_STP, X_DIR);

void setup()
{
  pinMode(FORWARD, INPUT_PULLUP);
  pinMode(BACKWARD, INPUT_PULLUP);

  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

  stepper.setMaxSpeed(5000); // steps/sec
  stepper.setAcceleration(4000); // steps/sec^2 - Amount of steps the speed is adjusted every sec
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
