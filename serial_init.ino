#include <SoftwareSerial.h>
#include <time.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "NMEA0183/NMEA0183.h"
//Rewrite to header:
//#include "NMEA0183-WiFi-Multiplexer/NMEA083-WiFi-Multiplexer"

//MACRO DEFNITIONS - UART's
//----------------------------------------------
// Serial = DEBUG
#define DEBUG_Baud 115200

// Ser1 = GPS_SERIAL
#define Ser1_RX_PIN 5
#define Ser1_TX_PIN 4
#define Ser1_Baud 4800

// Ser2 = GPS_SERIAL
#define Ser2_RX_PIN 12
#define Ser2_TX_PIN 13
#define Ser2_Baud 9600
//---------------------------------------------


#define MAX_NMEA0183_MESSAGE_SIZE 150
char buf1[MAX_NMEA0183_MESSAGE_SIZE] = "";    // Buffer for serial#1
char buf2[MAX_NMEA0183_MESSAGE_SIZE] = "";    // Buffer for serial#2

// Create software serial objects
SoftwareSerial swSer1(Ser1_RX_PIN, Ser1_TX_PIN);
SoftwareSerial swSer2(Ser2_RX_PIN, Ser2_TX_PIN);




void setup(void) {

  //Debug serial config
  Serial.begin(DEBUG_Baud);                   //Initialize hardware serial with baudrate of 115200

  //Ser1 config
  pinMode(Ser1_RX_PIN, INPUT);
  pinMode(Ser1_TX_PIN, OUTPUT);
  swSer1.enableIntTx(false);
  swSer1.begin(Ser1_Baud);                    //Initialize software serial with baudrate of 4800

  //Ser2 config
  pinMode(Ser2_RX_PIN, INPUT);
  pinMode(Ser2_TX_PIN, OUTPUT);
  swSer2.enableIntTx(false);
  swSer2.begin(Ser2_Baud);                    //Initialize software serial with baudrate of 9600


  //INIT:
  Serial.println("\nESP8266 Software serial config started");


}







void UARTS_Verification()
{
  char ch = 'q';
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


void loop()
{
  UARTS_Verification();


}