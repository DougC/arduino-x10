/*
  x10.cpp - X10 transmission library for Arduino version 0.4
  
  Original library				(0.1) by Tom Igoe.
  Timing bug fixes				(0.2) "   "   "
  #include bug fixes for 0012	                (0.3) "   "   "
  Integrates brohogan.blogspot.com X10 receive code from Arduino Playground (0.4)
    
  Zero crossing algorithms borrowed from David Mellis' shiftOut command
  for Arduino.
  
  The circuits can be found at http://www.arduino.cc/en/Tutorial/x10
 
*/

#include <stdlib.h>
#include "Arduino.h"
#include "x10.h"
#include "x10constants.h"
#include "PSC05.h"

volatile unsigned long mask;           // MSB first - bit 12 - bit 0
volatile unsigned int X10BitCnt; // counts bit sequence in frame
volatile unsigned int ZCrossCnt;   // counts Z crossings in frame
volatile unsigned long rcveBuff;       // holds the 13 bits received in a frame
volatile boolean X10rcvd;      // true if a new frame has been received
boolean _newX10;         // both the unit frame and the command frame received
byte _houseCode,_unitCode,_cmndCode;   // 
byte _hc,_uc;
byte startCode;   
byte zcross_pin;
byte rcve_pin;
byte led_pin;

x10 *object;

//static void Check_Rcvr();
//static void Parse_Frame();

void x10_Check_Rcvr_wrapper() {
   object->Check_Rcvr();
}

void x10_Parse_Frame_wrapper() {
   object->Parse_Frame();
}



void x10::init(int zeroCrossingPin, int dataPin, int rp, int led)
{
  led_pin=led;
  rcve_pin = rp;


  if (led_pin>0) { pinMode(led_pin, OUTPUT); }

  this->zeroCrossingPin = zeroCrossingPin;      // the zero crossing pin
  this->dataPin = dataPin;        		// the output data pin
  
  // Set I/O modes:
  pinMode(this->zeroCrossingPin, INPUT);
  pinMode(this->dataPin, OUTPUT);
  digitalWrite(this->zeroCrossingPin, HIGH);      // set 20K pullup (low active signal)
  
  // for receive
  zcross_pin = zeroCrossingPin;

  if (rp>0) {
    pinMode(rcve_pin,INPUT);             // receive X10 commands - low = 1
    pinMode(zcross_pin,INPUT);           // zero crossing - 60 Hz square wave
    digitalWrite(rcve_pin, HIGH);        // set 20K pullup (low active signal)
    attachInterrupt(0,x10_Check_Rcvr_wrapper,CHANGE);// (pin 2) trigger zero cross

    X10BitCnt=0; // counts bit sequence in frame
    ZCrossCnt=0;   // counts Z crossings in frame
     X10rcvd=false;      // true if a new frame has been received
     _newX10=false;         // both the unit frame and the command frame received
   }
}

x10::x10(int zeroCrossingPin, int dataPin, int rp, int led)
{
   init(zeroCrossingPin,dataPin,rp,led);
}
x10::x10(int zeroCrossingPin, int dataPin, int rp)
{
   init(zeroCrossingPin,dataPin,rp,0);
}
x10::x10(int zeroCrossingPin, int dataPin)
{
   init(zeroCrossingPin,dataPin,0,0);
}
/*
	Writes an X10 command out to the X10 modem
*/
void x10::write(byte houseCode, byte numberCode, int numRepeats) {
  byte startCode = B1110; 		// every X10 command starts with this
  if (rcve_pin>0) { detachInterrupt(0); }
  // repeat as many times as requested:
  for (int i = 0; i < numRepeats; i++) {
  	// send the three parts of the command:
  	sendBits(startCode, 4, true);	
    	sendBits(houseCode, 4, false);
    	sendBits(numberCode, 5, false);
    }
    // if this isn't a bright or dim command, it should be followed by
    // a delay of 3 power cycles (or 6 zero crossings):
    if ((numberCode != BRIGHT) && (numberCode != DIM)) {
    	waitForZeroCross(this->zeroCrossingPin, 6);
    }
  if (rcve_pin>0) { attachInterrupt(0,x10_Check_Rcvr_wrapper,CHANGE); } // (pin 2) trigger zero cross
}
/*
	Writes a sequence of bits out.  If the sequence is not a start code,
	it repeats the bits, inverting them.
*/

