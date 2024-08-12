//#include "NMEA0183/NMEA0183.h"
#include "MY_WiFi.h"

//MACRO DEFNITIONS - UART's
//----------------------------------------------
// Serial = DEBUG
#define DEBUG_Baud 115200

// Ser1 = GPS_SERIAL
#define Ser1_RX_PIN 4
#define Ser1_TX_PIN 5
#define Ser1_Baud 4800

// Ser2 = GPS_SERIAL
#define Ser2_RX_PIN 13
#define Ser2_TX_PIN 12
#define Ser2_Baud 4800

// Ser3 ....
//#define Ser2_RX_PIN 00
//#define Ser2_TX_PIN 02


// Create software serial objects
SoftwareSerial swSer1(Ser1_RX_PIN, Ser1_TX_PIN);
SoftwareSerial swSer2(Ser2_RX_PIN, Ser2_TX_PIN);
//----------------------------------------------


//MACRO DEFNITIONS - NMEA0183
//----------------------------------------------

// NMEA interface input buffer definitions
char buf1[MAX_NMEA0183_MESSAGE_SIZE] = "";  // Buffer for serial#1
char buf2[MAX_NMEA0183_MESSAGE_SIZE] = "";  // Buffer for serial#2



//MACRO DEFNITIONS - WiFi's
//----------------------------------------------

// declare telnet server (do NOT put in setup())
WiFiServer telnetServer(23);
WiFiClient serverClient;
#define AP_MODE true


//unsigned long startTime = millis();

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
  swSer2.begin(Ser2_Baud);  //Initialize software serial with baudrate of 4800


  //INIT:
  Serial.println("\nESP8266 Software serial config started");


  Serial.println("Sync,Sync,Sync,Sync,Sync Wifi startup");
  delay(500);
  Serial.println();

  Serial.print("Chip ID: 0x");
  Serial.println(ESP.getChipId(), HEX);

  Serial.println("Connect to Router requested");
  connectWifi(AP_MODE);

  // IF WIFI IS IN AP MODE -> CHECK HERE
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
    Serial.print("Ser_1> ");
    Serial.println(buf1);
  }


  if (GetNMEA0183_Message(swSer2,buf1) == true) {    // Get NMEA sentences from serial#2
    serverClient.println(buf1);    // Send to clients
    Serial.print("Ser_2> ");
    Serial.println(buf1);
  }


  //delay(10);  // to avoid strange characters left in buffer WiFi

}