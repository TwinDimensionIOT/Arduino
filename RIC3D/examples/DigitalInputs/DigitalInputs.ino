#include <RIC3D.h>

int DigitalValues[8];

//this initializes the device setting the corresponding inputs and outputs
RIC3D device();

void setup() 
{
  // Initialize Serial port
  Serial.begin(115200);

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
