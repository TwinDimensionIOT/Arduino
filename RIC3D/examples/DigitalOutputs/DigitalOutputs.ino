#include <RIC3D.h>


void setup() 
{
  // Initialize Serial port
  Serial.begin(115200);

  // Set the inputs and outputs acordingly
  pinMode(DO0, OUTPUT);
  pinMode(DO0FB, INPUT_PULLUP);
  pinMode(DO1, OUTPUT);
  pinMode(DO1FB, INPUT_PULLUP);

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
