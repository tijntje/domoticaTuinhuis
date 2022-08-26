#include <Arduino.h>
#include "WiFi.h"
#include "AsyncUDP.h"

#include <Adafruit_GFX.h>
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

SSD1306 display(0x3C, 16, 17);

#define ANALOG_PIN_0 36
int analog_value = 0;

int readLightSensor(){
  analog_value = analogRead(ANALOG_PIN_0);
  return analog_value                                                                                                ; //Read analog
}

unsigned int keyval=0;
String adcString;

const char* ssid = "Bommel";
const char* pass = "HetWachtwoordIsFizbo";
const int rele = 23;
AsyncUDP udp;

// set pin numbers
 const int I1=18;  
 const int I2=39;  
 const int I3=34;
 const int I4=35;
 const int I5=19;
 const int I6=21;   
 const int I7=22;
 const int I8=23;  
 const int O1=14;
 const int O2=13;
 const int O3=12;
 const int O4=15;  
 const int O5=2;
 const int O6=33;
 const int TO1=26;
 const int TO2=27;  

void setup() {
  Serial.begin(115200);     
  pinMode(18, INPUT); // initialize the inputs and outputs
  pinMode(39, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(19, INPUT);
  pinMode(21, INPUT);
  pinMode(22, INPUT);
  pinMode(23, INPUT);
 
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
  display.drawString(0, 0, "Hello Data MS");
  display.display();
  delay(300);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if (udp.listen(1234)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {
      Serial.print("UDP Packet Type: ");
      Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
      Serial.print(", From: ");
      Serial.print(packet.remoteIP());
      Serial.print(":");
      Serial.print(packet.remotePort());
      Serial.print(", To: ");
      Serial.print(packet.localIP());
      Serial.print(":");
      Serial.print(packet.localPort());
      Serial.print(", Length: ");
      Serial.print(packet.length()); //dlzka packetu
      Serial.print(", Data: ");
      Serial.write(packet.data(), packet.length());
      Serial.println();
      String myString = (const char*)packet.data();
      if (myString == "ZAP") {
        Serial.println("Zapinam rele");
        digitalWrite(rele, LOW);
      } else if (myString == "VYP") {
        Serial.println("Vypinam rele");
        digitalWrite(rele, HIGH);
      }
      packet.printf("Got %u bytes of data", packet.length());
    });
  }

}

void loop() {
  delay(1000);
  udp.broadcast("Anyone here?");
}