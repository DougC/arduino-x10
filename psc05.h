#ifndef LPCS05
#define LPCS05
// Defines and constants for PSC05 receiving

#define OFFSET_DELAY     500 	// uS from zero cross to center of bit (sugg 500-700 us)
#define HALF_CYCLE_DELAY 8334 	// Calculated 8334 uS between bit repeats in a half-cycle

#ifndef ON                      // use same defines from x10constants.h for rcvd cmnds
#define ON   B00101             // these are examples
#endif
#ifndef OFF
#define OFF  B00111
#endif

byte House[16] = {              // Lookup table for House Code
  B0110,  // A
  B1110,  // B
  B0010,  // C
  B1010,  // D
  B0001,  // E
  B1001,  // F
  B0101,  // G
  B1101,  // H
  B0111,  // I
  B1111,  // J
  B0011,  // K
  B1011,  // L
  B0000,  // M
  B1000,  // N
  B0100,  // O
  B1100,  // P
};

byte Unit[16] = {               // Lookup table for Unit Code
  B01100,  // 1
  B11100,  // 2
  B00100,  // 3
  B10100,  // 4
  B00010,  // 5
  B10010,  // 6
  B01010,  // 7
  B11010,  // 8
  B01110,  // 9
  B11110,  // 10
  B00110,  // 11
  B10110,  // 12
  B00000,  // 13
  B10000,  // 14
  B01000,  // 15
  B11000,  // 16
};
#endif
