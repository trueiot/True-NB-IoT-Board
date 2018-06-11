/*
TRUE_NB_BC95 v1.0
Author: True IoT
Create Date: 1 May 2018
Modified: 16 May 2018
Released for Maker and Developer
Modified : 2018.JUN.1
Re-coding to compatible with IoTtweet.com
*/


#include "True_NB_bc95.h"

bool True_NB_bc95::reboot() {
  MODEM_SERIAL->println("AT+NRB");
}

void True_NB_bc95::init(Stream &serial) {
  MODEM_SERIAL = &serial;
}

String True_NB_bc95::getIMEI()
{
  MODEM_SERIAL->println(F("AT+CGSN=1"));
  char waitForCGSN[] = "+CGSN:";
  String re_str;
  re_str = expect_rx_str(1000, waitForCGSN, 6);

  if (re_str != "") {
    return re_str;
  }
  return "";
}

String True_NB_bc95::getIMSI()
{
  MODEM_SERIAL->println(F("AT+CIMI"));
  char waitForMISI[] = "\r\n";
  String re_str;
  re_str = expect_rx_str(1000, waitForMISI, 2);

  if (re_str != "") {
    return re_str;
  }
  return "";
}


bool True_NB_bc95::check_match( char target[], char pattern[], int len_check) {
  int target_length = strlen(target);
  int count = 0;
  for (int i = 0; i < target_length; i++) {
    // find the 1st char in target
    if ( target[i] == pattern[0] ) {
      // start at index of target[index] that match[pattern]
      // loop and count the match char
      for (int j = 0; j < len_check; j++) {
        if ( target[i] == pattern[j]) {
          count++;
          i++;
        }
      }// for loop all char in pattern
    } // if target
  }// for
  if ( count == len_check ) {
    return true;
  }
  return false;
}


int True_NB_bc95::check_match_index( char target[], char pattern[], int len_check) {
  int target_length = strlen(target);
  int count = 0;
  int i = 0, index = 0;
  for (i = 0; i < target_length; i++) {
    // find the 1st char in target
    if ( target[i] == pattern[0] ) {
      // start at index of target[index] that match[pattern]
      // loop and count the match char
      for (int j = 0; j < len_check; j++) {
        if ( target[i] == pattern[j]) {
          count++;
          index = i;
          i++;
        }
      }// loop all char in pattern
    }
  }
  if ( count == len_check ) {
    return index;
  }
  return -1;
}


String True_NB_bc95::expect_rx_str( unsigned long period, char exp_str[], int len_check) {
  unsigned long cur_t = millis();
  unsigned long start_t = millis();
  bool str_found = 0;
  bool time_out = 0;
  bool loop_out = 0;
  int i = 0;
  int found_index = 0, end_index = 0;
  int modem_said_len = 0;
  char c;
  char modem_said[MODEM_RESP];
  String re_str;
  char str[BUF_MAX_SIZE];
  char *x;

  while ( !loop_out ) {
    if (MODEM_SERIAL->available()) {
      c = MODEM_SERIAL->read();
      modem_said[i++] = c;
    }
    else {
    }
    cur_t = millis();
    if ( cur_t - start_t > period ) {
      time_out = true;
      start_t = cur_t;
      loop_out = true;
    }
  }//while
  modem_said[i] = '\0';
  end_index = i;
  x = strstr(modem_said, exp_str) ;
  found_index = x ? x - modem_said : -1;
  if ( found_index >= 0  ) {
    i = 0;
    while ( modem_said[found_index + i + len_check] != 0x0D | i == 0) {
      str[i] = modem_said[found_index + i + len_check];
      re_str += String(str[i]);
      i++;
    }
    str[i] = '\0';
    return re_str;
  }
  return "";
}


bool True_NB_bc95::initModem() {
  Serial.println(F("######### True_NB_BC95 Library by TrueIoT V1.0 ##########"));
  Serial.println( "initial Modem to connect NB-IoT Network" );
  MODEM_SERIAL->println(F("AT+NRB"));
  delay(5000);
  char waitForReboot[] = "REBOOT";
  if ( expect_rx_str(2000, waitForReboot, 6) != "" ) {
    Serial.println("Reboot done Connecting to Network");
  }
  MODEM_SERIAL->println(F("AT+CFUN=1"));
  delay(2000);
  MODEM_SERIAL->println(F("AT"));
  delay(1000);
}

bool True_NB_bc95::check_modem_status() {

  MODEM_SERIAL->println(F("AT"));
  char waitForOK[] = "OK";
  if ( expect_rx_str(2000, waitForOK, 2) != "" ) {
    return true;
  }
  return false;
}

bool True_NB_bc95::register_network() {
  bool regist = 0;
  delay(2000);
  MODEM_SERIAL->println(F("AT+CGATT=1"));
  delay(3000);
  MODEM_SERIAL->println(F("AT+CGATT?"));
  delay(1000);
  char waitForCGATT[] = "+CGATT:1";
  if (expect_rx_str(1000, waitForCGATT, 8 ) != "" ) {
    Serial.println("regiester network Done");
    return true;
  }
  else {
    Serial.println("register network Fail!");
    return false;
  }
}

