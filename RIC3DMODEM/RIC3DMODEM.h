#ifndef RIC3DMODEM_H
#define RIC3DMODEM_H

#include <Arduino.h>
#include <string.h>
#include <StreamDebugger.h>

class RIC3DMODEM
{
private:
  bool _ATDump;
  bool _Debug;
  Stream *_ModemSerial;
  Stream *_MonitorSerial;
  StreamDebugger *_StreamDebugger;
  bool _SupportPubEx;

public:
  RIC3DMODEM();

  void begin(Stream *modemSerial, Stream *monitorSerial = NULL, bool debug = true, bool atDump = false);
  void turnOn(void);
  void turnOff(void);
  int test();
  int createPDPContext(char *apn, char *username, char *password);
  int activatePDPContext();
  int deactivatePDPContext();
  int setTCPClient(char *IP, char *port);
  int connectMQTTClient(const char *serverHost, int serverPort, const char *userName, const char *password);
  int disconnectMQTTClient();
  int subscribeToTopic();
  int publishData(char *key, char *value);
  int waitForAnswer(char *ans, long timeout = 10000);
  void readRPC();
  int checkMessages();
};

#endif