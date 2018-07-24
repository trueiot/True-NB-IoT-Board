# True NB-IoT Library
True NB-IoT Library is designed for `True NB-IoT Arduino Sheild` to connect with your own IoT Platform/ Server. 
The library is using Software Serial for communication with Arduino board.

### Auduino Uno Compatible 
เหมาะสำหรับการใช้งานกับ **Arduino Uno**  
หากต้องการใช้งานกับ Arduino Mega 2560 ให้เข้าไปดูวิธีการได้ใน  **`Arduino Mega setup` **

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

### [วิธีใช้งานกับ Dashboard IoTtweet]
<img src="https://github.com/Isaranu/IoTtweetNBIoT/blob/master/logo.png" width="100">
<img src="https://github.com/Isaranu/True-NB-IoT-Board/blob/master/true_nb_iot_iottweet.png" width="500">

- **สมัครใช้งาน IoTtweet** : เข้าไปที่ www.iottweet.com ทำการ Signup และ verify activate e-mail ให้เรียบร้อย. ไปที่ **"My IoT garage"** ทำการเพิ่ม device เพื่อรับ key ที่ใช้สำหรับใส่ลงใน code.

- **ออกแบบ Dashboard** : ไปที่ **"Dashboard"**, และทำการ Edit layout. เพิ่ม Widget ต่างๆที่ต้องการ พร้อมทั้งตั้งค่า (setting) widget แต่ละ element ให้เรียบร้อย. เมื่อเสร็จเรียบร้อย ให้กด **"Save layout"**.

- **เชื่อมต่อบอร์ด** : ประกอบบอร์ด True NB-IoT เข้ากับ Arduino UNO หรือ Arduino MEGA. (ประกอบให้ Pin ตรงกัน ตาม spec. ของบอร์ด) และทำการเชือมต่อสาย USB.

- **Upload code** : สามารถใช้ code ตัวอย่างจากใน Examples ได้ เช่น<br>
**simple_send_IoTtweet.ino** > ส่งค่า random number จำนวน 4 ค่า, ไปที่ Dashboard ในช่องข้อมูล slot0 ถึง slot3.<br>
> การเชื่อมต่อสัญญาณ NB-IoT ได้หรือไม่นั้น ขึ้นอยู่กับการเปิดสัญญาณในแต่ละพื้นที่.

### [Code]

คำอธิบายคำสั่ง code ต่างๆสำหรับส่งค่าไป **Dashboard IoTtweet**.<br>

- กรอกเลข ID ของ IoTtweet account และ key ของอุปกรณ์ที่ register ใน **My IoT garage**.
```CPP
String userid = "your-IoTtweet-account-ID";       /*IoTtweet account user ID (6 digits, included zero pre-fix)*/
String key = "your-device-key";                   /*IoTtweet registered device key in "MY IOT Garage"*/
```

- Variable ของข้อมูลที่ต้องการส่ง. สามารถแทนเข้าไปในตัวแปร data0, data1, data2, data3 ได้เลย.
```CPP
float data0, data1, data2, data3;                 /*Your sending data variable.*/
String private_tweet = "TRUE NB-IoT";             /*Your private tweet meassage to dashboard*/
String public_tweet = "Hello IoTtweet";           /*Your public tweet message to dashboard*/
```

- คำสั่งส่งข้อมูลไปยัง IoTtweet Dashboard
```CPP
modem.WriteDashboardIoTtweet(userid, key, data0, data1, data2, data3, private_tweet, public_tweet);
```

### NB-IoT Shield for Arduino (Narrow Band Internet of Things Shield for Arduino) 
เป็นบอร์ด สื่อสารสำาหรับเครือข่าย LTE-Cat.NB1 โดยใช้โมดูลรุ่น BC95-B8 จาก Quectel ซึ่งรองรับการใช้งานในย่าน ความถี่ 900 MHz พร้อมความสะดวกในการใช้งานผา่น USB  NB-IoT Shield เหมาะกับการนำไปพัฒนาแอพพลิเคชัน IoT ที่ต้องการประหยัดพลังงาน เช่น ระบบที่จอดรถอัจฉริยะ (Smart Parking System) 

### กรณีไฟไม่ติด อาจเกิดจาก Board เสียหาย  ขอความกรุณาคุณลูกค้านำ Board ไปตรวจสอบอาการเบื้องต้นที่ True Shop ได้ที่สาขาดังต่อไปนี้  `Central World , Central Bang Na ,Central Rama 2, Central Pinklao, Future Park Rangsit, Promenade` และหากพบว่า Bord เสียจริง ทางเราจะดำเนินการเปลี่ยน Bord ให้ใหม่ ตามสาขาดังกล่าว

แต่หากคุณลูกค้าไม่สะดวกไปตามสาขาที่แจ้ง สามารถแจ้งเรื่อง  Board เสียไว้ได้ที่ True Shop ทุกสาขา  และในส่วนสาขาที่ไม่ได้ระบุไว้ข้างตน ทางสาขาจะไม่สามารถเปลี่ยน หรือ ตรวจสอบได้ในทันที  จึงต้องรอทำการจัดส่งไปสาขาที่สามารถเปลี่ยนและตรวจสอบได้ จึงอาจใช้ระยะเวลาในการดำเนินการเพิ่มขึ้น จึงแจ้งคุณลูกค้าไว้ให้ทราบล่วงหน้า

   กรณีไม่ปรากฎไฟขึ้นที่ board ท่านสามารถแจ้งความประสงค์ขอเปลี่ยน board เครื่องใหม่ได้ที่ True Shop สาขา `Central World, Central Bangna, Central Rama2, Central Pinklao, Future Park Rangsit และ The Promenade` หากท่านไม่สะดวกเดินทางไปยัง True Shop สาขาข้างต้น ท่านสามารถแจ้งความประสงค์เพื่อส่ง board กลับมาเปลี่ยนได้ที่ True Shop สาขาอื่นๆ ทั่วประเทศ ทางบริษัทจะทำการจัดส่ง board เครื่องใหม่ให้กับท่าน

   กรณี Board Run โปรแกรมตัวอย่างแล้ว  และขึ้นว่า  Register network failed .  แสดงว่าอาจเกิดปัญหาจาก Network   รบกวนคุณลูกค้า ติอต่อ 1242 Call Center เพื่อตรวจสอบปัญหาด้านเครือข่าย เพื่อแก้ไข และจะทำการแจ้งผลกลับให้ทางคุณลูกค้าทราบ
กรณีปรากฎข้อความ “Register network failed” ขึ้นหลังการทดสอบ ท่านสามารถสอบถามเพิ่มเติมได้ที่ Call Center 1242 เพื่อทำการตรวจสอบด้านเครือข่าย โดยทางบริษัทจะทำการแจ้งกลับผลการตรวจสอบให้ท่านในลำดับต่อไป
