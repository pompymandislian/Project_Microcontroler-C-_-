/*Program by Agam Sanjaya from sansjaya channel 
 * Dated : 31-01-2020
 * credit : B.Aswinth Raj/www.circuitdigest.com
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);//adrressing lcd 0x27,16X2 lcd

// connect motor controller pins to Arduino digital pins
// motor one
int enA = 10;
int in1 = 9;
int in2 = 8;
// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;

//library disertakan di deskripsi

#define RL 10 //nilai RL =10 pada sensor
#define m -0.417 //hasil perhitungan gradien
#define b 0.425 //hasil perhitungan perpotongan
#define Ro 19 //hasil pengukuran RO
#define MQ_sensor A0 //definisi variabel

const int numReadings = 5;//nilai penambilan sample pembacaan sebesar 5 kali
float readings[numReadings];      
int readIndex = 0;              
float total = 0;                  
float average = 0;                

void setup() {
  Serial.begin(9600);
   for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
for (int times = 1; times <=3; times++) {    
  }
  
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
 float VRL; 
  float RS;  
  float ratio; 
   
  VRL = analogRead(MQ_sensor)*(5/1023.0); //konversi analog ke tegangan
   RS =(5.0 /VRL-1)*10 ;//rumus untuk RS
  ratio = RS/Ro;  // rumus mencari ratio
  float ppm = pow(10, ((log10(ratio)-b)/m));//rumus mencari ppm
  
 total = total - readings[readIndex];

  readings[readIndex] = ppm;

  total = total + readings[readIndex];

  readIndex = readIndex + 1;


  if (readIndex >= numReadings) {

    readIndex = 0;
  }

  average = total / numReadings;
  Serial.print (average); 
  if (average > 22)
{
   digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, 200);
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, 200);
  delay (10000);
   digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, 200);
  // turn on motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, 200);
  delay (10000);  
}
else
{
  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW); 
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
   delay(1000);

}

void loop() {

 
}
