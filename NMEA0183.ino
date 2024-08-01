#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <time.h>

#include "NMEA0183/NMEA0183.h"


#include "WiFi_auth.h"


#include <ESP8266WiFi.h>

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

//#define Ser2_RX_PIN 00
//#define Ser2_TX_PIN 02
#define Ser2_Baud 9600

// Create software serial objects
SoftwareSerial swSer1(Ser1_RX_PIN, Ser1_TX_PIN);
SoftwareSerial swSer2(Ser2_RX_PIN, Ser2_TX_PIN);
//----------------------------------------------


//MACRO DEFNITIONS - NMEA0183
//----------------------------------------------
#define MAX_NMEA0183_MESSAGE_SIZE 150
char buf1[MAX_NMEA0183_MESSAGE_SIZE] = "";  // Buffer for serial#1
char buf2[MAX_NMEA0183_MESSAGE_SIZE] = "";  // Buffer for serial#2


unsigned long startTime = millis();

// provide text for the WiFi status
const char *str_status[]= {
  "WL_IDLE_STATUS",
  "WL_NO_SSID_AVAIL",
  "WL_SCAN_COMPLETED",
  "WL_CONNECTED",
  "WL_CONNECT_FAILED",
  "WL_CONNECTION_LOST",
  "WL_DISCONNECTED"
};

// provide text for the WiFi mode
const char *str_mode[]= { "WIFI_OFF", "WIFI_STA", "WIFI_AP", "WIFI_AP_STA" };





void connectWifi() {
  Serial.print("Connecting as wifi client to SSID: ");
  Serial.println(ssid);

  // use in case of mode problem
  WiFi.disconnect();
  // switch to Station mode
  if (WiFi.getMode() != WIFI_STA) {
    WiFi.mode(WIFI_STA);
  }

  WiFi.begin ( ssid, password );

  //if (debug ) WiFi.printDiag(Serial);

  // ... Give ESP 10 seconds to connect to station.
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  // Check connection
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected; IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.print("WiFi connect failed to ssid: ");
    Serial.println(ssid);
    Serial.print("WiFi password <");
    Serial.print(password);
    Serial.println(">");
    Serial.println("Check for wrong typing!");
  }
}  // connectWiFi()





// declare telnet server (do NOT put in setup())
WiFiServer telnetServer(23);
WiFiClient serverClient;


// setup - configuration
//-----------------------------------------------
void setup(void) {

  //Debug serial config
  Serial.begin(DEBUG_Baud);  //Initialize hardware serial with baudrate of 115200

  //Ser1 config
  pinMode(Ser1_RX_PIN, INPUT);
  pinMode(Ser1_TX_PIN, OUTPUT);
  swSer1.enableIntTx(false);
  swSer1.begin(Ser1_Baud);  //Initialize software serial with baudrate of 4800

  //Ser2 config
  pinMode(Ser2_RX_PIN, INPUT);
  pinMode(Ser2_TX_PIN, OUTPUT);
  swSer2.enableIntTx(false);
  swSer2.begin(Ser2_Baud);  //Initialize software serial with baudrate of 9600


  //INIT:
  Serial.println("\nESP8266 Software serial config started");






  //delay(1000);
  //Serial.begin(115200);
  //delay(1000);
  Serial.println("Sync,Sync,Sync,Sync,Sync Wifi startup");
  delay(500);
  Serial.println();
  // signal start
  //pinMode(BUILTIN_LED2, OUTPUT);
  //digitalWrite(BUILTIN_LED2, LOW);
  //delay(100); // ms
  //digitalWrite(BUILTIN_LED2, HIGH);
  //delay(300); // ms

  Serial.print("Chip ID: 0x");
  Serial.println(ESP.getChipId(), HEX);

  Serial.println("Connect to Router requested");
  connectWifi();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi mode: ");
    Serial.println(str_mode[WiFi.getMode()]);
    Serial.print ( "Status: " );
    Serial.println (str_status[WiFi.status()]);
  } else {
    Serial.println("");
    Serial.println("WiFi connect failed, push RESET button.");
  }

  telnetServer.begin();
  telnetServer.setNoDelay(true);
  Serial.println("Please connect Telnet Client, exit with ^] and 'quit'");

  Serial.print("Free Heap[B]: ");
  Serial.println(ESP.getFreeHeap());

}  //setup




