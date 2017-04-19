/*
 X10 multi
 
 Turns on and off AC appliances. Example was built using a PL513
 X10 One-Way Interface Module from http://www.smarthome.com 
 as the modem, and two Powerhouse X10 Lamp Module from Smarthome
 to plug the lamps in.
 
 Set module 1 to house code A, unit code 1, and module 2
 to house code A, unit code 2.
 
 created 17 June 2007
 by Tom Igoe

 v4 demo, creatrope, uses HOUSE_A instead of A

 */

// include the X10 library files:
#include <x10.h>
#include <x10constants.h>

#define zcPin 2         // the zero crossing detect pin
#define dataPin 3       // the X10 data out pin
#define repeatTimes 3   // how many times each X10 message should repeat
// in an electrically noisy environment, you  
// can set this higher.


// set up a new x10  library instance:
x10 myHouse;

void setup() {
  // begin serial:
  Serial.begin(57600);
  myHouse.init(zcPin, dataPin);
  Serial.println(myHouse.version());
  // Turn off all lights:
  myHouse.write(HOUSE_A, ALL_UNITS_OFF,repeatTimes);
}

void loop() {
  // Turn on first module:
  myHouse.write(HOUSE_A, UNIT_1,repeatTimes);               
  myHouse.write(HOUSE_A, ON,repeatTimes);  
  // Turn off second module:   
  myHouse.write(HOUSE_A, UNIT_2,repeatTimes);            
  myHouse.write(HOUSE_A, OFF,repeatTimes);
  delay(500);
  // Turn off first module: 
  myHouse.write(HOUSE_A, UNIT_1,repeatTimes);
  myHouse.write(HOUSE_A, OFF,repeatTimes);
  // turn on second module:
  myHouse.write(HOUSE_A, UNIT_2,repeatTimes);              
  myHouse.write(HOUSE_A, ON,repeatTimes);
  delay(500);

}
