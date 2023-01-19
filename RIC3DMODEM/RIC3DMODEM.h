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

void PublishData(char* key, char* value);

void WaitForAnswer(char* ans);

#endif