//  GetNMEA0183_Message
// BLOCKING
//-----------------------------------------------
bool GetNMEA0183_Message(SoftwareSerial &swSer, char * buff) {
  static unsigned int ReceivedChars = 0;
  unsigned char Char = 0;

  if (NULL == buff)
  {
    Serial.printf("ERROR - NULL PTR buffer");
    return false;
  }

  while (swSer.available()) {
    Char = swSer.read();

    if (ReceivedChars == 0 && (Char == '\r')) return false;
    if (ReceivedChars == 0 && (Char == '\n')) return false;

    if ( (Char == '\n') || (Char == '\r') ) {
      ReceivedChars = 0;
      return true;
    }

    buff[ReceivedChars] = Char;
    buff[ReceivedChars + 1] = 0;
    ReceivedChars++;
    if (ReceivedChars >= MAX_NMEA0183_MESSAGE_SIZE - 2) {
      ReceivedChars = 0;
      buff[0] = 0;
      return false;
    }
  }
  return false;
}   //GetNMEA0183Message1







// UARTS testprint, to be used in terms of uart verification
//------------------------------------------------------------------
void UARTS_Verification() {
  char ch = 'q';
  while (true) {
    while (Serial.available() > 0) {  //wait for data at hardware serial
      ch = Serial.read();
      swSer1.write(ch);  //send data recived from hardware serial to software serial
      swSer2.write(ch);  //send data recived from hardware serial to software seriale
    }
    ch = 'q';
    while (swSer1.available() > 0) {        //wait for data at software serial
      Serial.write("1>");                   //Send data recived from software serial to hardware serial
      Serial.println((char)swSer1.read());  //Send data recived from software serial to hardware serial
    }
    ch = 'q';
    while (swSer2.available() > 0) {        //wait for data at software serial
      Serial.write("2>");                   //Send data recived from software serial to hardware serial
      Serial.println((char)swSer2.read());  //Send data recived from software serial to hardware serial
    }
  }
}   //UARTS_Verification




void loop() {
  //UARTS_Verification();

  // look for Client connect trial
  if (telnetServer.hasClient()) {
    if (!serverClient || !serverClient.connected()) {
      if (serverClient) {
        serverClient.stop();
        Serial.println("Telnet Client Stop");
      }
      serverClient = telnetServer.available();
      Serial.println("New Telnet client");
      serverClient.flush();  // clear input buffer, else you get strange characters 
    }


  // FACKUP  - there is no TELNET client and YOU ARE WRITING  serverClient.println(buf1);
  // PUT HERE :
  //    serverClient.println(buf1);    // Send to clients
  //  Serial.println(buf1);



  }

    // Do not need to read Yet
    //while(serverClient.available()) {  // get data from Client
    //Serial.write(serverClient.read());
    //}

  //bool GetNMEA0183_Message(SoftwareSerial &swSer, char * buff) {




// FACKUP  - there is no TELNET client and YOU ARE WRITING  serverClient.println(buf1);
// HERE - JUST SERIAL WRITE OF GetNMEA0183_Message(swSer1,buf1)

  if (GetNMEA0183_Message(swSer1,buf1) == true) {    // Get NMEA sentences from serial#1
    serverClient.println(buf1);    // Send to clients
    Serial.println(buf1);
  }


  if (GetNMEA0183_Message(swSer2,buf1) == true) {    // Get NMEA sentences from serial#2
    serverClient.println(buf1);    // Send to clients
    Serial.println(buf1);
  }


/*
  if (millis() - startTime > 2000) { // run every 2000 ms
    startTime = millis();

    if (serverClient && serverClient.connected()) {  // send data to Client
      serverClient.print("Telnet Test, millis: ");
      serverClient.println(millis());
      serverClient.print("Free Heap RAM: ");
      serverClient.println(ESP.getFreeHeap());
    }
  }
*/
  //delay(10);  // to avoid strange characters left in buffer WiFi

}