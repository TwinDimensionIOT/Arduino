#include <RIC3D.h>

//this initializes the device setting the corresponding inputs and outputs
RIC3D device();

void setup() 
{
  // Initialize Serial port
  Serial.begin(115200);
  
}


void loop() 
{
  // Close the relays
  digitalWrite(DO0, HIGH);   
  digitalWrite(DO1, HIGH);   
  delay(3000); 
  // Read the state of the relays
  Serial.print(digitalRead(DO0FB));
  Serial.print("  ");   
  Serial.print(digitalRead(DO1FB)); 
  Serial.print("  ");      
  delay(3000); 
  // Open the relays
  digitalWrite(DO0, LOW);
  digitalWrite(DO1, LOW);    
  delay(3000);  
  // Read the state of the relays
  Serial.print(digitalRead(DO0FB));      
  Serial.print("  ");                  
  Serial.println(digitalRead(DO1FB));  
  delay(3000);
}
