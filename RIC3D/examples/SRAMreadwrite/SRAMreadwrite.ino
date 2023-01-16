 
#include <SPI.h>
#include <sram.h>
#include <HardwareSerial.h>

SRAM ram(8); // chip select of the sram

uint16_t addr = 0;
uint8_t value = 0x0A;
uint8_t res = 0;

void setup()
{
  // Set the chip select of the Flash to high, and write protect of the sram to high
  pinMode(53,OUTPUT);
  digitalWrite(53,HIGH);
  DDRH |= B00000100;
  PORTH |= B00000100;
  ram.begin();
  Serial.begin(115200);
}

void loop()
{
  ram.writeByte(addr, value++);  // Write address
  res = ram.readByte(addr++);    // Read address
  Serial.println(res, HEX);
  
  delay(2000);
}
