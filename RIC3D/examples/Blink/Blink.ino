#include <RIC3D.h>

//this initializes the device setting the corresponding inputs and outputs
RIC3D device();

void setup() {

}


void loop() {
  
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
  digitalWrite(LED4,HIGH);
  delay(1000);
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);
  delay(1000);
  
}
