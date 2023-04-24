#include <RIC3D.h>
#include <RIC3DMODEM.h>
#include <AsyncDelay.h>


#define SerialMon Serial

#define SerialAT Serial3

// Module baud rate
uint32_t rate = 115200;  

AsyncDelay readInterval;

// Select SIM Card (0 = right, 1 = left)
bool sim_selected = 1; 

const char apn[]      = "internet.gprs.unifon.com.ar";
const char gprsUser[] = "wap";
const char gprsPass[] = "wap";

const char client[]   = "l5CQih08cPmpXfptvMBI";

Conf protocol(0,0,1,1);

RIC3DMODEM device(protocol);

void setup() 
{

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
  ConnectMQTTClient(client);
  SubscribeToTopic();

  readInterval.start(1000, AsyncDelay::MILLIS);
}

void loop() 
{
  if (readInterval.isExpired()) 
  {
    readInterval.restart();
    ReadRPC();  
  }
  if(!digitalRead(MODEM_STATUS))
  {
    if (SerialAT.available()) {
      SerialMon.write(SerialAT.read());
    }
    if (SerialMon.available()) {
      SerialAT.write(SerialMon.read());
    }
  }
}
