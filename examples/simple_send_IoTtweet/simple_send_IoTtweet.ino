#include "True_NB_bc95.h"
#include <AltSoftSerial.h>

AltSoftSerial Serial2;

True_NB_bc95 modem;

String userid = "your-IoTtweet-account-ID";       /*IoTtweet account user ID (6 digits, included zero pre-fix)*/
String key = "your-device-key";                   /*IoTtweet registered device key in "MY IOT Garage"*/
float data0, data1, data2, data3;                 /*Your sending data variable.*/
String private_tweet = "TRUE NB-IoT";             /*Your private tweet meassage to dashboard*/
String public_tweet = "Hello IoTtweet";           /*Your public tweet message to dashboard*/

long start = 0;
char sock[] = "0\0";

void setup() {
  
  Serial.begin(9600);
  Serial2.begin(9600);
  
  delay(3000);
  
  Serial.println("Starting...");
  
  modem.init(Serial2);
  modem.initModem();
  
  Serial.println( "IMEI = " + modem.getIMEI() );
  Serial.println( "IMSI = " + modem.getIMSI() );
  
  while (!modem.register_network());
  delay(1000);
  
  Serial.println( modem.check_ipaddr() );
  modem.create_UDP_socket( 4700, sock);
  
}

void loop() {
  if (millis() - start > 10000) {
    start = millis();

    /* Example data generating */
      data0 = random(20,80);
      data1 = random(30,70);
      data2 = random(40,60);
      data3 = random(50,55);

    /* Send data to IoTtweet dashboard */
    modem.WriteDashboardIoTtweet(userid, key, data0, data1, data2, data3, private_tweet, public_tweet);
  }
}
