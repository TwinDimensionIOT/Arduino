#include "RIC3D.h"


void TurnOnLeds(bool ledstate1,bool ledstate2, bool ledstate3 ,bool ledstate4)
{
  digitalWrite(LED1, ledstate1);
  digitalWrite(LED2, ledstate2);
  digitalWrite(LED3, ledstate3);
  digitalWrite(LED4, ledstate4);
}

void SetLedState(bool ledstate, int lednumber)
{
  switch (lednumber)
  {
  case 0:
    digitalWrite(LED1, ledstate);
    break;
  case 1:
    digitalWrite(LED2, ledstate);
    break;
  case 2:
    digitalWrite(LED3, ledstate);
    break;
  case 3:
    digitalWrite(LED4, ledstate);
    break;

  default:
    break;
  }
}

RIC3D::RIC3D()
{
  init();
}

void RIC3D::init()
{
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);  
  pinMode(DI0,INPUT_PULLUP);
  pinMode(DI1,INPUT_PULLUP);
  pinMode(DI2,INPUT_PULLUP);
  pinMode(DI3,INPUT_PULLUP);
  pinMode(DI4,INPUT_PULLUP);
  pinMode(DI5,INPUT_PULLUP);
  pinMode(DI6,INPUT_PULLUP);
  pinMode(DI7,INPUT_PULLUP);
  pinMode(DO0, OUTPUT);
  pinMode(DO0FB, INPUT_PULLUP);
  pinMode(DO1, OUTPUT);
  pinMode(DO1FB, INPUT_PULLUP);
}

void RIC3D::ReadDigitalInputs()
{
  this->DigitalValues[0] = digitalRead(DI0);
  this->DigitalValues[1] = digitalRead(DI1);
  this->DigitalValues[2] = digitalRead(DI2);
  this->DigitalValues[3] = digitalRead(DI3);
  this->DigitalValues[4] = digitalRead(DI4);
  this->DigitalValues[5] = digitalRead(DI5);
  this->DigitalValues[6] = digitalRead(DI6);
  this->DigitalValues[7] = digitalRead(DI7);
}
void RIC3D::ReadAnalogInputs()
{
  this->AnalogValues[0] = analogRead(AI0);
  this->AnalogValues[1] = analogRead(AI1);
  this->AnalogValues[2] = analogRead(AI2);
  this->AnalogValues[3] = analogRead(AI3);
}

void RIC3D::ReadTemperature()
{
  this->Temperature = (float(analogRead(TEMPSENSOR))*25/100)-60;
}

void RIC3D::ReadVoltage()
{
  this->Voltage = (float(analogRead(VCCSENSE))*(25*11)/10000);
}

void RIC3D::ReadAll()
{
  ReadDigitalInputs();
  ReadAnalogInputs();
  ReadTemperature();
  ReadVoltage();
}