void x10::sendBits(byte cmd, byte numBits, byte isStartCode) {
  byte thisBit;		// copy of command so we can shift bits
  
	// iterate the number of bits to be shifted:
	for(int i=1; i<=numBits; i++) {
		// wait for a zero crossing change:
		waitForZeroCross(this->zeroCrossingPin, 1);
		// shift off the last bit of the command:
		thisBit = !!(cmd & (1 << (numBits - i)));
		
		// repeat once for each phase:
		for (int phase = 0; phase < 3; phase++) {
			// set the data Pin:
			digitalWrite(this->dataPin, thisBit);
			delayMicroseconds(BIT_LENGTH);
			// clear the data pin:
			digitalWrite(this->dataPin, LOW);
			delayMicroseconds(BIT_DELAY);
		}
		
		// if this command is a start code, don't
		// send its complement.  Otherwise do:
		if(!isStartCode) {
			// wait for zero crossing:
			waitForZeroCross(zeroCrossingPin, 1);
			for (int phase = 0; phase < 3; phase++) {
				// set the data pin:
				digitalWrite(this->dataPin, !thisBit);
				delayMicroseconds(BIT_LENGTH);
				// clear the data pin:
				digitalWrite(dataPin, LOW);
				delayMicroseconds(BIT_DELAY);
			}
		}
	}
}


/*
  waits for a the zero crossing pin to cross zero

*/
void x10::waitForZeroCross(int pin, int howManyTimes) {
	unsigned long cycleTime = 0;
	
  	// cache the port and bit of the pin in order to speed up the
  	// pulse width measuring loop and achieve finer resolution.  calling
  	// digitalRead() instead yields much coarser resolution.
  	uint8_t bit = digitalPinToBitMask(pin);
  	uint8_t port = digitalPinToPort(pin);

  	for (int i = 0; i < howManyTimes; i++) {
		// wait for pin to change:
    	if((*portInputRegister(port) & bit))
    	 	while((*portInputRegister(port) & bit)) 
        		cycleTime++;
    	else
      		while(!(*portInputRegister(port) & bit)) 
        		cycleTime++;
  		}
}


/*
  version() returns the version of the library:
*/
int x10::version(void)
{
  Serial.print(this->zeroCrossingPin); Serial.print(" ");
  Serial.print(this->dataPin); Serial.print(" ");
  Serial.print(rcve_pin); Serial.print(" ");
  Serial.println(led_pin); 
  return 4;
}

boolean x10::received(void)
{
  return _newX10;
}

void x10::reset(void)
{
  _newX10 = false;
}

byte x10::unitCode(void)
{
  return _unitCode;
}
byte x10::houseCode(void)
{
  return _houseCode;
}
byte x10::uc(void)
{
  return _uc;
}

byte x10::hc(void)
{
 return _hc;
}
byte x10::cmndCode(void)
{
  return _cmndCode;
}

void x10::Check_Rcvr(){    // ISR - called when zero crossing (on CHANGE)
  if (X10BitCnt == 0) {                // looking for new frame
    delayMicroseconds(OFFSET_DELAY);   // wait for bit
    if(digitalRead(rcve_pin)) return;  // still high - no start bit - get out
    if (led_pin>0) { digitalWrite(led_pin, HIGH); }     // indicate you got something
    rcveBuff = 0;
    mask = 0x1000;                     // bitmask with bit 12 set
    rcveBuff = rcveBuff | mask;        // sets bit 12 (highest)
    mask = mask >> 1;                  // move bit down in bit mask
    X10BitCnt = 1;                     // inc the bit count
    ZCrossCnt = 1;                     // need to count zero crossings too
    return;
  }
  // Begins here if NOT the first bit . . .
  ZCrossCnt++;                         // inc the zero crossing count
  // after SC (first 4 bits) ignore the pariety bits - so only read odd crossings
  if (X10BitCnt < 5 || (ZCrossCnt & 0x01)){ // if it's an odd # zero crossing
    delayMicroseconds(OFFSET_DELAY);   // wait for bit
    if(!digitalRead(rcve_pin)) rcveBuff = rcveBuff | mask;  // got a 1 set the bit, else skip and leave it 0
    mask = mask >> 1;                  // move bit down in bit mask
    X10BitCnt++;

    if(X10BitCnt == 13){               // done with frame after 13 bits
      for (byte i=0;i<5;i++)delayMicroseconds(HALF_CYCLE_DELAY); // need this
      X10rcvd = true;                  // a new frame has been received
      if (led_pin>0) { digitalWrite(led_pin, LOW); }     // indicate you got something
      X10BitCnt = 0;
      x10_Parse_Frame_wrapper();                   // parse out the house & unit code and command
    }
  }
}

