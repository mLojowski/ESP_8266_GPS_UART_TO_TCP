ESP_8266_GPS_UART_TO_TCP

Credits:
UART -> https://docs.arduino.cc/learn/built-in-libraries/software-serial/
NMEA_0183 Multiplexer -> https://github.com/AK-Homberger/NMEA0183-WiFi-Multiplexer/tree/main
NMEA_0183 LIB -> https://github.com/ttlappalainen/NMEA0183
ESP_telnet example -> https://github.com/esp8266/Arduino/issues/1169 (tcp info)


INITIAL COMMIT: d89e6666d33c05f7785f6168dce31eb38ba97388

SoftwareSerial configuration for pins 
Ser1_RX_PIN 5
Ser1_TX_PIN 4

Ser2_RX_PIN 12
Ser2_TX_PIN 13

+ UARTS_Verification method with 
EXAMPLE OUTPUT:

 ESP8266 Software serial test started
1>1
2>1
1>2
2>2
1>3
2>3

