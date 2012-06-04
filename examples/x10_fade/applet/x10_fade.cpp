#include "WProgram.h"
#include "x10.h"
#include "x10constants.h"
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
  // send a "lights BRIGHT" command 19 times:
  myHouse.write(A, BRIGHT,19);

  delay(500);
  Serial.println("Lights down:");
  // send a "lights DIM" command 19 times:
  myHouse.write(A, DIM,19);
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

