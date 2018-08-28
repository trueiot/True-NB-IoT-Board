#include "True_NB_bc95.h"
#include <AltSoftSerial.h>

AltSoftSerial Serial2;

String check_udp_incoming();

True_NB_bc95 modem;

String ubidots_serv_ip = "50.23.124.66"; /* translate.ubidots.com */
String ubidots_serv_port = "9012";

String useragent = "nb001/1.1";  // Up to you...
String token = "Token Key here"; // Assign your Ubidots TOKEN
String device_name = "nb001"; // Assign the unique device label (device name)
String val_name3 = "lamp:";
//String data_u = "";

long start = 0;
long start2 = 0;
char sock[] = "0\0";
int  lamp_stts = 0; // 0:off, 1:on
bool rec_flag = false;

void setup() {

  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(13, OUTPUT);
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

void loop()
{
  String str_lamp = "";
  String _packet = "";
  int data0, data1, data2, data3;


  /*** Retirve lamp status ***/
  if (millis() - start2 > 5000)  // 5 sec
  {
    start2 = millis();

    _packet = useragent + "|LV|" + token + "|" + device_name + ":lamp" + "|end";
    //Serial.println("retrive:" + _packet);
    modem.sendUDPstr(ubidots_serv_ip, ubidots_serv_port, _packet);
  }


  // get lamp status
  str_lamp = check_udp_incoming();
  if ( str_lamp == "31") {
    lamp_stts = 1; // On
    digitalWrite(13, HIGH);
  }
  else if ( str_lamp == "30") {
    lamp_stts = 0; // Off
    digitalWrite(13, LOW);
  }


}

String check_udp_incoming()
{
  String tmp_buf;
  String data_in;
  String str_lampstts = "";
  int indx = 0;

  if (Serial2.available())
  {
    tmp_buf = Serial2.readString();
    //Serial.println("incomeing" + tmp_buf);
    if (tmp_buf.indexOf("+NSONMI") > 0)
    {
      Serial2.println("AT+NSORF=0,8"); // send read data from modem
      delay(300);
      if (Serial2.available())
      {
        //Serial.println("incomeing2");
        data_in = Serial2.readString();
        indx = data_in.indexOf("7C"); // find '|'
        str_lampstts = data_in.substring(indx + 2, indx + 4);
        Serial.println("data:" + str_lampstts);
      }
    }
  }
  return str_lampstts;
}

