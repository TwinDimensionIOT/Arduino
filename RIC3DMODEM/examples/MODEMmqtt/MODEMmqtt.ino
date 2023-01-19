#include <RIC3D.h>
#include <RIC3DMODEM.h>

#define SerialMon Serial

#define SerialAT Serial3

// Module baud rate
uint32_t rate = 115200; 

// Select SIM Card (0 = right, 1 = left)
bool sim_selected = 1; 

const char apn[]      = "internet.gprs.unifon.com.ar";
const char gprsUser[] = "wap";
const char gprsPass[] = "wap";

int sensorValue = 0;
float temperature = 0;
float voltage = 0;
char str[7];

int DigitalValues[8];

void setup() 
{
  pinMode(DI0,INPUT_PULLUP);
  pinMode(DI1,INPUT_PULLUP);
  pinMode(DI2,INPUT_PULLUP);
  pinMode(DI3,INPUT_PULLUP);
  pinMode(DI4,INPUT_PULLUP);
  pinMode(DI5,INPUT_PULLUP);
  pinMode(DI6,INPUT_PULLUP);
  pinMode(DI7,INPUT_PULLUP);


  SerialMon.begin(115200);
  SerialMon.println(F("***********************************************************"));
  SerialMon.println(F(" Initializing Modem"));
  pinMode(SIM_SELECT,OUTPUT);
  digitalWrite(SIM_SELECT,sim_selected);
  SerialMon.print(" Sim selected is the one on the ");
  SerialMon.println(sim_selected?"left":"right");
  ModemTurnOff();
  ModemTurnOn();
  SerialAT.begin(rate);
  analogReference(INTERNAL2V56);

  SerialMon.println(" Opening MQTT service ");
  CreatePDPContext(apn, gprsUser,  gprsPass);
  ActivatePDPContext();
  ConnectMQTTClient();
}

void loop() 
{
    delay(10000);
    sensorValue = analogRead(TEMPSENSOR);
    temperature = (float(sensorValue)*25/100)-60;
    sensorValue = analogRead(VCCSENSE);
    voltage = (float(sensorValue)*(25*11)/10000);
    dtostrf(temperature, 4, 2, str);
    PublishData("Temperatura",str);
    dtostrf(voltage, 4, 2, str);
    PublishData("Tension",str);
    DigitalValues[0] = digitalRead(DI0);
    DigitalValues[1] = digitalRead(DI1);
    DigitalValues[2] = digitalRead(DI2);
    DigitalValues[3] = digitalRead(DI3);
    DigitalValues[4] = digitalRead(DI4);
    DigitalValues[5] = digitalRead(DI5);
    DigitalValues[6] = digitalRead(DI6);
    DigitalValues[7] = digitalRead(DI7);
    for(int i = 0; i<8 ; i++)
    {
    PublishData(Dict[i],itoa(DigitalValues[i], str, 2));
    }
}