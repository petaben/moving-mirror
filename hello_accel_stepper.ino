#include <AccelStepper.h>

#define EN        8
#define X_DIR     5
#define X_STP     2

#define FORWARD     9 // limit X
#define BACKWARD     10 // limit Y

AccelStepper stepper(AccelStepper::DRIVER, X_STP, X_DIR);

void setup()
{
  pinMode(FORWARD, INPUT_PULLUP);
  pinMode(BACKWARD, INPUT_PULLUP);

  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

  stepper.setMaxSpeed(3000); // steps/sec
  stepper.setAcceleration(1000); // steps/sec^2 - Amount of steps the speed is adjusted every sec
}

void loop()
{
  if(!digitalRead(FORWARD)){
      stepper.moveTo(10000);
  }
  if(!digitalRead(BACKWARD)){
      stepper.moveTo(0);
  }
    stepper.run();
}
