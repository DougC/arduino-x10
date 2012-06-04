#include "WProgram.h"
#include "x10.h"
#include "x10constants.h"
/*
  X10 dimmer
 
 Dims and brightens an incandescent lamp on an X10
 lamp module.  Example was built using a PL513
 X10 One-Way Interface Module from http://www.smarthome.com 
 as the modem, and a Powerhouse X10 Lamp Module from Smarthome
 to plug the lamp in.
 
 created 15 June 2007
 by Tom Igoe
 */


#define zcPin 2
#define dataPin 3

// set up a new x10 instance:
x10 myHouse =  x10(zcPin, dataPin);

void setup() {
  Serial.begin(9600);
  // send a "Lights ON" command 3 times:
  myHouse.write(A, ON,3);
}

void loop() {
  Serial.println("Lights up:");
  // send a "lights BRIGHT" command 19 times.
  // it takes 19 BRIGHT or DIM commands to get
  // an incandescent lamp dim or bright.
  myHouse.write(A, ON,3);
  //delay(50);
  myHouse.write(B, OFF,3);
  delay(500);
  Serial.println("Lights down:");
  // send a "lights DIM" command 19 times:
  myHouse.write(A, OFF,3);
 // delay(50);
  myHouse.write(B, ON,3);
  delay(500);
}
int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

