#include "RIC3D.h"
#include "RIC3DMODEM.h"

//-----------------------------------------------------------------------------
RIC3DMODEM::RIC3DMODEM()
{
  _SupportPubEx = false;
}
//-----------------------------------------------------------------------------
void RIC3DMODEM::begin(Stream *modemSerial, Stream *monitorSerial, bool debug, bool atDump)
{
  _Debug = monitorSerial && debug;
  _ATDump = _Debug && atDump;
  if (_Debug)
  {
    _MonitorSerial = monitorSerial;
  }
  if (_ATDump){
    _StreamDebugger = new StreamDebugger(*modemSerial, *monitorSerial);
    _ModemSerial = _StreamDebugger;
  } else
    _ModemSerial = modemSerial;
}
//-----------------------------------------------------------------------------
void RIC3DMODEM::turnOn(void)
{
  if (_Debug)
    _MonitorSerial->println(F("ModemTurnOn"));
  delay(500);
  pinMode(MODEM_STATUS, INPUT_PULLUP);
  while (digitalRead(MODEM_STATUS))
  {
    pinMode(MODEM_PWRKEY, OUTPUT);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    pinMode(MODEM_PWRKEY, INPUT);
    delay(10000);
  }
  if (_Debug)
    _MonitorSerial->println(F(" Modem is now turned ON"));
}
//-----------------------------------------------------------------------------
void RIC3DMODEM::turnOff(void)
{
  if (_Debug)
    _MonitorSerial->println(F("ModemTurnOff"));
  pinMode(MODEM_STATUS, INPUT_PULLUP);
  while (!digitalRead(MODEM_STATUS))
  {
    if (_Debug)
      _MonitorSerial->println(F(" Turning Modem OFF"));
    pinMode(MODEM_PWRKEY, OUTPUT);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    pinMode(MODEM_PWRKEY, INPUT);
    delay(2000);
  }
  if (_Debug)
    _MonitorSerial->println(F(" Modem is now turned OFF"));
}
//-----------------------------------------------------------------------------
int RIC3DMODEM::test()
{
  if (_Debug)
    _MonitorSerial->println(F("ATtest"));
  _ModemSerial->print("AT\r\n");
  _ModemSerial->flush();
  return waitForAnswer("OK\r\n");
}
//-----------------------------------------------------------------------------
int RIC3DMODEM::createPDPContext(char *apn, char *username, char *password)
{
  if (_Debug)
    _MonitorSerial->println(F("CreatePDPContext"));
  _ModemSerial->print(F("AT+QICSGP=1,1,\""));
  _ModemSerial->print(apn);
  _ModemSerial->print(F("\",\""));
  _ModemSerial->print(username);
  _ModemSerial->print(F("\",\""));
  _ModemSerial->print(password);
  _ModemSerial->print(F("\",1\r\n"));
  _ModemSerial->flush();
  return waitForAnswer("OK\r\n");
}
//-----------------------------------------------------------------------------
int RIC3DMODEM::activatePDPContext()
{
  if (_Debug)
    _MonitorSerial->println(F("ActivatePDPContext"));
  _ModemSerial->print(F("AT+QIACT=1\r\n"));
  _ModemSerial->flush();
  return waitForAnswer("OK\r\n");
}
//-----------------------------------------------------------------------------
int RIC3DMODEM::deactivatePDPContext()
{
  if (_Debug)
    _MonitorSerial->println(F("DeactivatePDPContext"));
  _ModemSerial->print(F("AT+QIDEACT=1\r\n"));
  _ModemSerial->flush();
  return waitForAnswer("OK\r\n");
}
//-----------------------------------------------------------------------------
int RIC3DMODEM::setTCPClient(char *IP, char *port)
{
  if (_Debug)
    _MonitorSerial->println(F("SetTCPClient"));
  _ModemSerial->print(F("AT+QIOPEN=1,0,TCP,"));
  _ModemSerial->print(IP);
  _ModemSerial->print(F(","));
  _ModemSerial->print(port);
  _ModemSerial->print(F(",0,2\r\n"));
  _ModemSerial->flush();
  return waitForAnswer("OK\r\n");
}
//-----------------------------------------------------------------------------
int RIC3DMODEM::connectMQTTClient(const char *serverHost, int serverPort, const char *userName, const char *password)
{
  int result;
  if (_Debug)
    _MonitorSerial->println(F("ConnectMQTTClient"));

  _ModemSerial->print(F("AT+QMTPUBEX=?\r\n"));
  _ModemSerial->flush();
  _SupportPubEx = waitForAnswer("OK\r\n") == 0;

  if (_SupportPubEx)
  {
    _ModemSerial->print("AT+QMTCFG=\"pdpcid\",0,1\r\n");
    _ModemSerial->flush();
    if (result = waitForAnswer("OK\r\n"))
      return result;
    _ModemSerial->print("AT+QMTCFG=\"recv/mode\",0,1,0\r\n");
    _ModemSerial->flush();
    if (result = waitForAnswer("OK\r\n"))
      return result;
  }

  _ModemSerial->print(F("AT+QMTOPEN=0,\""));
  _ModemSerial->print(serverHost);
  _ModemSerial->print(F("\","));
  _ModemSerial->print(serverPort);
  _ModemSerial->print(F("\r\n"));
  _ModemSerial->flush();
  if (result = waitForAnswer("QMTOPEN: 0,0\r\n"))
    return result;

  _ModemSerial->print(F("AT+QMTCONN=0,\"client\",\""));
  _ModemSerial->print(userName);
  if (password)
  {
    _ModemSerial->print(F("\",\""));
    _ModemSerial->print(password);
  }
  _ModemSerial->print(F("\"\r\n"));
  _ModemSerial->flush();
  if (result = waitForAnswer("QMTCONN: 0,0,0"))
    return result;

  return result;
}
//-----------------------------------------------------------------------------
int RIC3DMODEM::disconnectMQTTClient()
{
  _ModemSerial->print(F("AT+QMTDISC=0\r\n"));
  _ModemSerial->flush();
  return waitForAnswer("OK");
}
//-----------------------------------------------------------------------------
int RIC3DMODEM::subscribeToTopic()
{
  if (_Debug)
    _MonitorSerial->println(F("SubscribeToTopic"));
  int result;
  _ModemSerial->print("AT+QMTSUB=0,1,\"v1/devices/me/rpc/request/+\",0\r\n");
  _ModemSerial->flush();
  if (result = waitForAnswer("QMTSUB: 0"))
    return result;
  return 1;
}
//-----------------------------------------------------------------------------
int RIC3DMODEM::publishData(char *key, char *value)
{
  int result;
  if (_SupportPubEx)
  {
    char strpayloadlen[16];
    int payloadlen = 9 + strlen(key) + strlen(value);
    // the last parameter defines the maximum data to send (maximum value 1500)
    _ModemSerial->print(F("AT+QMTPUBEX=0,0,0,0,\"v1/devices/me/telemetry\","));
    _ModemSerial->print(itoa(payloadlen, strpayloadlen, 10));
    _ModemSerial->print(F("\r\n"));
    _ModemSerial->flush();
    if (result = waitForAnswer(">"))
      return result;
    _ModemSerial->print(F("{\""));
    _ModemSerial->print(key);
    _ModemSerial->print(F("\":\""));
    _ModemSerial->print(value);
    _ModemSerial->print(F("\"}\r\n"));
    _ModemSerial->flush();
    if (result = waitForAnswer("QMTPUBEX: 0,0,0"))
      return result;
  }
  else
  {
    _ModemSerial->print(F("AT+QMTPUB=0,0,0,0,\"v1/devices/me/telemetry\"\r\n"));
    _ModemSerial->flush();
    if (result = waitForAnswer(">"))
      return result;
    _ModemSerial->print(F("{\""));
    _ModemSerial->print(key);
    _ModemSerial->print(F("\":\""));
    _ModemSerial->print(value);
    _ModemSerial->print(F("\"}"));
    _ModemSerial->print(F("\x1A"));
    _ModemSerial->flush();
    if (result = waitForAnswer("+QMTPUB: 0,0,0"))
      return result;
  }
  return result;
}
//-----------------------------------------------------------------------------
int RIC3DMODEM::waitForAnswer(char *ans, long timeout = 10000)
{
  char buffer[129];
  int buffer_len = 0;
  char chr;
  long millis_i = millis();
  while (millis() - millis_i < timeout)
  {
    while (_ModemSerial->available())
    {
      chr = buffer[buffer_len++] = _ModemSerial->read();
      buffer[buffer_len] = 0;
      buffer_len &= 127;
      if (strstr(buffer, ans))
        return 0;
      if (strstr(buffer, "ERROR"))
        return 1;
    }
    delay(0);
  }
  return 2;
}
//-----------------------------------------------------------------------------
int RIC3DMODEM::checkMessages()
{
  _ModemSerial->print("AT+QMTRECV?\r\n");
  _ModemSerial->flush();
  char buffer[64];
  char str[2];
  int recmessages = 0;
  str[1] = 0;
  char *ret;
  char *ok;
  char aux;
  int done = 0;
  strcpy(buffer, "");
  while (done == 0)
  {
    if (_ModemSerial->available())
    {
      str[0] = _ModemSerial->read();
      strcat(buffer, str);
      ok = strstr(buffer, "OK");
    }
    if (ok != NULL)
    {
      ret = strstr(buffer, "QMTRECV:");
      for (int i = 0; i < 5; i++)
      {
        aux = ret[2 * i + 9];
        recmessages += int(aux) - 48;
      }
      done = 1;
      // gMonitorSerial->write(buffer);
      // gMonitorSerial->flush();
    }
  }
  return recmessages;
}
//-----------------------------------------------------------------------------
void RIC3DMODEM::readRPC()
{
  int recmessages;
  bool relay0state;
  bool relay1state;
  recmessages = checkMessages();

  if (recmessages > 0)
  {
    for (int i = 0; i < 5; i++)
    {
      _ModemSerial->print("AT+QMTRECV=0,");
      char aux = char(i + 48);
      _ModemSerial->print(aux);
      _ModemSerial->print("\r\n");
      _ModemSerial->flush();
      char buffer[150];
      char str[2];
      str[1] = 0;
      char *ret;
      char *ok;
      int done = 0;
      strcpy(buffer, "");
      while (done == 0)
      {
        if (_ModemSerial->available())
        {
          str[0] = _ModemSerial->read();
          strcat(buffer, str);
          ok = strstr(buffer, "OK");
        }
        if (ok != NULL)
        {
          ret = strstr(buffer, "Rele0");
          if (ret != NULL)
          {
            if (ret[16] == 't')
              relay0state = 1;
            else
              relay0state = 0;
          }
          ret = strstr(buffer, "Rele1");
          if (ret != NULL)
          {
            if (ret[16] == 't')
              relay1state = 1;
            else
              relay1state = 0;
          }
          done = 1;
          _MonitorSerial->write(buffer);
          _MonitorSerial->flush();
        }
      }
    }
    digitalWrite(DO0, relay0state);
    if (_Debug)
    {
      _MonitorSerial->print("\n relay 0:");
      _MonitorSerial->println(relay0state);
    }
    digitalWrite(DO1, relay1state);
    if (_Debug)
    {
      _MonitorSerial->print("\n rely 1:");
      _MonitorSerial->println(relay1state);
    }
  }
}
//-----------------------------------------------------------------------------
