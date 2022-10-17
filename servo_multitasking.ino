#include <Servo.h>
Servo servo1 , servo2;
byte ldr = A1;
int nilai;
const unsigned long servointerval = 1000;
const unsigned long servo1interval = 500;
const unsigned long sensorinterval = 500;
unsigned long servo1timer;
unsigned long servo2timer;
unsigned long ldrsensor;
int pos = 0;

void setup() {
 Serial.begin(9600);
 servo1.attach (6);
 servo2.attach (7);

}

void moveservo() {
 // Serial.print (pos);
 if (pos <= 1000)
 {
  pos = 3000;
  servo2.writeMicroseconds(pos);
 }
 else {
  pos = 1000;
  servo2.writeMicroseconds(pos);  
}
servo1timer = millis();
}
void ldr1 (){
  nilai = analogRead (ldr);
  if (analogRead (nilai))
  { 
    Serial.println (nilai);
  }
  else
  {
    Serial.println ("ldr: ");
  }
  ldrsensor = millis();
}

void loop(){
 { 
  if ((millis() - servo1timer)>= servointerval)
  moveservo();
  if((millis() - ldrsensor)>= sensorinterval) 
  ldr1();
}
   if (nilai <=100){
     ((millis () - servo2timer) >= servo1interval);
    servo2.write (90);
    servo1.write (90);
    delay (1000);
    servo1.write (0);
    delay (1000);
  }
  servo2timer =millis(); 
  }
