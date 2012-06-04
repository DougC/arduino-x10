V4 X10 Send/Receive Code
by creatrope

Place this library in your libraries folder. It has been tested with Arduino 0018.

It is an upgraded version of the X10 library that incorporates the original X10 send code, with Brohogan's X10 receive code.  

This preliminary version is the most minimal integration possible - no attempt has been made to combine similar structures internally for maximal efficiency.

It is not quite upward compatible with the original v3 X10 send library.

The single letter X10 house codes #defines, which caused me lots of problems, have been replaced with #defines like HOUSE_A, HOUSE_B, rather than just A or B.

There are some new return functions, one that returns the largely unintelligible 'binary' house and unit codes, which are used for input in the original v3 library, and easily readable/printable ascii house codes and integer units codes.

The indicator LED pin is now settable.

There are several constructors, one of which retains the simple two field zCross and data output pin, and additional constructors that take one or both of the input pin and LED pin.



