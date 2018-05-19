# True NB-IoT Library
True NB-IoT Library is designed for `True NB-IoT Arduino Sheild` to connect with your own IoT Platform/ Server. 
The library is using Software Serial for communication with Arduino board.

### Specifications:
	Communication Module: Quectel BC95-B8
	Network Technology: LTE Cat. NB1 (NB-IoT)
	Frequency: Band 8 (900MHz)
	Data transmission: (Single tone)
		○ Downlink 24 kbps
		○ Uplink 15.625 kbps
	Protocol stack
		○ UDP
		○ CoAP
	Serial Communication (UART)
		Hardware Serial 
		○ RX : PIN0
		○ TX : PIN1
		Software Serial
		○ RX : PIN8
		○ TX : PIN9
	สามารถเชื่อมตอ่กบั Raspberry Pi ผา่นทาง USB และสามารถเลอืกใช้ผา่นทาง UART ได้ 
	Additional Feature:
		o NB-IoT Module Reset Button
		o Power Status LED
		o UART from Computer or Raspberry Pi USB
	NB-IoT Module Certification
		○ NBTC, CE, CCC, 
	SIM-Card Slot for True NB-IoT

# Getting Started
  1. Connect `True NB-IoT` on Arduino Board
  2. Coding & Compile using Arduino Desktop IDE
		- Install the Arduino board drivers
		- Import True NB-IoT Library from Library Manager
		- Open  Example and specify your server IP & Port
		- Compile & Upload

## Function List:

### General Function:
    reboot()
      - Used for reboot modem
    String getIMSI()    
    String getIMEI()
    bool closeUDPSocket()
    bool check_match( char target[], char pattern[], int len_check)
    int check_match_index( char target[], char pattern[], int len_check )
    String expect_rx_str( unsigned long period, char exp_str[], int len_check)
    bool initModem()
    bool check_modem_status()
    bool register_network()
    String check_ipaddr()
    bool check_incoming_msg()
    int check_modem_signal()
    bool create_UDP_socket(int port, char sock_num[])
    bool postRequest(char token[], char payload[], CoapPacketTrueIoT *packet)
    bool sendUDPPacket2(char socknum[], String remoteIP, int remotePort, CoapPacketTrueIoT *packet, int json_len)	


### NB-IoT Shield for Arduino (Narrow Band Internet of Things Shield for Arduino) 
เป็นบอร์ด สื่อสารสำาหรับเครือข่าย LTE-Cat.NB1 โดยใช้โมดูลรุ่น BC95-B8 จาก Quectel ซึ่งรองรับการใช้งานในย่าน ความถี่ 900 MHz พร้อมความสะดวกในการใช้งานผา่น USB  NB-IoT Shield เหมาะกับการนำไปพัฒนาแอพพลิเคชัน IoT ที่ต้องการประหยัดพลังงาน เช่น ระบบที่จอดรถอัจฉริยะ (Smart Parking System) 



 
