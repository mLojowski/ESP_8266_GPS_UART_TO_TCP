#include "WiFi_auth.h"
#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <time.h>

#include "NMEA0183/NMEA0183.h"

#define MAX_NMEA0183_MESSAGE_SIZE 150

// provide text for the WiFi status
static const char *str_status[]= {
  "WL_IDLE_STATUS",
  "WL_NO_SSID_AVAIL",
  "WL_SCAN_COMPLETED",
  "WL_CONNECTED",
  "WL_CONNECT_FAILED",
  "WL_CONNECTION_LOST",
  "WL_DISCONNECTED"
};

// provide text for the WiFi mode
static const char *str_mode[]= { "WIFI_OFF", "WIFI_STA", "WIFI_AP", "WIFI_AP_STA" };

// connectWiFi()
void connectWifi(bool);

//  GetNMEA0183_Message
// BLOCKING
bool GetNMEA0183_Message(SoftwareSerial &swSer, char * buff, unsigned int &ReceivedChars);