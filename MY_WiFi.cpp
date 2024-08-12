#include "IPAddress.h"
#include "MY_WiFi.h"


void connectWifi(bool AP_MODE) {

  if (AP_MODE)
  {
    Serial.println("Start WLAN AP");         // WiFi Mode AP
    WiFi.mode(WIFI_AP);
    IPAddress Local_ip = {192,168,8,4};
    IPAddress gateway = {192,168,8,1};
    IPAddress subnet = {255,255,255,0};

    //WiFi.softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);

    if (WiFi.softAPConfig(Local_ip, gateway, subnet))
    {
      Serial.println("WiFi.softAPConfig TRUE");
    }
    else
    {
      Serial.println("WiFi.softAPConfig FALSE");
    }

    WiFi.softAP(ssid, password);
    delay(100);
    IPAddress IP = WiFi.softAPIP();
    Serial.println("");
    Serial.print("AP IP address: ");
    Serial.println(IP);
  }
  else
  {
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

  }
}  // connectWiFi()


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

