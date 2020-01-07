/*--------------------------------------------------------------------------------------
ver.1 - initial version by Nicu FLORICA (niq_ro) to display a clock with RTC (DS1307/DS3231)
ver.2 - add data on screen
ver.3 - add manual adjust using 2 buttons
ver.3a - big characters
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
#include "Arial_black_16.h"
//#include "Arial14.h"

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
int an2 = 0;
byte an21 = 0;
byte an22 = 0;

int secunda = 0;

byte secunda2 = 54;
//byte secunda3 = 40;

int orat = 0;
int minutt = 0;
int zit = 0;
int lunat = luna;
int ant = an;
int maxday = 0;

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

RTC_DS1307 rtc;  // also work fine with DS3231

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define meniu 2
#define plus 3

byte stare = 0;  // 0 for clock, etc...
int pauzica = 250; // pause for debounce for buttons

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
 //  dmd.selectFont(Arial_14);
   dmd.selectFont(Arial_Black_16);
   #ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

pinMode(meniu, INPUT);
pinMode(plus, INPUT);
digitalWrite(meniu, HIGH);
digitalWrite(plus, HIGH);

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

if (stare == 0)
  {
if (digitalRead(meniu) == LOW)
 {
  orat = ora;
  delay (pauzica);
  stare = 1;
  Serial.println(stare);
 }
    
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
an2 = an - 2000;
an21 = an2/10;
an22 = an2%10;

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

if (secunda < secunda2)
{
 if (digitalRead(meniu) == LOW)
  {
  delay (pauzica);
  orat = ora;
  stare = 1;
  Serial.println(stare);
  }
//dmd.selectFont(Arial_14);
dmd.selectFont(Arial_Black_16);
// https://www.asciitable.com/ (48 = '0', ... 57 ='9')
if (minut != minutant) dmd.clearScreen( true );
x = 0;  
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
}
delay(pauza);
minutant = minut;

if (secunda > secunda2)
{
x = 1;  
y = 1;
 if (digitalRead(meniu) == LOW)
  {
  orat = ora;
  delay (pauzica);
  stare = 1;
  Serial.println(stare);
  }
// day and month
//dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
dmd.selectFont(SystemFont5x7);
if (zi < 10) dmd.drawChar(x+1,y, '0', GRAPHICS_NORMAL );
else
dmd.drawChar(x,y, 48+zi1, GRAPHICS_NORMAL );
dmd.drawChar(x+6,y, 48+zi2, GRAPHICS_NORMAL );
dmd.drawChar(x+11,y, '.', GRAPHICS_NORMAL );
if (luna < 10) dmd.drawChar(x+16,y, '0', GRAPHICS_NORMAL );
else
dmd.drawChar(x+16,y, 48+luna1, GRAPHICS_NORMAL );
dmd.drawChar(x+22,y, 48+luna2, GRAPHICS_NORMAL );
dmd.drawChar(x+27,y, '.', GRAPHICS_NORMAL );
// year
dmd.drawChar(x+4,y+8, '2', GRAPHICS_NORMAL );
dmd.drawChar(x+10,y+8, '0', GRAPHICS_NORMAL );
dmd.drawChar(x+16,y+8, 48+an21, GRAPHICS_NORMAL );
dmd.drawChar(x+22,y+8, 48+an22, GRAPHICS_NORMAL );

//delay(5000);
//dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
}
if (secunda == secunda2) dmd.clearScreen( true );
//if (secunda = 59) dmd.clearScreen( true );
} // stare = 0 - usual clock

// change to hour
if (stare == 1)
{
  dmd.selectFont(SystemFont5x7);
  dmd.clearScreen( true ); 
  delay(pauzica);
  stare = 2;
  Serial.println(stare);
}

// change hour
if (stare == 2)
{
  dmd.drawChar(x,y, 'h', GRAPHICS_NORMAL );
  dmd.drawChar(x+5,y, 'o', GRAPHICS_NORMAL );
  dmd.drawChar(x+10,y, 'u', GRAPHICS_NORMAL );
  dmd.drawChar(x+15,y, 'r', GRAPHICS_NORMAL );
  dmd.drawChar(x+20,y, ':', GRAPHICS_NORMAL );  

if (digitalRead(plus) == LOW)
  {
  orat = orat+1;
  delay (3*pauzica);
  }
if (digitalRead(meniu) == LOW)
  {
  stare = 3;
  delay (pauzica);
  Serial.println(stare);
  delay(pauzica);
  }
 if (orat > 23) orat = 0;
  if (orat < 10) dmd.drawChar(x+10,y+8, ' ', GRAPHICS_NORMAL );
  else
  dmd.drawChar(x+10,y+8, 48+orat/10, GRAPHICS_NORMAL );
  dmd.drawChar(x+16,y+8, 48+orat%10, GRAPHICS_NORMAL );
} // end change hour

// change to minutes
if (stare == 3)
{
  dmd.selectFont(SystemFont5x7);
  dmd.clearScreen( true );
  minutt = minut;
  stare = 4;
  Serial.println(stare);
}

// change minutes
if (stare == 4)
{
  dmd.drawChar(x,y, 'm', GRAPHICS_NORMAL );
  dmd.drawChar(x+5,y, 'i', GRAPHICS_NORMAL );
  dmd.drawChar(x+10,y, 'n', GRAPHICS_NORMAL );
  dmd.drawChar(x+15,y, 's', GRAPHICS_NORMAL );
  dmd.drawChar(x+20,y, ':', GRAPHICS_NORMAL );  

if (digitalRead(plus) == LOW)
  {
  minutt = minutt+1;
  delay (3*pauzica);
  }
if (digitalRead(meniu) == LOW)
  {
  stare = 5;
  delay (pauzica);
  Serial.println(stare);
  }
 if (minutt > 59) minutt = 0;
  if (minutt < 10) dmd.drawChar(x+10,y+8, ' ', GRAPHICS_NORMAL );
  else
  dmd.drawChar(x+10,y+8, 48+minutt/10, GRAPHICS_NORMAL );
  dmd.drawChar(x+16,y+8, 48+minutt%10, GRAPHICS_NORMAL );
} // end change minutes

// change to year
if (stare == 5)
{
  dmd.selectFont(SystemFont5x7);
  dmd.clearScreen( true );
  ant = an2;
  stare = 6;
  Serial.println(stare);
}


// change years
if (stare == 6)
{
  dmd.drawChar(x,y, 'y', GRAPHICS_NORMAL );
  dmd.drawChar(x+5,y, 'e', GRAPHICS_NORMAL );
  dmd.drawChar(x+10,y, 'a', GRAPHICS_NORMAL );
  dmd.drawChar(x+15,y, 'r', GRAPHICS_NORMAL );
  dmd.drawChar(x+20,y, ':', GRAPHICS_NORMAL );  

if (digitalRead(plus) == LOW)
  {
  ant = ant+1;
  delay (3*pauzica);
  }
if (digitalRead(meniu) == LOW)
  {
  stare = 7;
  delay (pauzica);
  Serial.println(stare);
  }
 if (ant > 39) ant = 19;
  dmd.drawChar(x,y+8, '2', GRAPHICS_NORMAL );
  dmd.drawChar(x+6,y+8, '0', GRAPHICS_NORMAL );  
  dmd.drawChar(x+12,y+8, 48+ant/10, GRAPHICS_NORMAL );
  dmd.drawChar(x+18,y+8, 48+ant%10, GRAPHICS_NORMAL );
} // end change years

// change to months
if (stare == 7)
{
  dmd.selectFont(SystemFont5x7);
  dmd.clearScreen( true );
  lunat = luna;
  stare = 8;
}

// change months
if (stare == 8)
{
  dmd.drawChar(x,y, 'm', GRAPHICS_NORMAL );
  dmd.drawChar(x+5,y, 'o', GRAPHICS_NORMAL );
  dmd.drawChar(x+10,y, 'n', GRAPHICS_NORMAL );
  dmd.drawChar(x+15,y, 't', GRAPHICS_NORMAL );
  dmd.drawChar(x+20,y, 'h', GRAPHICS_NORMAL );  

if (digitalRead(plus) == LOW)
  {
  lunat = lunat+1;
  delay (3*pauzica);
  }
if (digitalRead(meniu) == LOW)
  {
  stare = 9;
  delay (pauzica);
  Serial.println(stare);
  }
 if (lunat >12) lunat = 1;
  if (lunat < 10) dmd.drawChar(x+10,y+8, ' ', GRAPHICS_NORMAL );
  else
  dmd.drawChar(x+10,y+8, 48+lunat/10, GRAPHICS_NORMAL );
  dmd.drawChar(x+16,y+8, 48+lunat%10, GRAPHICS_NORMAL );
} // end change minutes

// change to day
if (stare == 9)
{
  dmd.selectFont(SystemFont5x7);
  dmd.clearScreen( true );
  zit = zi;
  stare = 10;
}

// change days
if (stare == 10)
{
  dmd.drawChar(x,y, 'd', GRAPHICS_NORMAL );
  dmd.drawChar(x+5,y, 'a', GRAPHICS_NORMAL );
  dmd.drawChar(x+10,y, 'y', GRAPHICS_NORMAL );
  dmd.drawChar(x+15,y, ':', GRAPHICS_NORMAL );
//  dmd.drawChar(x+20,y, 'h', GRAPHICS_NORMAL );  

if (digitalRead(plus) == LOW)
  {
  zit = zit+1;
  delay (3*pauzica);
  }
if (digitalRead(meniu) == LOW)
  {
  stare = 11;
  delay (pauzica);
  Serial.println(stare);
  }

  if (lunat == 4 || lunat == 5 || lunat == 9 || lunat == 11) { //30 days hath September, April June and November
    maxday = 30;
  }
  else {
  maxday = 31; //... all the others have 31
  }
  if (lunat ==2 && ant%4 == 0) { //... Except February alone, and that has 28 days clear, and 29 in a leap year.
    maxday = 29;
  }
  if (lunat ==2 && ant % 4 !=0) {
    maxday = 28;
  }

 if (zit >maxday) zit = 1;
  if (zit < 10) dmd.drawChar(x+10,y+8, ' ', GRAPHICS_NORMAL );
  else
  dmd.drawChar(x+10,y+8, 48+zit/10, GRAPHICS_NORMAL );
  dmd.drawChar(x+16,y+8, 48+zit%10, GRAPHICS_NORMAL );
} // end change days

// change to normal
if (stare == 11)
{
  dmd.selectFont(SystemFont5x7);
  dmd.clearScreen( true );
   // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    rtc.adjust(DateTime(2000+ant, lunat, zit, orat, minutt, 0));
  stare = 0;
}

} // end main loop
