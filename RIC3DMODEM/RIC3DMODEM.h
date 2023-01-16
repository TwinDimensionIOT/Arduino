#ifndef RIC3DMODEM_H
#define RIC3DMODEM_H

#include <Arduino.h>
#include <RIC3D.h>

void ModemTurnOn(void);

void ModemTurnOff(void);

void ATtest();

void CreatePDPContext(char* apn, char* username, char* password);

void ActivatePDPContext();

void SetTCPClient(char* IP, char* port);

void ConnectMQTTClient();

void PublishData(float temp);

void WaitForAnswer(char* ans);

#endif