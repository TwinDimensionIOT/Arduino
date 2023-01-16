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

const char server[]   = "143.198.79.3";
const int  port       = "41150";

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

  SerialMon.println(" Setting up TCP client service in transparent mode");
  CreatePDPContext(apn, gprsUser,  gprsPass);
  ActivatePDPContext();
  SetTCPClient(server, port);
}

void loop() 
{
    
    if (SerialAT.available()) 
    {
      SerialMon.write(SerialAT.read());
    }
}

      
