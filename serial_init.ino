#include <SoftwareSerial.h>
#include <time.h>
// pins Rx GPIO14 (D5) and Tx GPIO 12 (D6)
#define Ser1_RX_PIN 5
#define Ser1_TX_PIN 4

#define Ser2_RX_PIN 12
#define Ser2_TX_PIN 13

SoftwareSerial swSer1(Ser1_RX_PIN, Ser1_TX_PIN);
SoftwareSerial swSer2(Ser2_RX_PIN, Ser2_TX_PIN);

char ch = 'q';

void setup() {
  Serial.begin(115200);  //Initialize hardware serial with baudrate of 115200
  swSer1.begin(4800);    //Initialize software serial with baudrate of 4800
  swSer2.begin(9600);   //Initialize software serial with baudrate of 9600
  Serial.println("\nESP8266 Software serial test started");

    ets_delay_us(100);
}


void loop() {
//char ch = 'q';
while (true)
{
  while (Serial.available() > 0) {  //wait for data at hardware serial
    ch=Serial.read();
    swSer1.write(ch);     //send data recived from hardware serial to software serial
    swSer2.write(ch);     //send data recived from hardware serial to software seriale
  }
  ch='q';
  while (swSer1.available() > 0) {  //wait for data at software serial
    Serial.write("1>");            //Send data recived from software serial to hardware serial
    Serial.println((char)swSer1.read());    //Send data recived from software serial to hardware serial
  }
  ch='q';
  while (swSer2.available() > 0) {  //wait for data at software serial
    Serial.write("2>");             //Send data recived from software serial to hardware serial
    Serial.println((char)swSer2.read());  //Send data recived from software serial to hardware serial
  }
}
}