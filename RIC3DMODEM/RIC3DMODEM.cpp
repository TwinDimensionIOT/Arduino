
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


void ConnectMQTTClient()
{

    Serial3.write("AT+QMTCFG=pdpcid,0,1\r\n");
    Serial3.flush();
    WaitForAnswer("OK\r\n");    
    Serial3.write("AT+QMTOPEN=0,tdata.tesacom.net,1883\r\n");
    Serial3.flush();
    WaitForAnswer("QMTOPEN: 0,0\r\n");
    Serial3.write("AT+QMTCONN=0,client,l5CQih08cPmpXfptvMBI\r\n");
    Serial3.flush();
    WaitForAnswer("QMTCONN: 0,0,0");
}  


void PublishData(float temp)
{
    char* str = malloc(30);
    char* strtemp = malloc(10);
    Serial3.write("AT+QMTPUBEX=0,0,0,0,v1/devices/me/telemetry,25\r\n");
    Serial3.flush();
    WaitForAnswer(">");
    strcpy(str,"{\"Temperatura\":\"");
    dtostrf(temp, 4, 2, strtemp);
    strcat(str,strtemp);
    strcat(str,"\"}\r\n");
    Serial3.write(str);
    Serial3.flush();
    WaitForAnswer("QMTPUBEX: 0,0,0");  
    free(str);
    free(strtemp);
}

void WaitForAnswer(char* ans)
{
    char buffer[64];
    char str[2];
    str[1] = 0;
    char* ret;
    int done = 0;
    strcpy(buffer,"");
    while(done == 0) 
        {
            if (Serial3.available())
            {
                str[0] = Serial3.read();
                strcat(buffer,str);
                ret = strstr(buffer,ans);
            }
            if(ret != NULL)
            {
                done = 1;
                Serial.write(buffer);
                Serial.flush();
            }
        } 
}