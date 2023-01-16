#include <RIC3D.h>

int DigitalValues[8];

void setup() 
{
  // Initialize Serial port
  Serial.begin(115200);

  // Set Digital inputs as pull-ups
  pinMode(DI0,INPUT_PULLUP);
  pinMode(DI1,INPUT_PULLUP);
  pinMode(DI2,INPUT_PULLUP);
  pinMode(DI3,INPUT_PULLUP);
  pinMode(DI4,INPUT_PULLUP);
  pinMode(DI5,INPUT_PULLUP);
  pinMode(DI6,INPUT_PULLUP);
  pinMode(DI7,INPUT_PULLUP);
}

void loop() 
{
  // Read the inputs
  DigitalValues[0] = digitalRead(DI0);
  DigitalValues[1] = digitalRead(DI1);
  DigitalValues[2] = digitalRead(DI2);
  DigitalValues[3] = digitalRead(DI3);
  DigitalValues[4] = digitalRead(DI4);
  DigitalValues[5] = digitalRead(DI5);
  DigitalValues[6] = digitalRead(DI6);
  DigitalValues[7] = digitalRead(DI7);

  // Print the values over Serial Port
  Serial.print("Inputs = ");
  for(int i = 0; i<8 ; i++)
  {
    Serial.print("DI");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(DigitalValues[i]);
    Serial.print("   ");
  }

  Serial.print("\n");   
  delay(1000);
}
