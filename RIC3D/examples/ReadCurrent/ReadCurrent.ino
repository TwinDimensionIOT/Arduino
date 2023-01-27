#include <RIC3D.h>

int sensorValue[4];
double current[4];

void setup() {
  // Set internal reference of the adc to 2,56V
  analogReference(INTERNAL2V56);
  // Initialize Serial port
  Serial.begin(115200);
}

void loop() {

  // Read the sensor and compute current at each input
  sensorValue[0] = analogRead(AI0);
  sensorValue[1] = analogRead(AI1);
  sensorValue[2] = analogRead(AI2);
  sensorValue[3] = analogRead(AI3);
  for(int i = 0; i<4 ; i++)
  {
    current[i] = float(sensorValue[i])/40;
  }
  // Print the current in each analog input
  Serial.print("Current = ");
  for(int i = 0; i<4 ; i++)
  {
    Serial.print(current[i]);
    Serial.print("   ");
  }
  Serial.print("\n");
  delay(1000);
}
