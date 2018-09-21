/*

TRUE_NB_BC95 v1.0
Author: True IoT
Create Date: 1 May 2018
Modified: 16 May 2018

Modified : 2018.JUN.1
Re-coding to compatible with IoTtweet.com

*/


#include "True_NB_bc95.h"


bool True_NB_bc95::reboot() {
  sprintf_P(&(_modem_buf[0]), PSTR("AT+NRB"));
  return ( exec_modem_command(&(_modem_buf[0]), "Neul \r\nOK", "\r\n", 15000, 15000) != "ERROR" );
}


void True_NB_bc95::init(Stream &serial) {
  MODEM_SERIAL = &serial;
}


String True_NB_bc95::getIMEI()
{
  sprintf_P(&(_modem_buf[0]), PSTR("AT+CGSN=1"));
  return ( exec_modem_command(&(_modem_buf[0]), "+CGSN:", "\r\n", 1000, 1000) );
}


String True_NB_bc95::getIMSI()
{
  sprintf_P(&(_modem_buf[0]), PSTR("AT+CIMI"));
  return ( exec_modem_command(&(_modem_buf[0]), "\r\n", "\r\n", 1000, 1000) );
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


String True_NB_bc95::expect_rx_str(char start_tag[], char end_tag[],  unsigned long timeout_ms) {
  unsigned long end_t = millis() + timeout_ms;
  int i = -1;
  int end_tag_index = -1;
  int start_tag_length = strlen(start_tag);
  int end_tag_length = strlen(end_tag);
  char c;

  do
  {
    if ( MODEM_SERIAL->available() ) {
      c = MODEM_SERIAL->read();

      if ( i + 1 < BUF_MAX_SIZE ) {
        ++i;
        _modem_buf[i] = c;
      }

      if ( (i + 1) == start_tag_length ) {
        if ( memcmp(&(_modem_buf[0]), &(start_tag[0]), start_tag_length) != 0 ) {
          for ( int j = 1; j < start_tag_length; ++j ) {
            _modem_buf[j - 1] = _modem_buf[j];
          }
          --i;
        }
      }

      if ( ((i + 1) >= (start_tag_length + end_tag_length))
      &&   (memcmp(&(_modem_buf[(i + 1) - end_tag_length]), &(end_tag[0]), end_tag_length) == 0) ) {
        end_tag_index = (i + 1) - end_tag_length;
      }
    }
  } while ( (end_tag_index == -1) && (millis() < end_t) );

  if ( end_tag_index != -1 )  {
    if ( end_tag_index > start_tag_length ) {
      _modem_buf[end_tag_index] = '\0';

      return String(&(_modem_buf[start_tag_length]));
    }
    else
    {
      return "OK";
    }
  }

  return "ERROR";
}


String True_NB_bc95::exec_modem_command(char cmd[], char start_tag[], char end_tag[],  unsigned long timeout_ms,  unsigned long post_delay_ms) {
  unsigned long end_t;
  String re_str;

  flush_modem_serial(2);

  MODEM_SERIAL->println(cmd);

  re_str = expect_rx_str(start_tag, end_tag, timeout_ms);

  if ( (re_str != "ERROR") && (post_delay_ms > 0) )
  {
    delay(post_delay_ms);
  }

  return ( re_str );
}


bool True_NB_bc95::initModem() {
  Serial.println(F("######### True_NB_BC95 Library by TrueIoT V1.0 ##########"));
  Serial.println(F("Setting up modem to connect NB-IoT Network..."));

  Serial.print(F("Rebooting modem, "));
  if ( reboot() ) {
    Serial.println(F("OK."));
  }
  else
  {
    Serial.println(F("FAIL."));
  }

  Serial.print(F("Setting full functionality, "));
  sprintf_P(&(_modem_buf[0]), PSTR("AT+CFUN=1"));
  if ( exec_modem_command(&(_modem_buf[0]), "OK", "\r\n", 1000, 2000) != "ERROR" )  {
    Serial.println(F("OK."));
  }
  else
  {
    Serial.println(F("FAIL."));
  }

  Serial.print(F("Requesting modem ACK, "));
  if ( check_modem_status() ) {
    Serial.println(F("OK."));
  }
  else
  {
    Serial.println(F("FAIL."));
  }
}


bool True_NB_bc95::check_modem_status() {
  sprintf_P(&(_modem_buf[0]), PSTR("AT"));
  return ( exec_modem_command(&(_modem_buf[0]), "OK", "\r\n", 1000, 1000) != "ERROR" );
}


bool True_NB_bc95::register_network() {
  Serial.print(F("Registering network, "));
  sprintf_P(&(_modem_buf[0]), PSTR("AT+CGATT=1"));
  if ( exec_modem_command(&(_modem_buf[0]), "OK",       "\r\n", 5000, 3000) != "ERROR" ) {
    sprintf_P(&(_modem_buf[0]), PSTR("AT+CGATT?"));
    if ( exec_modem_command(&(_modem_buf[0]), "+CGATT:1", "\r\n", 5000, 1000) != "ERROR" ) {
      Serial.println(F("OK."));
      return true;
    }
  }

  Serial.println(F("FAIL."));
  return false;
}


String True_NB_bc95::check_ipaddr() {
  sprintf_P(&(_modem_buf[0]), PSTR("AT+CGPADDR=0"));
  return ( exec_modem_command(&(_modem_buf[0]), "+CGPADDR:0,", "\r\n", 1000, 1000) );
}


bool True_NB_bc95::check_incoming_msg() {
  String re_str;

  int totalSocket = 1;
  for (int j = 0; j < totalSocket; j++) {
    sprintf(&(_modem_buf[0]), "AT+NSORF=%d,8", j);
    re_str = exec_modem_command(&(_modem_buf[0]), "+NSONMI:0,8", "\r\n", 1000, 1000);
    if ( re_str != "ERROR" ) {
      return true;
    }
  }

  return false;
}


int True_NB_bc95::check_modem_signal() {
  String re_str;
  sprintf_P(&(_modem_buf[0]), PSTR("AT+CSQ"));
  re_str = exec_modem_command(&(_modem_buf[0]), "+CSQ:", "\r\n", 1000, 1000);
  if ( re_str != "ERROR" ) {
    char ssi_str[3] = "\0\0\0";

    ssi_str[0] = re_str[0];
    if ( re_str[1] != ',') {
      ssi_str[1] = re_str[1];
    }

    return -1 * (113 - atoi(ssi_str) * 2);;
  }

  return -200;
}


bool True_NB_bc95::create_UDP_socket(int port, char sock_num[]) {
  sprintf(&(_modem_buf[0]), "AT+NSOCR=DGRAM,17,%d,1", port);
  return ( exec_modem_command(&(_modem_buf[0]), sock_num, "\r\n", 2000, 2000) != "ERROR" );
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


void True_NB_bc95::flush_modem_serial(unsigned long char_timeout_ms) {
  unsigned long end_t;

  end_t = millis() + char_timeout_ms;
  do {
    if ( MODEM_SERIAL->available() ) {
      (void)MODEM_SERIAL->read();
      end_t = millis() + char_timeout_ms;
    }
  } while ( millis() < end_t );
}


bool True_NB_bc95::sendUDPstr(String ip, String port, String data) {
  int str_len = data.length();
  char buffer[str_len+2];
  data.toCharArray(buffer, str_len+1);

  flush_modem_serial(2);

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

  if ( expect_rx_str("OK", "\r\n", 1000) != "ERROR" ) {
    return true;
  }
  else
  {
    return false;
  }
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

  Serial.println(F("------ Send to Cloud.IoTtweet -------"));
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
  Serial.println(F("--------------------------------------"));

  sendUDPstr(IoTtweetNBIoT_HOST, IoTtweetNBIoT_PORT, _packet);
  return "OK";
}


String True_NB_bc95::check_udp_incoming_str(){
  String retNSOMI;
  int indexNSONMI;
  String recvBuf;
  int delim_pos[10];
  String msg[10];
  String msgdata = "";

  if(MODEM_SERIAL->available()){

    retNSOMI = MODEM_SERIAL->readString();
    //Serial.println(retNSOMI);

    /* Check +NSOMI index */
    indexNSONMI = retNSOMI.indexOf("+NSONMI:");

    if(indexNSONMI>0){
      /* Send NSORF to require incoming message size : 100 byte */
      MODEM_SERIAL->println(F("AT+NSORF=0,100"));
      delay(300);

        if(MODEM_SERIAL->available()){
          recvBuf = MODEM_SERIAL->readString();
          //Serial.println("recvBuf is " + recvBuf);

          /* Parse buffer to message */
          for(int chkDelim=0; chkDelim<=5; chkDelim++){

            if(chkDelim == 0){
              delim_pos[chkDelim] = recvBuf.indexOf(F(","));
              msg[chkDelim] = recvBuf.substring(0,delim_pos[chkDelim]);
            }else{
              delim_pos[chkDelim] = recvBuf.indexOf(F(","),(delim_pos[chkDelim-1]+1));
              msg[chkDelim] = recvBuf.substring(delim_pos[chkDelim-1]+1,delim_pos[chkDelim]);
            }
            //Serial.println("delim_pos[" + String(chkDelim) + "] : " + delim_pos[chkDelim]);
            //Serial.println("msg[" + String(chkDelim) + "]: " + msg[chkDelim]);
          }

           //Serial.println(msg[4]);
           //Serial.println("len : " + String(msg[4].length()));
           msgdata = hex2string(msg[4]);
           //Serial.println("msg data : " + msgdata);
        }
      }

  }

  return msgdata;
}


String True_NB_bc95::hex2string(String hexData){
  String converted;
  char fetchC;
  _hexData = hexData;

  for(int hexCnt=0; hexCnt<_hexData.length(); hexCnt += 2){
      fetchC =  byteConverting(_hexData[hexCnt])<<4 | byteConverting(_hexData[hexCnt+1]);
      converted += fetchC;
  }

  return converted;
}


char True_NB_bc95::byteConverting(char cHex){
  char _byte;
  char _cHex = cHex;

  if((_cHex >= '0') && (_cHex <= '9')){
    _byte = _cHex - 0x30;
  }else{
    //..
  }

  if((_cHex >= 'A') && (_cHex <= 'F')){
    _byte = _cHex - 55;
  }else{
    //..
  }

  return _byte;
}
