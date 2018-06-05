#include "True_NB_bc95.h"
#include <AltSoftSerial.h>

#include "DHT.h"

#define DHTpin 7  /* Connect DHT out to Pin 7 */
#define DHTType DHT22
DHT dht(DHTpin, DHTType);

AltSoftSerial Serial2;

True_NB_bc95 modem;

String userid = "your-IoTtweet-account-ID";       /*IoTtweet account user ID (6 digits, included zero pre-fix)*/
String key = "your-device-key";                   /*IoTtweet registered device key in "MY IOT Garage"*/
float data0, data1, data2, data3;                 /*Your sending data variable.*/
String private_tweet = "TRUE NB-IoT";             /*Your private tweet meassage to dashboard*/
String public_tweet = "Data from DHT22 sensor";   /*Your public tweet message to dashboard*/

long start = 0;
char sock[] = "0\0";

float t, h;

void setup() {
  
  Serial.begin(9600);
  Serial2.begin(9600);
  dht.begin();
  
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

      t = dht.readTemperature();
      h = dht.readHumidity();

      if(isnan(t) || isnan(h)){
        Serial.println("Sensor read failed !");
        return;
      }

      Serial.println("t = " + String(t) + " celcuis");
      Serial.println("h = " + String(h) + " %RH");
      Serial.println();

    /* Send data to IoTtweet dashboard */
    modem.WriteDashboardIoTtweet(userid, key, t, h, 0, 0, private_tweet, public_tweet);
  }
}
