#include  <Servo.h>
#include <Wire.h>
#include <MPU6050.h>
byte degree[8] = {
  0b00000,
  0b00110,
  0b01111,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
 

MPU6050 mpu;

Servo myservo; 
int pos = 90;   // initial position
int sens1 = A0; // LRD 1 pin
int sens2 = A1; //LDR 2 pin
int tolerance = 2; 
void setup() 
{ 
  Serial.begin(115200);
  Serial.println("Initialize MPU6050");
  
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("tidak ada sensor MPU6050 yang terpasang!");
    delay(500);
  }
 
  //Kalibrasi gyrometer
  mpu.calibrateGyro();
  mpu.setThreshold(3);
  myservo.attach(7);  // attaches the servo on pin 9 to the servo object 
  pinMode(sens1, INPUT);
  pinMode(sens2, INPUT);
  myservo.write(pos);
  Serial.begin (9600);
  delay(2000); // a 2 seconds delay while we position the solar panel
}  
 
void loop() 
{   
  int val1 = analogRead(sens1); // read the value of sensor 1
  int val2 = analogRead(sens2); // read the value of sensor 2
Serial.println (val1);
Serial.println (val2);
  if((abs(val1 - val2) <= tolerance) || (abs(val2 - val1) <= tolerance)) {
    //do nothing if the difference between values is within the tolerance limit
  } else {    
    if(val1 > val2)
    {
      pos = --pos;
    }
    if(val1 < val2) 
    {
      pos = ++pos;
    }
  }

  if(pos > 180) { pos = 180; } // reset to 180 if it goes higher
  if(pos < 0) { pos = 0; } // reset to 0 if it goes lower
  
  myservo.write(pos); // write the position to servo
  gyro();
  delay(50);

}

void gyro (){
  Vector rawGyro = mpu.readRawGyro();
 
  Serial.println("Gryometer");
  Serial.print("X : ");
  Serial.print(rawGyro.XAxis);
  Serial.print("Y : ");
  Serial.print(rawGyro.YAxis);
  Serial.print("Z : ");
  Serial.println(rawGyro.ZAxis);
 
  //untuk pembacaan accelerometer
  Vector rawAccel = mpu.readRawAccel();
 
  Serial.println("Accelerometer");
  Serial.print("X : ");
  Serial.print(rawAccel.XAxis);
  Serial.print("Y : ");
  Serial.print(rawAccel.YAxis);
  Serial.print("Z : ");
  Serial.println(rawAccel.ZAxis);
}
