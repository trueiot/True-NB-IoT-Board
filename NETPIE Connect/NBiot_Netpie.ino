#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)      // Arduino UNO
    #include <AltSoftSerial.h>
    AltSoftSerial bc95serial;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)   // Arduino MEGA2560
    #define bc95serial Serial1
#endif

#include <Arduino.h>
#include "DHT.h"
#include "BC95Udp.h"
#include "MicrogearNB.h"
// กำหนดข้อมูลเชื่อมต่อ NETPIE
#define APPID "APP ID" // ต้องแก้ไข
#define KEY "APP KEY" // ต้องแก้ไข
#define SECRET "SECRET KEY" // ต้องแก้ไข

// กำหนดข้อมูลเชื่อมต่อ DHT22
#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

BC95UDP client;
Microgear mg(&client);

#define MAXBUFFSIZE 32
char payload[MAXBUFFSIZE];
long lastDHTRead = 0;

void setup() {
  bc95serial.begin(9600);
  BC95.begin(bc95serial);
  BC95.reset();

  Serial.begin(9600);
  Serial.println(F("Microgear Arduino NB-IoT Start!"));
  Serial.print(F("IMEI: "));
  Serial.println(BC95.getIMEI());
  Serial.print(F("IMSI: "));
  Serial.println(BC95.getIMSI());

  Serial.print(F("Attach Network..."));
  while (!BC95.attachNetwork()) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println(F("\nNB-IOT attached!"));
  Serial.print(F("RSSI: "));
  // ค่าความแรงสัญญาณ NB-IoT
  Serial.println(BC95.getSignalStrength());
  Serial.print(F("IPAddress: "));
  Serial.println(BC95.getIPAddress());

  mg.init(APPID, KEY, SECRET);
  mg.begin(5555);
}

void loop() {
  // เซนเซอร์​ DHT อ่านถี่เกินไปไม่ได้ จะให้ค่า error
  // จึงต้องเช็คเวลาครั้งสุดท้ายที่อ่านค่าว่าทิ้งช่วงนานพอหรือยัง ในที่นี้ตั้งไว้ 1 วินาที
  if(millis() - lastDHTRead > 1000){
    float humid = dht.readHumidity(); // อ่านค่าความชื้น
    float temp = dht.readTemperature(); // อ่านค่าอุณหภูมิ
    lastDHTRead = millis();

    Serial.print("Humid: "); Serial.print(humid); Serial.print(" %, ");
    Serial.print("Temp: "); Serial.print(temp); Serial.println(" C ");

    // ตรวจสอบค่า humid และ temp เป็นตัวเลขหรือไม่
    if (isnan(humid) || isnan(temp)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      // เตรียมสตริงในรูปแบบ "humid,temp"
      String datastring = (String)humid + "," + (String)temp;

      Serial.print(F("Sending --> "));
      Serial.println(datastring);
      datastring.toCharArray(payload, MAXBUFFSIZE-1);
      mg.publish("/nbiot/sensor", payload);
    }

    Serial.print(F("Sent Signal Strength: "));
    Serial.println(BC95.getSignalStrength());
    mg.publish("/nbiot/rssi", BC95.getSignalStrength());
}

    mg.loop();
}