#if 1
void x10::Parse_Frame() {   // parses the receive buffer to get House, Unit, and Cmnd
  if(rcveBuff & 0x1){                  // last bit set so it's a command
    _cmndCode = rcveBuff & 0x1F;        // mask 5 bits 0 - 4 to get the command
    _newX10 = true;                     // now have complete pair of frames
  }
  else {                               // last bit not set so it's a unit
    _unitCode = rcveBuff & 0x1F;        // mask 5 bits 0 - 4 to get the unit
    _uc = _unitCode;
    _newX10 = false;                    // now wait for the command
    for (byte i=0; i<16; i++){         // use lookup table to get the actual unit #
      if (Unit[i] == _unitCode){
        _unitCode = i+1;                // this gives Unit 1-16
        break;                         // stop search when found!
      }
    }
  }
  rcveBuff = rcveBuff >> 5;            // shift the house code down to LSB
  _houseCode = rcveBuff & 0x0F;         // mask the last 4 bits to get the house code
  _hc = _houseCode;
  for (byte i=0; i<16; i++){           // use lookup table to get the actual command #
    if (House[i] == _houseCode){ 
      _houseCode = i+65;                // this gives House 'A' - 'P'
      break;                           // stop search when found!
    }
  }
  rcveBuff = rcveBuff >> 4;            // shift the start code down to LSB
  startCode = rcveBuff & 0x0F;         // mask the last 4 bits to get the start code
  X10rcvd = false;                     // reset status
}
#else
// NEW VERSION - better error checking
void Parse_Frame() {   // parses the receive buffer to get House, Unit, and Cmnd
  // Remember this is called by ISR - so STILL IN ISR! - no delays or prints!
  byte rawCmndUnit;                     // Command or Unit portion of rcveBuff
  byte rawHouse;                        // House portion of rcveBuff
  byte foundMatch = false;              // set when a match is found

  // break the rcveBuff into it's parts . . .
  if(rcveBuff & 0x1) procCmnd = true;   // last bit set so frame is a command (2nd)
  else procCmnd = false;                // else frame is a unit (1st)
  rawCmndUnit = rcveBuff & 0x1F;        // mask 5 bits 0 - 4 get the unit / command
  rcveBuff = rcveBuff >> 5;             // shift the house code down to LSB
  rawHouse = rcveBuff & 0x0F;           // mask the last 4 bits to get the house code
  rcveBuff = rcveBuff >> 4;             // shift the start code down to LSB
  X10.Start = rcveBuff & 0x0F;          // mask the last 4 bits to get the start code

  // start with the House code - SB the same for both frames
  for (byte i=0; i<16; i++){            // use lookup table to get the actual command #
    if (House[i] == rawHouse){ 
      X10.House = i+65;                 // this gives House 'A' - 'P'
      foundMatch = true;
      break;                            // stop search when found!
    }
  }
  // if no match, wipe out the start byte to abort the command
  if (foundMatch == false) X10.Start = 0; 
  // if it's the 1st frame, save off the house
  if (foundMatch == true && procCmnd == false) firstHouse = X10.House; 
  // if it's the 2nd frame, compare the house code for the 2 frames
  if (foundMatch == true && procCmnd == true){
    if (X10.House != firstHouse) X10.Start = 0; // abort if not the same
  }
  // If it's the 1st frame process the Unit code . . .
  if (procCmnd == false){               // last bit not set so it's a unit
    _newX10 = false;                     // now wait for the command
    foundMatch = false;                 // reset this before search
    for (byte i=0; i<16; i++){          // use lookup table to get the actual unit #
      if (Unit[i] == rawCmndUnit){
        X10.Unit = i+1;                 // this gives Unit 1-16
        foundMatch = true;
        break;                          // stop search when found!
      }
    }
    if (foundMatch == false) X10.Start = 0; // wipe out the start byte if no match
  }

  // If it's the 2nd frame process the Command code . . .
  if (procCmnd == true){                // last bit set so it's a command
    if (rawCmndUnit == ON) X10.Cmnd=on; // convert commands to generic commands
    else if (rawCmndUnit == OFF) X10.Cmnd=off;
    else if (rawCmndUnit == DIM) X10.Cmnd=dim;
    else if (rawCmndUnit == BRIGHT) X10.Cmnd=bright;
    else X10.Start = 0;                 // wipe out the start byte if no match
    _newX10 = true;                      // now have complete pair of frames
  }

  // if the command was set as invalid, it's not a new command
  if (X10.Start != B1110) _newX10 = false; // final error check here
  //if (gParam[NO_P16]){                  // optionally stop phantom P-16 commands
  // if (X10.House == 'P' && X10.Unit == 16) _newX10 = false; // Fix for phantom P-16 comand
  //}
}
#endif

void x10::attach(void)
{
   attachInterrupt(0,x10_Check_Rcvr_wrapper,CHANGE);// (pin 2) trigger zero cross
}
void x10::detach(void)
{
   detachInterrupt(0);                  // must detach interrupt before sending
}

void x10::debug(void){
  Serial.print("SC-");
  Serial.print(startCode,BIN);
  Serial.print(" HOUSE-");
  Serial.print(_houseCode);
  Serial.print(" UNIT-");
  Serial.print(_unitCode,DEC);
  Serial.print(" CMND");
  Serial.print(_cmndCode,DEC);
  if(_cmndCode == ON)Serial.print(" (ON)");
  if(_cmndCode == OFF)Serial.print(" (OFF)");
  Serial.println("");
}
