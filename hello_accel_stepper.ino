#include <AccelStepper.h>

#define EN        8
#define X_DIR     5
#define X_STP     2

AccelStepper stepper(AccelStepper::DRIVER, X_STP, X_DIR);

void setup()
{
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

  stepper.setMaxSpeed(1000); // steps/sec
  stepper.setAcceleration(500); // steps/sec^2 - Amount of steps the speed is adjusted every sec
  stepper.moveTo(3200);
}

void loop()
{
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());

    stepper.run();
}
