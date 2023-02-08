#ifndef RIC3DMODEM_H
#define RIC3DMODEM_H

#include <Arduino.h>
#include <RIC3D.h>

const char Dict[][4] = {"DI0","DI1","DI2","DI3","DI4","DI5","DI6","DI7","AI0","AI1","AI2","AI3"};

void ModemTurnOn(void);

void ModemTurnOff(void);

void ATtest();

void CreatePDPContext(char* apn, char* username, char* password);

void ActivatePDPContext();

void SetTCPClient(char* IP, char* port);

void ConnectMQTTClient();

void SubscribeToTopic();

void PublishData(char* key, char* value);

void WaitForAnswer(char* ans);

int ReadRPC();

class Conf
{
    public:
    int conf_di;
    int conf_ai;
    int conf_t;
    int conf_v;
    Conf(int di, int ai, int t, int v);
};

class RIC3DMODEM: public RIC3D
{
    private:
    Conf configuration;

    public:

    RIC3DMODEM(Conf);

    PublishAll();
};

#endif