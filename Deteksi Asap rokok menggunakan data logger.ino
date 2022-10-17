#include <OneWire.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <math.h>
#define Vc 4.95
//the number of R0 you detected just now
#define R0A 20.50
#define R0B 22.28
// sensor gas
#define pinSensorC A2

// deklarasi LCD i2c
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RTC
RTC_DS1307 RTC;
OneWire  ds(8);  // temperature senor on pin 8 (a 4.7K resistor is necessary) //
  const int chipSelect = 10; //cs or the save select pin from the sd shield is connected to 10.

// sensor gas
  double ppmkalA, ppmkalB;
  double VRLC;
  double RsC;
  double co;
  long RLC = 1000; // 1000 Ohm
  long RoC = 830; // 830 ohm ( SILAHKAN DISESUAIKAN)
  int sensorvalueC;

// formaldehide A
  int sensorValueA;
  double RsA;
  double ppmA;

// formaldehide B
  int sensorValueB;
  double RsB;
  double ppmB;

// zero tare
  double tareA, tareB, tareco;

//LDR
  byte ldr = A3;
  byte led = 9;
  int nilai;

// work in line numbers.  Font height in ht
  int16_t ht = 20, top = 1, line, lines = 15, scroll;
  double outputValueA = 0, outputValueB = 0, outputValueC = 0;

File dataFile;
DateTime now;

void setup() {
  pinMode(led, OUTPUT);
  lcd.begin();
  lcd.backlight();
  lcd.print("Alfin");

  
  delay(1000);
  Serial.begin(9600);
  //setup clock
  Wire.begin();
  RTC.begin();
  //check or the Real Time Clock is on
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // uncomment it & upload to set the time, date and start run the RTC!
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  //setup SD card
  Serial.print("Initializing SD card...");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Initializing SD card...");
  delay(1000);

  // see if the SD card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    lcd.setCursor(0,1);
    lcd.print("Card failed, or not present");
    delay(1000);
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  lcd.setCursor(0,1);
  lcd.print("card initialized.");
  delay(1000);

  //write down the date (year / month / day         prints only the start, so if the logger runs for sevenal days you only findt the start back at the begin.
  now = RTC.now();
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.print("Start logging on: ");
  dataFile.print(now.year(),DEC);
  dataFile.print('/');
  dataFile.print(now.month(),DEC);
  dataFile.print('/');
  dataFile.print(now.day(),DEC);
  dataFile.println(" ");
  dataFile.println("ppm1 \t ppm2 \t co \t Time");
  dataFile.close();
  delay(1000);

  hcho();
  hcho2();
  gas();
  //tareA = ppmkalA;
  //tareB = ppmkalB;
  //tareco = co;
  delay(1000);
  lcd.clear();
}
void loop() {
  hcho();
  hcho2();
  gas();
  sdcard();
  Serial.print("HCHO ppmA: ");
  //Serial.print(ppmA);
  Serial.print(ppmkalA);
  Serial.print("\t");
  
  Serial.print("HCHO ppmB: ");
  //Serial.print(ppmB);
  Serial.print(ppmkalB);
  Serial.print("\t");

  Serial.print("CO: ");
  Serial.print(co);
  Serial.println(" ppm");

  tampilkan();
}

void sdcard() {
  //read the time
  now = RTC.now();
  
  //open file to log data in.
   dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  // log the temperature and time.
  if (dataFile) {
    dataFile.print(ppmkalA);
    dataFile.print("\t");
    
    dataFile.print(ppmkalB);
    dataFile.print("\t");
    
    dataFile.print(co);
    dataFile.print("\t");
    dataFile.print(now.hour(),DEC);
    dataFile.print(":");
    dataFile.print(now.minute(),DEC);
    dataFile.print(":");
    dataFile.println(now.second(),DEC);
   
    dataFile.close();
    // print to the serial port too:
    Serial.println("data stored");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  //delay(60000); // this will log the temperature every minute.
}


// fuction with check the temperature sensor and update the tempeature. 
void hcho() {
  sensorValueA = analogRead(A1);
  RsA=(1023.0/sensorValueA)-1;
  ppmA=pow(10.0,((log10(RsA/R0A)-0.0827)/(-0.4807)));
  ppmkalA = ppmA/10;
}
void hcho2() {
  sensorValueB = analogRead(A0);
  RsB=(1023.0/sensorValueB)-1;
  ppmB=pow(10.0,((log10(RsB/R0B)-0.0827)/(-0.4807)));
  ppmkalB = ppmB;
}
void gas() {
  sensorvalueC = analogRead(pinSensorC); // membaca nilai ADC dari sensor
  VRLC= sensorvalueC*5.00/1024;  // mengubah nilai ADC ( 0 - 1023 ) menjadi nilai voltase ( 0 - 5.00 volt )
  //Serial.print("VRL : ");
  //Serial.print(VRL);
  //Serial.println(" volt");

  RsC = ( 5.00 * RLC / VRLC ) - RLC;
  //Serial.print("Rs1 : ");
  //Serial.print(Rs1);
  //Serial.println(" Ohm");

  co = 100 * pow(RsC / RoC,-1.53); // ppm = 100 * ((rs/ro)^-1.53);
}
void tampilkan() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HCHO1:");
  lcd.setCursor(0,1);
  lcd.print(ppmkalA - tareA);
  lcd.print(" PPM");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HCHO2:");
  lcd.setCursor(0,1);
  lcd.print(ppmkalB - tareB);
  lcd.print(" PPM");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CO   :");
  lcd.setCursor(0,1);
  lcd.print(co - tareco);
  lcd.print(" PPM");
  delay(2000);
}
void sensorldr()
{
 nilai = analogRead(ldr);
 Serial.print("Nilai LDR: ");
 Serial.println(nilai);

 if (nilai < 500) {
 digitalWrite(led, HIGH);
 }
 else {
 digitalWrite(led, LOW);
  }
 }
