#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <DHT.h>

#include <FirebaseArduino.h>
#define FIREBASE_HOST "nckh-76d63.firebaseio.com" 
#define FIREBASE_AUTH "EPu1iSnAtJZLooX3bRFsafdpZuOwLf7I7h1daoup"
#define DHTPIN D4     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);
#define SensorPin D3      
#define Offset 0.00          
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40 
static float pHValue,voltage;  
float temp,NH3Value;
String status;

WiFiUDP Udp;
int pHArray[ArrayLenth];  
int pHArrayIndex=0;
const int Time = 3000; 

void setup() 
{
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
Serial.begin(115200);
        dht.begin();
        Serial.print("Connecting");
        while (WiFi.status() != WL_CONNECTED) {
                Serial.print(".");
                delay(100);
        }
        Serial.println("\r\nWiFi connected");
        Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
}
void loop() {
 Udp.parsePacket();
 while(Udp.available()){
 Serial.println(Udp.remoteIP());
 Udp.flush();
 delay(5);
 }

     
        delay(Time);
        nhietdo();
        pH();
        NH3Value = NH3(temp,pHValue);
        Status();
        Serial.printf("Nhiet do %s - pH %s - NH3 %s\r\n", String(temp,1).c_str(),String(pHValue,1).c_str() ,String(NH3Value,1).c_str());
        FireBase();
                
        
 
}
// Lay gia tri nhiet do, do am

void nhietdo()
{
        temp = dht.readTemperature();
        if (isnan(temp)) 
        {
                Serial.println("Failed to read from DHT sensor!");
                return;
        }
}

// Tinh gia tri pH
void pH()
{
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   
  {
    Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
    Serial.println(pHValue,2);
        printTime=millis();
  }
}

//Doi gia tri analog sang gia tri pH
double avergearray(int* arr, int number)
{
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    
          max=arr[i];
        }else{
          amount+=arr[i]; 
        }
      }
    }
    avg = (double)amount/(number-2);
  }
  return avg;
}

//Tinh nong do NH3
float NH3(float t, float pH)
{
  float a;
  float pKA = (2729.69/(t+273.16))+0.1105-0.000071*(t+273.16);
  return  a = 0.05*(100/(1+pow(10,(pKA-pH))));
}

//cap nhat trang thai Tom
void Status()
{
  if(pHValue >= 9.5 )
  {
    status = "NGUY HIEM pH CAO";
  }
  else if (pHValue <= 6.5 )
  {
    status = "NGUY HIEM pH THAP";
  }
  else if (pHValue <= 9 && pHValue >= 7)
  {
    status = "AN TOAN";
  }
  else if (pHValue <1 && pHValue >= 0.1)
  {
    status = "LOI CAM BIEN";
  }
  
}


// Dua len FireBase
void FireBase() 
{                                 
        Firebase.setFloat("/Temperature", temp);
        Firebase.setFloat("/pH", pHValue);
        Firebase.setFloat("/NH3",NH3Value);
        Firebase.setString("/Status", status); 
        if (Firebase.failed()) 
        {
         Serial.print("setting /number failed:");
         Serial.println(Firebase.error());
         return;
       }
}
