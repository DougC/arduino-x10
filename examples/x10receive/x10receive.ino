/* Arduino Interface to the PSC05 X10 Receiver.                       BroHogan 3/24/09
 * SETUP: X10 PSC05/TW523 RJ11 to Arduino (timing for 60Hz)
 * - RJ11 pin 1 (BLK) -> Pin 2 (Interrupt 0) = Zero Crossing
 * - RJ11 pin 2 (RED) -> GND
 * - RJ11 pin 3 (GRN) -> Pin 4 = Arduino receive
 * - RJ11 pin 4 (YEL) -> Pin 5 = Arduino transmit (via X10 Lib)
 * NOTES:
 * - Must detach interrup when transmitting with X10 Lib
 */

#include "Arduino.h"                  // this is needed to compile with Rel. 0013
#include <x10.h>                       // X10 lib is used for transmitting X10
#include <x10constants.h>              // X10 Lib constants
#define RPT_SEND 2

#define ZCROSS_PIN     2               // BLK pin 1 of PSC05
#define RCVE_PIN       4               // GRN pin 3 of PSC05
#define TRANS_PIN      5               // YEL pin 4 of PSC05
#define LED_PIN        13              // for testing

x10 SX10= x10(ZCROSS_PIN,TRANS_PIN,RCVE_PIN,LED_PIN);// set up a x10 library instance:

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.print("x10 receive/send test");
}

// A simple test program that demonstrates integrated send/receive
// prints X10 input, then get P1  on/off if unit code on input was 1
void loop(){
SX10.writeXTBIIR(16,STATUS_REQUEST);
delay(500);
SX10.debug();                       // print out the received command
SX10.reset();
 delay(1000);
}



