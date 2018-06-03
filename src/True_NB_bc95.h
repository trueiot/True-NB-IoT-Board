#include <Stream.h>
#include <Arduino.h>

#define COAP_HEADER_SIZE 4
#define COAP_OPTION_HEADER_SIZE 1
#define COAP_PAYLOAD_MARKER 0xFF
#define MAX_OPTION_NUM 10
#define BUF_MAX_SIZE 64
#define COAP_DEFAULT_PORT 5683


#define COAP_VER 1
#define COAP_TYPE_CON  0
#define COAP_TYPE_NONCON  1
#define COAP_TYPE_ACK  2
#define COAP_TYPE_RESET  3
#define COAP_GET  1
#define COAP_POST  2
#define COAP_PUT  3
#define COAP_DELETE  4

#define MODEM_RESP 128

/* Dashboard Partner parameter : IoTtweet.com */
#define IoTtweetNBIoT_HOST "35.185.177.33"    // - New Cloud IoTtweet server
#define IoTtweetNBIoT_PORT "5683"             // - Default udp port

class CoapPacketTrueIoT {
  public:
    uint8_t ver;
    uint8_t type;
    uint8_t tokenlen;
    uint8_t ver_type_tokenlen;
    uint8_t code[2];
    uint8_t messageid[4];
    uint8_t token[8];
    uint8_t option_1_name_path[8];
    uint8_t option_2_length[2];
    uint8_t option_2_ver[4];
    uint8_t option_3_dev_token_len[2];
    uint8_t option_3_dev_token_ext_len[2];
    uint8_t option_3_dev_token[20];
    uint8_t option_4_len_telemetry_word[2];
    uint8_t option_4_telemetry_word[18];
    uint8_t option_4_endmask[2];
    uint8_t buffer[BUF_MAX_SIZE];

};


class True_NB_bc95 {
  public:
    void init(Stream &serial);
    bool reboot();
    String getIMSI();
    String getIMEI();
    bool closeUDPSocket();
    bool check_match( char target[], char pattern[], int len_check);
    int check_match_index( char target[], char pattern[], int len_check );
    String expect_rx_str( unsigned long period, char exp_str[], int len_check);
    bool initModem();
    bool check_modem_status();
    bool register_network();
    String check_ipaddr();
    bool check_incoming_msg();
    int check_modem_signal();
    bool create_UDP_socket(int port, char sock_num[]);
    bool postRequest(char token[], char payload[], CoapPacketTrueIoT *packet);
    bool sendUDPPacket2(char socknum[], String remoteIP, int remotePort, CoapPacketTrueIoT *packet, int json_len);

    bool sendUDPstr(String ip, String port, String data);
    String WriteDashboardIoTtweet(String userid, String key, float slot0, float slot1, float slot2, float slot3, String tw, String twpb);

  private:
    Stream* MODEM_SERIAL;
    String _packet, _userid, _key, _tw, _twpb;
    float _slot0, _slot1, _slot2, _slot3;

};
