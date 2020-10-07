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
#define SET_ACCELERATION 300.0
AccelStepper stepper(1, STEP_PIN_C, DIRECTION_PIN_C);
float newSpeed;
float maxSpeed = 3000.0;
bool newDataBit, runAllowed = false,adding=false;
char commandChar;
long currentPosition;
long int steps = 0, mainNewStep, addedNewStep,memMainStep;
void checkSerial();
void checkRunning();
void mainProgram(long currentPositionValue,float maxSpeedValue,long stepValue);
void addingProgram(long currentPositionValue,float maxSpeedValue,long stepValue);
void stopMotor();
void runMotor();
void setup()
{
  Serial.begin(9600);
  pinMode(ENABLE_PIN_C, OUTPUT);
  digitalWrite(ENABLE_PIN_C, HIGH);
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
  if(!stepper.isRunning() && adding == true){ //İLAVE YAPMAYI BİTİRDİM
    mainProgram(currentPosition,newSpeed,memMainStep);
  }
  if (runAllowed == true) //B VEYA C KOMUTLARI GELMİŞ
  {
    if (stepper.distanceToGo() == 0) //YOLU TAMAMLADIM
    {
      stopMotor();
      checkSerial();
    }
    else //YOLU HENÜZ TAMAMLAMADIM
    {
      runMotor();
      checkSerial();
    }
  }  
}
void checkSerial()
{
  if (Serial.available())
  {
    newDataBit = true;
    //B C D 
    commandChar = Serial.read();
  }
  if (newDataBit == true) //YENİ VERİ GELDİ
  {
    newSpeed = Serial.parseFloat(); //HizTxt
    mainNewStep = Serial.parseInt(); //ExtraStepAnaTxt
    addedNewStep = Serial.parseInt(); //ExtraStepIlaveTxt
    Serial.println(addedNewStep);
    memMainStep = mainNewStep + memMainStep;
    if (commandChar == 'B') //ANA PROGRAM
    {
      runAllowed = true;
      adding = false; //İLAVE İŞLEMİ YAPMIYORUM
      mainProgram(stepper.currentPosition(),newSpeed,mainNewStep); //memMainStep
    }
    if (commandChar == 'C') //İLAVE PROGRAM
    {
      runAllowed = true;
      currentPosition = stepper.currentPosition(); //ANA PROGRAMA KALDIĞI YERDEN DEVAM ETSİN DİYE
      addingProgram(0,maxSpeed,addedNewStep);
      adding = true; //İLAVE İŞLEMİ YAPIYORUM   
    }
    if (commandChar == 'D') //DUR
    {
      runAllowed = false; 
      stopMotor();
    }
    newDataBit = false;
  }
}

void mainProgram(long currentPositionValue,float maxSpeedValue,long stepValue)
{ 
  if (stepper.distanceToGo() == 0) //YOLUMU TAMAMLADIM
  {
    adding = false; 
    steps = stepValue;
    stepper.setCurrentPosition(currentPositionValue);
    //stepper.setSpeed(0);
    stepper.setMaxSpeed(maxSpeedValue);
    stepper.moveTo(steps);
  }
  else
  {
    steps = stepValue + steps;
    stepper.setCurrentPosition(currentPositionValue);
    //stepper.setSpeed(0);
    stepper.setMaxSpeed(newSpeed);
    stepper.moveTo(steps);
  }
}
void addingProgram(long currentPositionValue,float maxSpeedValue,long stepValue){
  stepper.setCurrentPosition(currentPositionValue);
  stepper.setMaxSpeed(maxSpeedValue);
  stepper.moveTo(stepValue); //move du
}

void runMotor(){
  digitalWrite(ENABLE_PIN_C, LOW);
  stepper.run();
}
void stopMotor(){
  memMainStep = 0;
  stepper.setCurrentPosition(0);
  digitalWrite(ENABLE_PIN_C, HIGH);
  stepper.stop();
}
