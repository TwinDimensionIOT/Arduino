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