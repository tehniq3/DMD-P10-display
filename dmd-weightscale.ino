/*--------------------------------------------------------------------------------------

 dmd_test.cpp 
   Demo and example project for the Freetronics DMD, a 512 LED matrix display
   panel arranged in a 32 x 16 layout.

 Copyright (C) 2011 Marc Alexander (info <at> freetronics <dot> com)

 See http://www.freetronics.com/dmd for resources and a getting started guide.

 Note that the DMD library uses the SPI port for the fastest, low overhead writing to the
 display. Keep an eye on conflicts if there are any other devices running from the same
 SPI port, and that the chip select on those devices is correctly set to be inactive
 when the DMD is being written to.

 USAGE NOTES
 -----------

 - Place the DMD library folder into the "arduino/libraries/" folder of your Arduino installation.
 - Get the TimerOne library from here: http://code.google.com/p/arduino-timerone/downloads/list
   or download the local copy from the DMD library page (which may be older but was used for this creation)
   and place the TimerOne library folder into the "arduino/libraries/" folder of your Arduino installation.
 - Restart the IDE.
 - In the Arduino IDE, you can open File > Examples > DMD > dmd_demo, or dmd_clock_readout, and get it
   running straight away!

 * The DMD comes with a pre-made data cable and DMDCON connector board so you can plug-and-play straight
   into any regular size Arduino Board (Uno, Freetronics Eleven, EtherTen, USBDroid, etc)
  
 * Please note that the Mega boards have SPI on different pins, so this library does not currently support
   the DMDCON connector board for direct connection to Mega's, please jumper the DMDCON pins to the
   matching SPI pins on the other header on the Mega boards.

 This example code is in the public domain.
 The DMD library is open source (GPL), for more see DMD.cpp and DMD.h

--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------
  Includes
--------------------------------------------------------------------------------------*/
#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
//#include "SystemFont5x7.h"
//#include "Arial_black_16.h"
#include "Arial14.h"

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

/* sample for digital weight scale of hx711
 * library design: Weihong Guan (@aguegu)
 * library host on
 *https://github.com/aguegu/ardulibs/tree/3cdb78f3727d9682f7fd22156604fc1e4edd75d1/hx711
 *adapted by Nicu FLORICA (niq_ro) for 200kg scale cell
 */
#include <hx711.h>
Hx711 scale(A2, A3);
// Hx711.DOUT - pin #A2
// Hx711.SCK - pin #A3

float greutate = 0;
float tarare = 0; 
float corectie = 1.014541387;

int x,y;
int s, z, u, r;
int numar = 2000;
int pauza = 200;

/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}

/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
--------------------------------------------------------------------------------------*/
void setup(void)
{
 Serial.begin(9600);
   //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

   //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
   dmd.selectFont(Arial_14);
}

/*--------------------------------------------------------------------------------------
  loop
  Arduino architecture main loop
--------------------------------------------------------------------------------------*/
void loop(void)
{

  greutate = scale.getGram();
  greutate = greutate - tarare;
  greutate = greutate / corectie; 
numar = greutate;
   Serial.print(greutate/10., 1);
  Serial.println(" kg");
// https://www.asciitable.com/ (48 = '0', ... 57 ='9')
s = numar/1000;
r = numar%1000;
z = r/100;
r = r%100;
u = r/10;
r = r%10;

dmd.clearScreen( true );
x = 1;  
y = 1;
//   dmd.drawChar(x,y, '2', GRAPHICS_NORMAL );
if (s == 0) dmd.drawChar(x,y, ' ', GRAPHICS_NORMAL );
else
dmd.drawChar(x,y, 48+s, GRAPHICS_NORMAL );
if ((s == 0) and (z ==0)) dmd.drawChar(x+7,y, ' ', GRAPHICS_NORMAL );
else
dmd.drawChar(x+7,y, 48+z, GRAPHICS_NORMAL );
dmd.drawChar(x+15,y, 48+u, GRAPHICS_NORMAL );
dmd.drawChar(x+24,y, 48+r, GRAPHICS_NORMAL );
dmd.drawChar(x+22,y, ',', GRAPHICS_NORMAL );
delay(pauza);


}
