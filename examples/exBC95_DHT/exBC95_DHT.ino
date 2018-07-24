/* 
  //True IoT  Date 11/05/2018
  //Example program for connect DHT22 (Arduino Pin7) get  Teperature,Humidity and get NB-IoT RSSI  to show on LCD (I2C-20x4)
*/

#include "True_NB_bc95.h"
#include <Wire.h>
#include <DHT.h>;
#include <LiquidCrystal_I2C.h>
#include <AltSoftSerial.h>
AltSoftSerial Serial2;

//Constants of Temperature & Humidity Sensor
#define DHTPIN 7     // what pin we're connected to pin 7's Arduino Uno R3
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;
int humidity;  //Stores humidity value
int temperature; //Stores temperature value
int rssi; //Stores RSSI value
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars and 4 line display

True_NB_bc95 modem;
//IP 188.166.189.39 for http://trueiot.io
//String udpRemoteIP = "188.166.189.39";
// IP for 104.196.24.70 http://demo.thingsboard.io
String udpRemoteIP = "104.196.24.70";
int udpRemotePort = 5683;
//char iotToken[] = "3Pem0KqnuP1X71twsGjy\0";
//char iotToken[] = "QdqdgVPxwNyffJX0AYyV\0";   //true
char iotToken[] = "JlVAIC4gjwHRYUetR7wX\0";  //demo

//Define json format
char jsonData[] = "{\"temperature\":%d, \"humidity\":%d,\"rssi\":%d}\0";   //Define json format
char buff[48];
int jsonData_len = 0;

long start = 0;
CoapPacketTrueIoT coap;
char sock[] = "0\0";
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  lcd.begin();    //init LCD
  lcd.backlight();
  dht.begin();      //init DHT 22
  delay(3000);
  Serial.println("Starting to connect NB-IoT Network...");
  modem.init(Serial2);
  modem.initModem();
  Serial.println( "IMEI = " + modem.getIMEI() );
  Serial.println( "IMSI = " + modem.getIMSI() );
  
  while (!modem.register_network());
  delay(1000);
  Serial.println( "Device IP = " + modem.check_ipaddr() );
  modem.create_UDP_socket( 4700, sock);

}


void loop() {
  delay(3000);

  //Read Sensor values
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  rssi =  modem.check_modem_signal();
  //
  lcd.setCursor(0, 0);
  lcd.print("   True NB-IoT  ");

  lcd.setCursor(0, 1);
  humidity = dht.readHumidity();
  lcd.print("Humidity    : ");
  lcd.print(humidity);
  lcd.print("%");

  lcd.setCursor(0, 2);
  temperature = dht.readTemperature();
  lcd.print("Temperature : ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 3);
  rssi = modem.check_modem_signal();
  lcd.print("RSSI        : ");
  //lcd.print("-");
  lcd.print(rssi);
  lcd.print(" dB");

  modem.check_incoming_msg();
  delay(3000);
  Serial.println( modem.check_modem_signal() );

  //***************** if using Thingsboard CoAP************************************
  if (millis() - start > 5000) {
    start = millis();
    //sprintf(buff, jsonData, (int)random(10, 20), (int)random(50, 60) );   //Used this line if you have not DHT
    sprintf(buff, jsonData, (int)temperature, (int)humidity, (int)rssi  );
    jsonData_len = strlen(buff);
    Serial.println(buff);
    modem.postRequest( iotToken, buff, &coap );
    modem.sendUDPPacket2( sock, udpRemoteIP, udpRemotePort, &coap, jsonData_len);
  }

}


