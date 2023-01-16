#include <RIC3D.h>

int sensorValue = 0;
double temperature = 0;

void setup() {
  // Set internal reference of the adc to 2,56V
  analogReference(INTERNAL2V56);
  // Initialize Serial port
  Serial.begin(115200);
}

void loop() {

  // Read the sensor and compute the temperature value
  sensorValue = analogRead(TEMPSENSOR);
  temperature = (float(sensorValue)*25/100)-60;

  // Print the sensor value and the temperature
  Serial.print("sensor = ");
  Serial.println(sensorValue);
  Serial.print("temperature = ");
  Serial.println(temperature);

  delay(1000);
}