String True_NB_bc95::check_ipaddr() {
  MODEM_SERIAL->println(F("AT+CGPADDR=0"));
  delay(1000);
  char waitForCGPADDR[] = "+CGPADDR:0,";
  String re_str;
  re_str = expect_rx_str(1000, waitForCGPADDR, 11 );
  if ( re_str != "" ) {
    return re_str;
  }
  return "";
}

bool True_NB_bc95::check_incoming_msg() {
  char strCmd[BUF_MAX_SIZE / 2];
  bool found = false;
  int totalSocket = 1;
  String checkIncmd = "AT+NSORF= ,8\r\n";
  for (int j = 0; j < totalSocket; j++) {
    for (int i = 0; i < 14; i++) {
      if (i == 9) {
        MODEM_SERIAL->print(j);
      }
      else {
        strCmd[i] = checkIncmd[i];
        MODEM_SERIAL->print( strCmd[i]);
      }
    }// for i
  }//for j
  char waitForNSONMI[] = "+NSONMI:0,8";
  if ( expect_rx_str(1000, waitForNSONMI, 11) != "" ) {
    //Serial.println( " There is a message need to read" );
    return true;
  }
  return false;
}

int True_NB_bc95::check_modem_signal() {
  char resp_result[BUF_MAX_SIZE];
  int index = 0;
  int ssi;
  char ssi_str[3];
  MODEM_SERIAL->println("AT+CSQ");
  char waitForCSQ[] = "+CSQ:";
  String re_str;
  re_str = expect_rx_str( 1000, waitForCSQ, 5);
  if ( re_str != "" ) {
    ssi_str[0] = re_str[0];
    // check the next char is not "," It is not single digit
    if ( re_str[1] != 0x2c) {
      //Serial.println( resp_result[index+2]);
      ssi_str[1] = re_str[1];
      ssi_str[2] = '\0';
      ssi = atoi(ssi_str);
      ssi = -1 * (113 - ssi * 2);
      return ssi;
    }
    // it is single digit
    ssi_str[1] = '\0';
    ssi = atoi(ssi_str);
    ssi = -1 * (113 - ssi * 2);
    return ssi;
  }
  else {
    return -200;
  }

}

bool True_NB_bc95::create_UDP_socket(int port, char sock_num[]) {

  MODEM_SERIAL->print(F("AT+NSOCR=DGRAM,17,"));
  MODEM_SERIAL->print( port );
  MODEM_SERIAL->println(F(",1"));
  delay(2000);
  if ( expect_rx_str(1000, sock_num, 1) != "" ) {
    return true;
  }
  return false;
}

bool True_NB_bc95::postRequest(char token[], char payload[], CoapPacketTrueIoT *packet) {

  int token_len, payload_len;
  token_len = strlen(token);
  payload_len = strlen(payload);
  uint8_t header_token_len = 0x04;
  packet->ver = COAP_VER;
  packet->type = COAP_TYPE_CON;
  packet->ver_type_tokenlen = packet->ver << 6 | (packet->type & 0x03) << 4 | (header_token_len & 0x0F) ;
  packet->code[0] = 0x30;
  packet->code[1] = COAP_POST | 0x30;
  for (int i = 0; i < 4; i++) {
    packet->messageid[i] = random(0, 9) | 0x30;
  }
  for (int i = 0; i < 8; i++) {
    packet->token[i] = random(0, 9) | 0x30;
  }
  packet->option_1_name_path[0] = 0x42;
  packet->option_1_name_path[1] = 0x33;
  packet->option_1_name_path[2] = 0x36;
  packet->option_1_name_path[3] = 0x31;
  packet->option_1_name_path[4] = 0x37;
  packet->option_1_name_path[5] = 0x30;
  packet->option_1_name_path[6] = 0x36;
  packet->option_1_name_path[7] = 0x39;
  packet->option_2_length[0] = 0x30;
  packet->option_2_length[1] = 0x32;
  packet->option_2_ver[0] = 0x37;
  packet->option_2_ver[1] = 0x36;
  packet->option_2_ver[2] = 0x33;
  packet->option_2_ver[3] = 0x31;
  packet->option_3_dev_token_len[0] = 0x30;
  packet->option_3_dev_token_len[1] = 0x44;
  packet->option_3_dev_token_ext_len[0] = 0x30;
  packet->option_3_dev_token_ext_len[1] = 0x37;

  for ( int i = 0; i < token_len; i++) {
    packet->option_3_dev_token[i] = token[i];
  }
  packet->option_4_len_telemetry_word[0] = 0x30;
  packet->option_4_len_telemetry_word[1] = 0x39;
  packet->option_4_telemetry_word[0] = 0x37;
  packet->option_4_telemetry_word[1] = 0x34;
  packet->option_4_telemetry_word[2] = 0x36;
  packet->option_4_telemetry_word[3] = 0x35;
  packet->option_4_telemetry_word[4] = 0x36;
  packet->option_4_telemetry_word[5] = 0x43;
  packet->option_4_telemetry_word[6] = 0x36;
  packet->option_4_telemetry_word[7] = 0x35;
  packet->option_4_telemetry_word[8] = 0x36;
  packet->option_4_telemetry_word[9] = 0x44;
  packet->option_4_telemetry_word[10] = 0x36;
  packet->option_4_telemetry_word[11] = 0x35;
  packet->option_4_telemetry_word[12] = 0x37;
  packet->option_4_telemetry_word[13] = 0x34;
  packet->option_4_telemetry_word[14] = 0x37;
  packet->option_4_telemetry_word[15] = 0x32;
  packet->option_4_telemetry_word[16] = 0x37;
  packet->option_4_telemetry_word[17] = 0x39;
  packet->option_4_endmask[0] = 0x46;
  packet->option_4_endmask[1] = 0x46;

  for ( int i = 0; i <= payload_len; i++) {
    packet->buffer[i] = payload[i];
  }


}

