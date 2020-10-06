#include <AccelStepper.h>
#include <stdio.h>
#define STEP_PIN_A 25
#define STEP_PIN_B 22
#define STEP_PIN_C 31
#define STEP_PIN_D 48
#define STEP_PIN_E 49
#define DIRECTION_PIN_A 26
#define DIRECTION_PIN_B 23
#define DIRECTION_PIN_C 32
#define DIRECTION_PIN_D 50
#define DIRECTION_PIN_E 51
#define ENABLE_PIN_A 27
#define ENABLE_PIN_B 24
#define ENABLE_PIN_C 33
#define ENABLE_PIN_D 52
#define ENABLE_PIN_E 53
#define SET_ACCELERATION 200.0
AccelStepper stepper(1, STEP_PIN_C, DIRECTION_PIN_C);
float newSpeed;
bool newDataBit = false;
long int steps = 0, newStep;
void checkSerial();
void checkRunning();
void setup()
{
  Serial.begin(9600);
  pinMode(ENABLE_PIN_C, OUTPUT);
  digitalWrite(ENABLE_PIN_C, LOW);
  stepper.setCurrentPosition(0);             //initial value
  stepper.setMaxSpeed(0.0);                  //initial value
  stepper.setAcceleration(SET_ACCELERATION); //initial value
}

void loop()
{
  checkRunning();
  checkSerial();
}
void checkRunning()
{
  if (stepper.distanceToGo() == 0)
  {
    digitalWrite(ENABLE_PIN_C, HIGH);
    stepper.stop();
    checkSerial();
  }
  else
  {
    digitalWrite(ENABLE_PIN_C, LOW);
    stepper.run();
    checkSerial();
  }
}
void checkSerial()
{
  if (Serial.available())
  {
    newDataBit = true;
    newSpeed = Serial.parseFloat();
    newStep = Serial.parseInt();
    if (newDataBit == true)
    {
      if (stepper.distanceToGo() == 0)
      {
        steps = newStep;
        stepper.setCurrentPosition(0.0);
        stepper.setSpeed(0);
        stepper.setMaxSpeed(newSpeed);
        stepper.moveTo(steps);
        Serial.println(stepper.currentPosition());
      }
      else
      {
        steps = newStep + steps;
        stepper.setCurrentPosition(stepper.currentPosition());
        stepper.setSpeed(0);
        stepper.setMaxSpeed(newSpeed);
        stepper.moveTo(steps);
        Serial.println(stepper.currentPosition());
      }
      newDataBit = false;
    }
  }
}
