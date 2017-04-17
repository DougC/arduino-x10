/*
  X10 dimmer
  
  Dims and brightens an incandescent lamp on an X10
  lamp module.  Example was built using a PL513
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
x10 myHouse;

void setup() {
	Serial.begin(57600);
	myHouse.init(zcPin, dataPin);
	Serial.println(myHouse.version());
	// send a "Lights ON" command 3 times:
	myHouse.write(HOUSE_A, ON,3);
}

void loop() {
  Serial.println("Lights up:");
  // send a "lights BRIGHT" command 19 times.
  // it takes 19 BRIGHT or DIM commands to get
  // an incandescent lamp dim or bright.
  myHouse.write(HOUSE_A, BRIGHT,19);

  delay(500);
  Serial.println("Lights down:");
  // send a "lights DIM" command 19 times:
  myHouse.write(HOUSE_A, DIM,19);
  delay(500);
}
