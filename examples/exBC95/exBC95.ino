#include "True_NB_bc95.h"
#include <AltSoftSerial.h>
AltSoftSerial Serial2;


True_NB_bc95 modem;

// IP for 104.196.24.70 http://demo.thingsboard.io
//String udpRemoteIP = "104.196.24.70";
String udpRemoteIP = "188.166.189.39";
int udpRemotePort = 5683;

char iotToken[] = "ngSQG83rFeo9nVsvj0Bx\0";  //Used your Token from ThingsBoard ,see on ThingsBoard User Guide

char jsonData[] = "{\"temperature\":%d, \"humidity\":%d }\0";
char buff[48];
int jsonData_len = 0;

long start = 0;
CoapPacketTrueIoT coap;
char sock[] = "0\0";
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  delay(3000);
  Serial.println("Starting...");
  modem.init(Serial2);
  modem.initModem();
  Serial.println( modem.getIMEI() );
  while (!modem.register_network());
  delay(1000);
  Serial.println( modem.check_ipaddr() );
  modem.create_UDP_socket( 4700, sock);

}


void loop() {
  delay(3000);
  //modem.check_incoming_msg();
  delay(3000);
  Serial.println( modem.check_modem_signal() );
  //***************** if using Thingsboard CoAP************************************
  if (millis() - start > 9000) {
    start = millis();
    sprintf(buff, jsonData, (int)random(10,20), (int)random(50,60) );
    jsonData_len = strlen(buff);
    Serial.println(buff);
    modem.postRequest( iotToken, buff, &coap );
    modem.sendUDPPacket2( sock, udpRemoteIP, udpRemotePort, &coap, jsonData_len);
  }

}


