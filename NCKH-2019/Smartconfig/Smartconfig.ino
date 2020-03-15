#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;

void setup() {
 int cnt = 0;
 Serial.begin(115200);
 WiFi.mode(WIFI_STA);
 while(WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 if(cnt++ >= 10){
 WiFi.beginSmartConfig();
 while(1){
 delay(1000);
 if(WiFi.smartConfigDone()){
 Serial.println("SmartConfig Success");
 break;
 }
 }
 }
 
 Serial.println("");
 Serial.println("");
 
 WiFi.printDiag(Serial);
 
 Udp.begin(49999);
 Serial.println("Server started");

 Serial.println(WiFi.localIP());
}
}
void loop() {
 Udp.parsePacket();
 while(Udp.available()){
 Serial.println(Udp.remoteIP());
 Udp.flush();
 delay(5);
 }
}