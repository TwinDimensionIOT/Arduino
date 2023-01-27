#ifndef RIC3D_H
#define RIC3D_H

#include <Arduino.h>

#define LED1 47
#define LED2 48
#define LED3 42
#define LED4 43

#define DI0 22
#define DI1 23
#define DI2 24
#define DI3 25
#define DI4 A8
#define DI5 A9
#define DI6 A10
#define DI7 A11

#define DO0 77
#define DO1 12
#define DO0FB 13
#define DO1FB 11

#define AI0 A12
#define AI1 A13
#define AI2 A14
#define AI3 A15

#define MODEM_STATUS 30
#define MODEM_PWRKEY 31
#define MODEM_EMERG_OFF 32

#define SIM_SELECT 9

#define VCCSENSE A0
#define TEMPSENSOR A1

void TurnOnLeds(bool ledstate1,bool ledstate2, bool ledstate3 ,bool ledstate4);

void SetLedState(bool ledstate, int lednumber);

class RIC3D
{
    protected:
    int DigitalValues[8];
    int AnalogValues[4];
    float Temperature;
    float Voltage;

    public:

    RIC3D();

    void init();

    void ReadDigitalInputs();
    void ReadAnalogInputs();
    void ReadTemperature();
    void ReadVoltage();

    void ReadAll();

};


#endif