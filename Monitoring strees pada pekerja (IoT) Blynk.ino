#define BLYNK_PRINT Serial
#define ONE_WIRE_BUS 2
#define USE_ARDUINO_INTERRUPTS true
#define ESP8266_BAUD 115200
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>

char auth[] = "j3GXnYV91_jZz25Dvp8cnTFqGEjpWuEw";
char ssid[] = "PARULIAN";
char pass[] = "12081997";

ESP8266 wifi(&Serial);
BlynkTimer timer;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
PulseSensorPlayground pulseSensor; 
    
const int PulseWire = 0;     
const int LED13 = 13;          
int Threshold = 550;          
float bpmtotal;                               
float Celsius = 0;
float ctotal;
float voltage;
float gsr;
float gsr_avarage;
float vtotal;

int ledbpm = 10;
int ledsuhu = 11;
int ledgsr = 12;
int nilaiSensor = 0;
int nilai;


String kondisi;
unsigned count = 0;

void setup() {
   Serial.begin(9600);
   delay(10);
   Serial.begin(ESP8266_BAUD);
   delay(10);
   Blynk.begin(auth, wifi, ssid, pass);
    
    pinMode(ledsuhu, OUTPUT);
    pinMode(ledgsr, OUTPUT);
    pinMode(ledbpm, OUTPUT);
    pulseSensor.analogInput(PulseWire);   
    pulseSensor.blinkOnPulse(LED13);      
    pulseSensor.setThreshold(Threshold);   
    if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  
    sensors.begin();
    timer.setInterval(5000L, myTimerEvent);
  }
}

void loop() {
  
  Blynk.run();
  timer.run();   
}


void myTimerEvent(){

  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  ctotal = Celsius + 1.03;
  Serial.print(" C  ");
  Serial.print(ctotal);
  if (ctotal < 33)
  {
    nilai += nilai;
     digitalWrite (ledsuhu ,HIGH);
  }
   else
   {
    digitalWrite (ledsuhu ,LOW);
   } 
  delay(800);

//////////////////////// SUHU TUBUH\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

long sum = 0;
  for(int i = 0; i<10;i++)
  {
    nilaiSensor=analogRead(A0);
    sum += nilaiSensor;
    delay (500);
  }
  gsr_avarage = sum/10;
  Serial.println(gsr_avarage);
   float nilaiSensor = analogRead(A0);
   float voltage = nilaiSensor * (5.0 / 1023);
   vtotal = voltage - 0.37;
  Serial.print (vtotal);
  Serial.println (" mv ");
   if (vtotal > 6)
  {
    nilai += nilai;
    digitalWrite (ledgsr ,HIGH);
  }
   else
   {
    digitalWrite (ledgsr ,LOW);
   } 
  delay (800);

///////////////////GSR\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

 int myBPM = pulseSensor.getBeatsPerMinute();  

if (pulseSensor.sawStartOfBeat()) {            
 Serial.println("♥  A HeartBeat Happened ! "); 
// Serial.print("BPM: ");                       
// Serial.println(myBPM);  
 bpmtotal = myBPM + 1.55;
 Serial.print("BPM: ");                       
 Serial.println(bpmtotal);            
   if (bpmtotal > 100)
  {
    nilai += nilai;
     digitalWrite (ledbpm ,HIGH);
  }
   else
   {
    digitalWrite (ledbpm ,LOW);
   } 
}
 delay (800);

 ////////////////////DETAK JANTUNG\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 
  if(nilai >= 2 ){

   kondisi  = "stress"; 
  }
  if (kondisi == "stress"){
    
  }
   else{
   
    kondisi  = "Tidak Stress";
   }
  nilai = 0;

  Blynk.email("mandispompy@gmail.com", "KEADAAN SEKARANG", kondisi);
  Blynk.notify(kondisi);
 
  Blynk.virtualWrite(V0, ctotal);
  Blynk.virtualWrite(V1, vtotal);
  Blynk.virtualWrite(V2, bpmtotal);
  Blynk.virtualWrite(V3, kondisi);
}
