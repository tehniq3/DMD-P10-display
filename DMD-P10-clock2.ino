/*--------------------------------------------------------------------------------------
ver.1 - initial version by Nicu FLORICA (niq_ro) to display a clock with RTC (DS1307/DS3231)
ver.2 - add data on screen
--------------------------------------------------------------------------------------*/
// changeD by Nicu FLORICA (niq_ro) to display a clock with RTC (DS1307/DS3231)

/*--------------------------------------------------------------------------------------
  Includes
--------------------------------------------------------------------------------------*/
#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

#include "SystemFont5x7.h"
//#include "Arial_black_16.h"
#include "Arial14.h"

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

int x,y;
int pauza = 500;

int ora = 0;
byte ora1 = 0;
byte ora2 = 0;
int minut = 0;
int minutant = 70;
byte minut1 = 0;
byte minut2 = 0;
int zi = 0;
byte zi1 = 0;
byte zi2 = 0;
int luna = 0;
byte luna1 = 0;
byte luna2 = 0;
int an = 0;
byte an1 = 0;
byte an2 = 0;
int secunda = 0;

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};



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
    //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

   
   //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
   dmd.selectFont(Arial_14);

   #ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
   //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // MANUAL ADJUST
}

/*--------------------------------------------------------------------------------------
  loop
  Arduino architecture main loop
--------------------------------------------------------------------------------------*/
void loop(void)
{
   DateTime now = rtc.now();
ora = now.hour(), DEC;
ora1 = ora/10;
ora2 = ora%10;
minut = now.minute(), DEC;
minut1 = minut/10;
minut2 = minut%10;
zi =  now.day(), DEC;
zi1 = zi/10;
zi2 = zi%10;
luna = now.month(), DEC;
luna1 = luna/10;
luna2 = luna%10;
an = now.year(), DEC;
an = an - 2000;
an1 = an/10;
an2 = an%10;

secunda = now.second(), DEC; 

 /*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();   
    Serial.println();
*/


// https://www.asciitable.com/ (48 = '0', ... 57 ='9')
if (minut != minutant) dmd.clearScreen( true );
x = 1;  
y = 1;
//dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
//dmd.selectFont(Arial_14);
//   dmd.drawChar(x,y, '2', GRAPHICS_NORMAL );
if (ora < 10) dmd.drawChar(x,y, ' ', GRAPHICS_NORMAL );
else
dmd.drawChar(x,y, 48+ora1, GRAPHICS_NORMAL );
dmd.drawChar(x+7,y, 48+ora2, GRAPHICS_NORMAL );
if (millis()/1000%2 == 1)
dmd.drawChar(x+15,y, ':', GRAPHICS_NORMAL );
else
dmd.drawChar(x+15,y, ' ', GRAPHICS_NORMAL );
dmd.drawChar(x+18,y, 48+minut1, GRAPHICS_NORMAL );
dmd.drawChar(x+25,y, 48+minut2, GRAPHICS_NORMAL );

delay(pauza);
minutant = minut;

if (secunda == 30)
{
// day and month
dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
dmd.selectFont(SystemFont5x7);
if (zi < 10) dmd.drawChar(x+1,y, '0', GRAPHICS_NORMAL );
else
dmd.drawChar(x+1,y, 48+zi1, GRAPHICS_NORMAL );
dmd.drawChar(x+7,y, 48+zi2, GRAPHICS_NORMAL );
dmd.drawChar(x+11,y, '.', GRAPHICS_NORMAL );
if (luna < 10) dmd.drawChar(x+16,y, '0', GRAPHICS_NORMAL );
else
dmd.drawChar(x+16,y, 48+luna1, GRAPHICS_NORMAL );
dmd.drawChar(x+22,y, 48+luna2, GRAPHICS_NORMAL );
dmd.drawChar(x+26,y, '.', GRAPHICS_NORMAL );
// year
dmd.drawChar(x+4,y+8, '2', GRAPHICS_NORMAL );
dmd.drawChar(x+10,y+8, '0', GRAPHICS_NORMAL );
dmd.drawChar(x+16,y+8, 48+an1, GRAPHICS_NORMAL );
dmd.drawChar(x+22,y+8, 48+an2, GRAPHICS_NORMAL );

delay(5000);

dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
dmd.selectFont(Arial_14);
}

} // end main loop
