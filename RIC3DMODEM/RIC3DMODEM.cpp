#include "RIC3DMODEM.h"
#include <string.h>

void ModemTurnOn(void)
{
    Serial.println(" Turning Modem ON");
    delay(500);
    pinMode(MODEM_STATUS,INPUT_PULLUP);
    while(digitalRead(MODEM_STATUS))
    {
        pinMode(MODEM_PWRKEY,OUTPUT);
        digitalWrite(MODEM_PWRKEY,LOW);
        delay(1000);
        pinMode(MODEM_PWRKEY,INPUT);
        delay(10000);
    }    
    Serial.println(" Modem is now turned ON");
}

void ModemTurnOff(void)
{

    pinMode(MODEM_STATUS,INPUT_PULLUP);
    while(!digitalRead(MODEM_STATUS))
    {
        Serial.println(" Turning Modem OFF");
        pinMode(MODEM_PWRKEY,OUTPUT);
        digitalWrite(MODEM_PWRKEY,LOW);
        delay(1000);
        pinMode(MODEM_PWRKEY,INPUT);
        delay(2000);
        Serial.println(" Modem is now turned OFF");

    }
}

void ATtest()
{
    Serial3.write("AT\r\n");
    Serial3.flush();
    WaitForAnswer("OK\r\n");
}

void CreatePDPContext(char* apn, char* username, char* password)
{
    char str[60];
    strcpy(str,"AT+QICSGP=1,1,");
    strcat(str,apn);
    strcat(str,",");
    strcat(str,username);
    strcat(str,",");
    strcat(str,password);
    strcat(str,",1\r\n");
    Serial3.write(str);
    Serial3.flush();
    WaitForAnswer("OK\r\n");  
}

void ActivatePDPContext()
{
    Serial3.write("AT+QIACT=1\r\n");
    Serial3.flush();
    WaitForAnswer("OK\r\n");  
}

void SetTCPClient(char* IP, char* port)
{
    char str[60];
    strcpy(str,"AT+QIOPEN=1,0,TCP,");
    strcat(str,IP);
    strcat(str,",");
    strcat(str,port);
    strcat(str,",0");
    strcat(str,",2");
    strcat(str,"\r\n");
    Serial3.write(str);
    Serial3.flush();
    WaitForAnswer("OK\r\n");  
}


void ConnectMQTTClient(char* client_id)
{
    Serial3.write("AT+QMTCFG=pdpcid,0,1\r\n");
    Serial3.flush();
    WaitForAnswer("OK\r\n");
    Serial3.write("AT+QMTCFG=recv/mode,0,1,0\r\n");
    Serial3.flush();
    WaitForAnswer("OK\r\n");    
    Serial3.write("AT+QMTOPEN=0,tdata.tesacom.net,1883\r\n");
    Serial3.flush();
    WaitForAnswer("QMTOPEN: 0,0\r\n");
    char str[80];
    strcpy(str,"AT+QMTCONN=0,client,");
    strcat(str,client_id);
    strcat(str,"\r\n");
    Serial3.write(str);
    Serial3.flush();
    WaitForAnswer("QMTCONN: 0,0,0");
}  

void SubscribeToTopic()
{
    Serial3.write("AT+QMTSUB=0,1,v1/devices/me/rpc/request/+,0\r\n");
    Serial3.flush();
    WaitForAnswer("QMTSUB: 0");
}

void PublishData(char* key, char* value)
{
    char strpayloadlen[5];
    int payloadlen = 9 + strlen(key) + strlen(value);
    // the last parameter defines the maximum data to send (maximum value 1500)
    Serial3.write("AT+QMTPUBEX=0,0,0,0,v1/devices/me/telemetry,");
    Serial3.write(itoa(payloadlen, strpayloadlen,10));
    Serial3.write("\r\n");
    Serial3.flush();
    WaitForAnswer(">");
    Serial3.write("{\"");
    Serial3.write(key);
    Serial3.write("\":\"");
    Serial3.write(value);
    Serial3.write("\"}\r\n");
    Serial3.flush();
    WaitForAnswer("QMTPUBEX: 0,0,0");  

    //dtostrf(temp, 4, 2, strtemp);
    //strcat(str,strtemp);
}

