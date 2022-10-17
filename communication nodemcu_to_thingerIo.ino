#include <SoftwareSerial.h>
#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>


SoftwareSerial DataSerial (12, 13);

unsigned long previousMillis = 0;
const long interval = 3000;

//variable array untuk data parsing
String arrData[2];
// konfigurasi thingerIO
#define USERNAME "pompy"
#define DEVICE_ID "Serial_ThingerIO"
#define DEVICE_CREDENTIAL "_3eljvAxQLvY"
#define LED_PIN 4 // pin D2 nodemcu
ThingerESP8266 thing (USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

const char * ssid = "PARULIAN";
const char* password = "12081997";
//sediakan variable untuk menampung nilai sensor yang siap dikirim ke thingerIO
int ldr, ldr1;

void setup() {
Serial.begin (9600);
DataSerial.begin(9600);
pinMode (LED_PIN , OUTPUT);

// koneksi ke wifi
WiFi.begin (ssid, password);
// cek koneksi
while (WiFi.status() !=WL_CONNECTED)
{
  delay(500);
  digitalWrite(LED_PIN, LOW);
  }
  // apabila terkoneksi
  digitalWrite(LED_PIN, HIGH);
  // hubungkan nodemu ke thingerIO
  thing.add_wifi (ssid, password);
  thing["Dataku"] >>[](pson & out)
  {
    out ["ldr"] = ldr;
    out["ldr1"] = ldr1;
  };
}


void loop() {
//konfigurasi millis
unsigned long currentMillis = millis(); // baca waktu miilis saat ini
if (currentMillis - previousMillis>= interval)
{
// update previousMillis
previousMillis = currentMillis;
//prioritaskan pembacaan data dari arduino uno (hasil kiriman data)
// baca data serial
String data= "";
while (DataSerial.available()>0)
{
  data += char (DataSerial.read ());
}
// buang spasi data
data.trim ();
// uji data
if (data != "")
{
  int index = 0;
  for(int i=0 ; i<= data.length(); i++)
  {
    char delimiter = '#';
    if (data [i] != delimiter)
    arrData [index] += data [i];
    else
    index++; // variabel index bertambah 1
  }
  // pastikan bahwa data yang dikirim lengkap (2 ldr)
  // urutannya 0, 1
  if (index == 1)  
  {
    //tampilkan nilai sensor ke serial monitor
    Serial.println ("LDR1 : " + arrData[0]); // ldr
    Serial.println ("LDR2 : " + arrData[1]); // ldr1
    Serial.println ();
  }
  // isi variable yang akan dikirim
  ldr = arrData[0].toInt(); //ngubah dari arrdata bentuk string ke int kalau float toFloat
  ldr1 = arrData[1].toInt();
//picu pengiriman data ke thingerio
thing.handle();
  
  arrData[0] = "";
  arrData[1] = "";
}
// minta data ke arduino uno
DataSerial.println ("Ya");
}
}