bool True_NB_bc95::sendUDPPacket2(char socknum[], String remoteIP, int remotePort, CoapPacketTrueIoT *packet, int json_len) {

  char buff[BUF_MAX_SIZE];
  // count header until 0XFF end mask  before json data, len=95
  int len = 96;
  len = (len + json_len * 2) / 2;
  sprintf(buff, "%s,%s,%d,%d,", socknum, remoteIP.c_str(), remotePort, len);
  MODEM_SERIAL->print(F("AT+NSOST="));
  MODEM_SERIAL->print(buff);


  MODEM_SERIAL->print( packet->ver_type_tokenlen, HEX);
  MODEM_SERIAL->write( packet->code, 2);
  MODEM_SERIAL->write( packet->messageid, 4);
  MODEM_SERIAL->write( packet->token, 8);
  MODEM_SERIAL->write( packet->option_1_name_path, 8 );
  MODEM_SERIAL->write( packet->option_2_length, 2);
  MODEM_SERIAL->write( packet->option_2_ver, 4);
  MODEM_SERIAL->write( packet->option_3_dev_token_len, 2);
  MODEM_SERIAL->write( packet->option_3_dev_token_ext_len, 2);

  for (int i = 0; i < 20; i++) {
    MODEM_SERIAL->print( packet->option_3_dev_token[i], HEX);
  }
  MODEM_SERIAL->write( packet->option_4_len_telemetry_word, 2);
  MODEM_SERIAL->write( packet->option_4_telemetry_word, 18);
  MODEM_SERIAL->write( packet->option_4_endmask, 2);

  for (int i = 0; i < json_len; i++) {
    MODEM_SERIAL->print( packet->buffer[i], HEX);
  }
  MODEM_SERIAL->print("\r\n");

}

bool True_NB_bc95::sendUDPstr(String ip, String port, String data) {

  int str_len = data.length();
  char buffer[str_len+2];
  data.toCharArray(buffer, str_len+1);

  /* Start AT command */
  MODEM_SERIAL->print(F("AT+NSOST=0"));
  MODEM_SERIAL->print(F(","));
  MODEM_SERIAL->print(ip);
  MODEM_SERIAL->print(F(","));
  MODEM_SERIAL->print(port);
  MODEM_SERIAL->print(F(","));
  MODEM_SERIAL->print(String(str_len));
  MODEM_SERIAL->print(F(","));

  /* Fetch print data in hex format */
  char *h_buf;
  h_buf = buffer;
  char fetch[3] = "";
  bool chk = false;
  int i=0;

  while(*h_buf)
  {
    chk = itoa((int)*h_buf,fetch,16);
    if(chk){
      MODEM_SERIAL->print(fetch);
    }
    h_buf++;
  }
  MODEM_SERIAL->print("\r\n");

}

String True_NB_bc95::WriteDashboardIoTtweet(String userid, String key, float slot0, float slot1, float slot2, float slot3, String tw, String twpb){

  _userid = userid;
  _key = key;
  _slot0 = slot0;
  _slot1 = slot1;
  _slot2 = slot2;
  _slot3 = slot3;
  _tw = tw;
  _twpb = twpb;

  Serial.println("------ Send to Cloud.IoTtweet -------");
             _packet = _userid;
             _packet += ":";
             _packet += _key;
             _packet += ":";
             _packet += String(_slot0);
             _packet += ":";
             _packet += String(_slot1);
             _packet += ":";
             _packet += String(_slot2);
             _packet += ":";
             _packet += String(_slot3);
             _packet += ":";
             _packet += _tw;
             _packet += ":";
             _packet += _twpb;

  Serial.println("packet sent : " + String(_packet));
  Serial.println("--------------------------------------");

  sendUDPstr(IoTtweetNBIoT_HOST, IoTtweetNBIoT_PORT, _packet);
  return "OK";

}
