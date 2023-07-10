#include "RIC3DMODEM.h"
#include <string.h>

Stream* gModemSerial;
Stream* gMonitorSerial;
bool gSupportPubEx;

void ModemBegin(Stream* modemSerial, Stream* monitorSerial) {
  gModemSerial = modemSerial;
  gMonitorSerial = monitorSerial;
}

void ModemTurnOn(void) {
  gMonitorSerial->println(F("ModemTurnOn"));
  delay(500);
  pinMode(MODEM_STATUS, INPUT_PULLUP);
  while (digitalRead(MODEM_STATUS)) {
    pinMode(MODEM_PWRKEY, OUTPUT);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    pinMode(MODEM_PWRKEY, INPUT);
    delay(10000);
  }
  gMonitorSerial->println(F(" Modem is now turned ON"));
}

void ModemTurnOff(void) {
  gMonitorSerial->println(F("ModemTurnOff"));
  pinMode(MODEM_STATUS, INPUT_PULLUP);
  while (!digitalRead(MODEM_STATUS)) {
    gMonitorSerial->println(F(" Turning Modem OFF"));
    pinMode(MODEM_PWRKEY, OUTPUT);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    pinMode(MODEM_PWRKEY, INPUT);
    delay(2000);
    gMonitorSerial->println(F(" Modem is now turned OFF"));
  }
}

int ATtest() {
  gMonitorSerial->println(F("ATtest"));
  gModemSerial->print("AT\r\n");
  gModemSerial->flush();
  return WaitForAnswer("OK\r\n");
}

int CreatePDPContext(char* apn, char* username, char* password) {
  gMonitorSerial->println(F("CreatePDPContext"));
  gModemSerial->print(F("AT+QICSGP=1,1,\""));
  gModemSerial->print(apn);
  gModemSerial->print(F("\",\""));
  gModemSerial->print(username);
  gModemSerial->print(F("\",\""));
  gModemSerial->print(password);
  gModemSerial->print(F("\",1\r\n"));
  gModemSerial->flush();
  return WaitForAnswer("OK\r\n");
}

int ActivatePDPContext() {
  gMonitorSerial->println(F("ActivatePDPContext"));
  gModemSerial->print(F("AT+QIACT=1\r\n"));
  gModemSerial->flush();
  return WaitForAnswer("OK\r\n");
}

int SetTCPClient(char* IP, char* port) {
  gMonitorSerial->println(F("SetTCPClient"));
  gModemSerial->print(F("AT+QIOPEN=1,0,TCP,"));
  gModemSerial->print(IP);
  gModemSerial->print(F(","));
  gModemSerial->print(port);
  gModemSerial->print(F(",0,2\r\n"));
  gModemSerial->flush();
  return WaitForAnswer("OK\r\n");
}


int ConnectMQTTClient(char* user_name, char* mqtt_ip, char* mqtt_port) {
  gMonitorSerial->println(F("ConnectMQTTClient"));
  int result;

  gModemSerial->print(F("AT+QMTPUBEX=?\r\n"));
  gModemSerial->flush();
  gSupportPubEx = WaitForAnswer("OK\r\n") == 0;

  if (gSupportPubEx) {
    gModemSerial->print("AT+QMTCFG=\"pdpcid\",0,1\r\n");
    gModemSerial->flush();
    if (result = WaitForAnswer("OK\r\n")) return result;
    gModemSerial->print("AT+QMTCFG=\"recv/mode\",0,1,0\r\n");
    gModemSerial->flush();
    if (result = WaitForAnswer("OK\r\n")) return result;
  }

  gModemSerial->print(F("AT+QMTOPEN=0,\""));
  gModemSerial->print(mqtt_ip);
  gModemSerial->print(F("\","));
  gModemSerial->print(mqtt_port);
  gModemSerial->print(F("\r\n"));
  gModemSerial->flush();
  if (result = WaitForAnswer("QMTOPEN: 0,0\r\n")) return result;

  gModemSerial->print(F("AT+QMTCONN=0,\"client\",\""));
  gModemSerial->print(user_name);
  gModemSerial->print(F("\"\r\n"));
  gModemSerial->flush();
  if (result = WaitForAnswer("QMTCONN: 0,0,0")) return result;

  return result;
}

int DisconnectMQTTClient() {
  gModemSerial->print(F("AT+QMTDISC=0\r\n"));
  gModemSerial->flush();
  return WaitForAnswer("OK");
}

int SubscribeToTopic() {
  gMonitorSerial->println(F("SubscribeToTopic"));
  int result;
  gModemSerial->print("AT+QMTSUB=0,1,\"v1/devices/me/rpc/request/+\",0\r\n");
  gModemSerial->flush();
  if (result = WaitForAnswer("QMTSUB: 0")) return result;
  return 1;
}