void WaitForAnswer(char* ans)
{
    char buffer[64];
    char str[2];
    str[1] = 0;
    char* ret;
    char* err;
    int done = 0;
    strcpy(buffer,"");
    while(done == 0) 
        {
            if (Serial3.available())
            {
                str[0] = Serial3.read();
                strcat(buffer,str);
                ret = strstr(buffer,ans);
                err = strstr(buffer,"ERROR");

            }
            if(ret != NULL || err != NULL)
            {
                done = 1;
                Serial.write(buffer);
                Serial.flush();
            }
/*             if(err != NULL)
            {
                done = 1;
                Serial.write(buffer);
                Serial.flush();
            } */
        } 
}

int CheckMessages()
{
    Serial3.write("AT+QMTRECV?\r\n");
    Serial3.flush();
    char buffer[64];
    char str[2];
    int recmessages = 0;
    str[1] = 0;
    char* ret;
    char* ok;
    char aux;
    int done = 0;
    strcpy(buffer,"");
    while(done == 0) 
        {
            if (Serial3.available())
            {
                str[0] = Serial3.read();
                strcat(buffer,str);
                ok = strstr(buffer,"OK");

            }
            if(ok != NULL)
            {
                ret = strstr(buffer,"QMTRECV:");
                for(int i = 0; i<5 ; i++)
                {
                    aux = ret[2 * i+9];
                    recmessages += int(aux)-48;
                } 
                done = 1;
                //Serial.write(buffer);
                //Serial.flush();
            }
        }
    return recmessages;
}

void ReadRPC()
{
    int recmessages;
    bool relay0state;
    bool relay1state;
    recmessages = CheckMessages();

    if(recmessages > 0)
    {
        for(int i = 0; i<5 ; i++)
        {
        Serial3.write("AT+QMTRECV=0,");
        char aux = char(i+48);
        Serial3.write(aux);
        Serial3.write("\r\n");
        Serial3.flush();
        char buffer[120];
        char str[2];
        str[1] = 0;
        char* ret;
        char* ok;
        int done = 0;
        strcpy(buffer,"");
        while(done == 0) 
            {
                if (Serial3.available())
                {
                    str[0] = Serial3.read();
                    strcat(buffer,str);
                    ok = strstr(buffer,"OK");
                }
                if(ok != NULL)
                {
                    ret = strstr(buffer,"Rele0");
                    if(ret != NULL)
                    {
                        if (ret[16] == 't')
                            relay0state = 1;
                        else  
                            relay0state = 0;      
                    }
                    ret = strstr(buffer,"Rele1");
                    if(ret != NULL)
                    {
                        if (ret[16] == 't')
                            relay1state = 1;
                        else  
                            relay1state = 0;      
                    }
                    done = 1;
                    Serial.write(buffer);
                    Serial.flush();
                }
            } 
        }   
    digitalWrite(DO0,relay0state);
    digitalWrite(DO1,relay1state);
    }
}


Conf::Conf(int di,int ai,int t,int v)
{
    this->conf_di = di;
    this->conf_ai = ai;
    this->conf_t = t;
    this->conf_v = v;
}

RIC3DMODEM::RIC3DMODEM(Conf config):configuration(config)
{
  this->configuration = config;
  init();
}



RIC3DMODEM::PublishAll()
{
    ReadAll();
    if(this->configuration.conf_di)
    {
        char* str = malloc(7);
        for(int i = 0; i<8 ; i++)
        {
            PublishData(Dict[i],itoa(this->DigitalValues[i], str, 2));
        }
        free(str);
    }
    if(this->configuration.conf_ai)
    {
        char* str = malloc(7);
        for(int i = 0; i<4 ; i++)
        {
            dtostrf(this->AnalogValues[i]/40, 4, 2, str);
            PublishData(Dict[i+8],str);
        }
        free(str);
    }
    if(this->configuration.conf_t)
    {
        char* str = malloc(7);
        dtostrf(this->Temperature, 4, 2, str);
        PublishData("Temperatura",str);
        free(str);
    }
    if(this->configuration.conf_v)
    {
        char* str = malloc(7);
        dtostrf(this->Voltage, 4, 2, str);
        PublishData("Tension",str);
        free(str);
    }
}

