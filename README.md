CHECKOUT:
- create dir NMEA0183 - Arduino related issue (need to name ino same as the Dir wit ino within)

  git clone ..
  git checkout main

- git submodule init
- git submodule update

  cd NMEA0183 ( -> origin  https://github.com/ttlappalainen/NMEA0183.git)
  git checkout master
  git pull origin master


ESP_8266_GPS_UART_TO_TCP

Credits:
UART -> https://docs.arduino.cc/learn/built-in-libraries/software-serial/
NMEA_0183 Multiplexer -> https://github.com/AK-Homberger/NMEA0183-WiFi-Multiplexer/tree/main
NMEA_0183 LIB -> https://github.com/ttlappalainen/NMEA0183
ESP_telnet example -> https://github.com/esp8266/Arduino/issues/1169 (tcp info)


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