int PublishData(char* key, char* value) {
  int result;
  if (gSupportPubEx) {
    char strpayloadlen[16];
    int payloadlen = 9 + strlen(key) + strlen(value);
    // the last parameter defines the maximum data to send (maximum value 1500)
    gModemSerial->print(F("AT+QMTPUBEX=0,0,0,0,\"v1/devices/me/telemetry\","));
    gModemSerial->print(itoa(payloadlen, strpayloadlen, 10));
    gModemSerial->print(F("\r\n"));
    gModemSerial->flush();
    if (result = WaitForAnswer(">")) return result;
    gModemSerial->print(F("{\""));
    gModemSerial->print(key);
    gModemSerial->print(F("\":\""));
    gModemSerial->print(value);
    gModemSerial->print(F("\"}\r\n"));
    gModemSerial->flush();
    if (result = WaitForAnswer("QMTPUBEX: 0,0,0")) return result;
  } else {
    gModemSerial->print(F("AT+QMTPUB=0,0,0,0,\"v1/devices/me/telemetry\"\r\n"));
    gModemSerial->flush();
    if (result = WaitForAnswer(">")) return result;
    gModemSerial->print(F("{\""));
    gModemSerial->print(key);
    gModemSerial->print(F("\":\""));
    gModemSerial->print(value);
    gModemSerial->print(F("\"}"));
    gModemSerial->print(F("\x1A"));
    gModemSerial->flush();
    if (result = WaitForAnswer("+QMTPUB: 0,0,0")) return result;
  }
  return result;
}

int WaitForAnswer(char* ans, long timeout = 10000) {
  char buffer[129];
  int buffer_len = 0;
  char chr;
  long millis_i = millis();
  while (millis() - millis_i < timeout) {
    while (gModemSerial->available()) {
      chr = buffer[buffer_len++] = gModemSerial->read();
      buffer[buffer_len] = 0;
      buffer_len &= 127;
      if (strstr(buffer, ans)) return 0;
      if (strstr(buffer, "ERROR")) return 1;
    }
    delay(0);
  }
  return 2;
}

int CheckMessages() {
  gModemSerial->print("AT+QMTRECV?\r\n");
  gModemSerial->flush();
  char buffer[64];
  char str[2];
  int recmessages = 0;
  str[1] = 0;
  char* ret;
  char* ok;
  char aux;
  int done = 0;
  strcpy(buffer, "");
  while (done == 0) {
    if (gModemSerial->available()) {
      str[0] = gModemSerial->read();
      strcat(buffer, str);
      ok = strstr(buffer, "OK");
    }
    if (ok != NULL) {
      ret = strstr(buffer, "QMTRECV:");
      for (int i = 0; i < 5; i++) {
        aux = ret[2 * i + 9];
        recmessages += int(aux) - 48;
      }
      done = 1;
      //gMonitorSerial->write(buffer);
      //gMonitorSerial->flush();
    }
  }
  return recmessages;
}

void ReadRPC() {
  int recmessages;
  bool relay0state;
  bool relay1state;
  recmessages = CheckMessages();

  if (recmessages > 0) {
    for (int i = 0; i < 5; i++) {
      gModemSerial->print("AT+QMTRECV=0,");
      char aux = char(i + 48);
      gModemSerial->print(aux);
      gModemSerial->print("\r\n");
      gModemSerial->flush();
      char buffer[150];
      char str[2];
      str[1] = 0;
      char* ret;
      char* ok;
      int done = 0;
      strcpy(buffer, "");
      while (done == 0) {
        if (gModemSerial->available()) {
          str[0] = gModemSerial->read();
          strcat(buffer, str);
          ok = strstr(buffer, "OK");
        }
        if (ok != NULL) {
          ret = strstr(buffer, "Rele0");
          if (ret != NULL) {
            if (ret[16] == 't')
              relay0state = 1;
            else
              relay0state = 0;
          }
          ret = strstr(buffer, "Rele1");
          if (ret != NULL) {
            if (ret[16] == 't')
              relay1state = 1;
            else
              relay1state = 0;
          }
          done = 1;
          gMonitorSerial->write(buffer);
          gMonitorSerial->flush();
        }
      }
    }
    digitalWrite(DO0, relay0state);
    gMonitorSerial->print("\n relay 0:");
    gMonitorSerial->println(relay0state);
    digitalWrite(DO1, relay1state);
    gMonitorSerial->print("\n rely 1:");
    gMonitorSerial->println(relay1state);
  }
}


Conf::Conf(int di, int ai, int t, int v) {
  this->conf_di = di;
  this->conf_ai = ai;
  this->conf_t = t;
  this->conf_v = v;
}

RIC3DMODEM::RIC3DMODEM(Conf config)
  : configuration(config) {
  this->configuration = config;
  init();
}



RIC3DMODEM::PublishAll() {
  ReadAll();
  if (this->configuration.conf_di) {
    char* str = malloc(7);
    for (int i = 0; i < 8; i++) {
      PublishData(Dict[i], itoa(this->DigitalValues[i], str, 2));
    }
    free(str);
  }
  if (this->configuration.conf_ai) {
    char* str = malloc(7);
    for (int i = 0; i < 4; i++) {
      dtostrf(this->AnalogValues[i] / 40, 4, 2, str);
      PublishData(Dict[i + 8], str);
    }
    free(str);
  }
  if (this->configuration.conf_t) {
    char* str = malloc(7);
    dtostrf(this->Temperature, 4, 2, str);
    PublishData("Temperatura", str);
    free(str);
  }
  if (this->configuration.conf_v) {
    char* str = malloc(7);
    dtostrf(this->Voltage, 4, 2, str);
    PublishData("Tension", str);
    free(str);
  }
}
