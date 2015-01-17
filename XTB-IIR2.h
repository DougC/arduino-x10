
#ifndef XTB-IIR2
#define XTB-IIR2
// Defines and constants for XTB-IIR2 digital port which emulates the TW523

// From http://jvde.us/xtb/XTB-IIR_modeoptions.htm
// Programming the Mode Options in the XTB-IIR requires a sequence of X10 commands to be sent using a Maxi
// Controller plugged into its X10 Input receptacle.  Mode commands can also be sent via the digital port.  Each key
// should be pressed for about a second, and the gap between each key press must be less than 4 seconds.  XTB-IIR
// only accepts commands on the selected housecode (default is P).  The one exception is masking housecodes, where
// the OFF/ON must be sent on the housecode that is being enabled or disabled.  All programming begins with the key
// sequence 9-8-2 on the selected housecode, followed by the single key for the mode to change, and then either ON or
// OFF.  For example, the key sequence 9-8-2-14-ON enables transmit on all 3-phases.  The LED will flash for about a
// second after the ON or OFF key is released if the command is accepted.
//
// The user programmable mode options are as follows:  (default settings are in parentheses)
//		16 (on) 	Repeater Enable
//		15 (off)	not used at this time (enabled TW523 mode in XTB-II)
//		14 (off)	Enable 3-Phase Transmit (converts single phase to 3-phase)
//		13 (off)	Delay Transmit Burst to reduce flashing of nearby dimmers
//		12 (off)	Reduced Transmit Power
//		11 (off)	Auto Retransmit of repeat following collision
//		10 (off)	Abort Transmission on collision
//		 9 (on)		Smart Bright/Dim Repeat for sequential commands
//		 8 (off)	Only One Repeat for each command to prevent repeater ping-pong
//		 7 (off)	High Command Storm Threshold 60/min.  (OFF allows 30/min)
//		 6 (on)		Sensitivity (ON for high gain, OFF for low gain)
//		 5 (off)	High AGC threshold (increase threshold by factor of 1.5)
//		 4 (on)		Enable post X10 window AGC sample point
//		 3 (on)		Enable pre X10 window AGC sample point
//		 2  (P)		Housecode for Mode Programming after ALL OFF reset (9-8-2-2-ON)
//		 2 DIM 		Read detection threshold as pre-set dim command
//		 2 OFF 		Unlock the housecode disable non-volatile memory
//		 2 ALL_OFF	Disable repeating commands on the ALL_OFF housecode
//		 2 ALL_ON 	Enable repeating commands on the ALL_ON housecode
//		 1 (off)	Return All Bits to the digital port with no error checking

#define OFFSET_DELAY     500 	// uS from zero cross to center of bit (sugg 500-700 us)
#define HALF_CYCLE_DELAY 8334 	// Calculated 8334 uS between bit repeats in a half-cycle
#define XTB-IIR_SEND_DELAY 1000 // Each progrkey should be pressed for about a second

#ifndef STATUS_REQUEST
#define STATUS_REQUEST		B11111
#endif
#ifndef ON					   // use same defines from x10constants.h for rcvd cmnds
#define ON			B00101             // these are examples
#endif
#ifndef OFF
#define OFF			B00111
#endif

byte Mode[16] = {               // Lookup table for Unit Code
  B01100,  // 1 (off) Return All Bits to the digital port with no error checking
  B11100,  // 2  (P) Housecode for Mode Programming after ALL OFF reset (9-8-2-2-ON)
	   // 2 DIM Read detection threshold as pre-set dim command
	   // 2 OFF Unlock the housecode disable non-volatile memory
	   // 2 ALL_OFF Disable repeating commands on the ALL_OFF housecode
	   // 2 ALL_ON  Enable repeating commands on the ALL_ON housecode
  B00100,  // 3 (on) Enable pre X10 window AGC sample point
  B10100,  // 4 (on) Enable post X10 window AGC sample point
  B00010,  // 5 (off) High AGC threshold (increase threshold by factor of 1.5)
  B10010,  // 6 (on) Sensitivity (ON for high gain, OFF for low gain)
  B01010,  // 7 (off) High Command Storm Threshold 60/min.  (OFF allows 30/min)
  B11010,  // 8 (off) Only One Repeat for each command to prevent repeater ping-pong
  B01110,  // 9 (on) Smart Bright/Dim Repeat for sequential commands
  B11110,  // 10 (off) Abort Transmission on collision
  B00110,  // 11 (off) Auto Retransmit of repeat following collision
  B10110,  // 12 (off) Reduced Transmit Power
  B00000,  // 13 (off) Delay Transmit Burst to reduce flashing of nearby dimmers
  B10000,  // 14 (off) Enable 3-Phase Transmit (converts single phase to 3-phase)
  B01000,  // 15 (off) not used at this time (enabled TW523 mode in XTB-II)
  B11000,  // 16 (on) Repeater Enable
};
#endif
