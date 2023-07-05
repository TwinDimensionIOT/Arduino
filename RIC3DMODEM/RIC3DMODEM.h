#ifndef RIC3DMODEM_H
#define RIC3DMODEM_H

#include <Arduino.h>
#include <RIC3D.h>

const char Dict[][4] = { "DI0", "DI1", "DI2", "DI3", "DI4", "DI5", "DI6", "DI7", "AI0", "AI1", "AI2", "AI3" };

void ModemBegin(Stream *modemSerial = &Serial3, Stream *monitorSerial = &Serial);

void ModemTurnOn(void);

void ModemTurnOff(void);

int ATtest();

int CreatePDPContext(char* apn, char* username, char* password);

int ActivatePDPContext();

int SetTCPClient(char* IP, char* port);

//void ConnectMQTTClient(char* client_id);
int ConnectMQTTClient(char* user_name, char* mqtt_ip, char* mqtt_port);

int DisconnectMQTTClient();

int SubscribeToTopic();

int PublishData(char* key, char* value);

int WaitForAnswer(char* ans, long timeout = 10000);

void ReadRPC();

int CheckMessages();

class Conf {
public:
  int conf_di;
  int conf_ai;
  int conf_t;
  int conf_v;
  Conf(int di, int ai, int t, int v);
};

class RIC3DMODEM : public RIC3D {
private:
  Conf configuration;

public:

  bool DebugMode;

  RIC3DMODEM(Conf);

  PublishAll();
};

#endif