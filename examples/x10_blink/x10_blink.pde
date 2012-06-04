/*
  X10 blink
  
  Blinks an lamp on an X10 lamp module.  
  Example was built using a PL513
  X10 One-Way Interface Module from http://www.smarthome.com 
  as the modem, and a Powerhouse X10 Lamp Module from Smarthome
  to plug the lamp in.
  
  created 15 June 2007
  by Tom Igoe

  v4 demo, creatrope, uses HOUSE_A instead of A

*/
#include <x10.h>
#include <x10constants.h>

#define zcPin 2
#define dataPin 3

// set up a new x10 instance:
x10 myHouse =  x10(zcPin, dataPin);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Lights on:");
  // send a "lights on" command 3 times:
  myHouse.write(HOUSE_A, ON,3);

  delay(500);
  Serial.println("Lights off:");
  // send a "lights off" command 3 times:
  myHouse.write(HOUSE_A, OFF,3);
  delay(500);
}
