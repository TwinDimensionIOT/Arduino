#include <RIC3D.h>
#include <RIC3DMODEM.h>

RIC3D device();

#define SerialMon Serial

#define SerialAT Serial3

// Module baud rate
uint32_t rate = 115200; 

// Select SIM Card (0 = right, 1 = left)
bool sim_selected = 1; 

const char apn[]      = "internet.gprs.unifon.com.ar";
const char gprsUser[] = "wap";
const char gprsPass[] = "wap";

const char mqtt_ip[] = "tdata.tesacom.net";
const char mqtt_port[] = "1883";

const char user_name[] = "l5CQih08cPmpXfptvMBI"; // aca se debe poner el id del device de tdata

int sensorValue = 0;
float temperature = 0;
float voltage = 0;
char str[7];

int interval = 10000;
long old_millis = 0;

void setup() 
{
  SerialMon.begin(115200);
  SerialMon.println(F("***********************************************************"));
  SerialMon.println(F(" Initializing Modem"));
  pinMode(SIM_SELECT,OUTPUT);
  digitalWrite(SIM_SELECT,sim_selected);
  SerialMon.print(" Sim selected is the one on the ");
  SerialMon.println(sim_selected?"left":"right");
  ModemBegin(&SerialAT,&SerialMon);
  ModemTurnOff();
  ModemTurnOn();
  SerialAT.begin(rate);
  analogReference(INTERNAL2V56);
  SerialMon.println(" Opening MQTT service ");
  if(CreatePDPContext(apn, gprsUser,  gprsPass))
  {
    SerialMon.println("Error creating PDP context");
  }
  ActivatePDPContext();
  if(ConnectMQTTClient(user_name,mqtt_ip,mqtt_port))
  {
    SerialMon.println("Error connecting to tdata");
  }
}

void loop() 
{
  if((millis() - old_millis) > interval)
  {
    old_millis = millis();
    sensorValue = analogRead(TEMPSENSOR);
    temperature = (float(sensorValue)*25/100)-60;
    sensorValue = analogRead(VCCSENSE);
    voltage = (float(sensorValue)*(25*11)/10000);
    dtostrf(temperature, 4, 2, str);
    PublishData("Temperatura",str);
    dtostrf(voltage, 4, 2, str);
    PublishData("Tension",str);
  }
}