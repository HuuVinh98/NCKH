#include <DHT.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "nckh-76d63.firebaseio.com" 
#define FIREBASE_AUTH "EPu1iSnAtJZLooX3bRFsafdpZuOwLf7I7h1daoup"
#define DHTPIN D4     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);
#define SensorPin D3      
#define Offset -2.5          
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40 
static float pHValue,voltage;  
float temp,humi,NH3Value;
String status;


const char* ssid = "NTT"; 
const char* password = "internetvanvat"; 
int pHArray[ArrayLenth];  
int pHArrayIndex=0;
const int Time = 5000; 

void setup() 
{
        Serial.begin(115200);
        dht.begin();
        Serial.print("Connecting");
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
                Serial.print(".");
                delay(100);
        }
        Serial.println("\r\nWiFi connected");
        Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
}

void loop() 
{
     
        delay(Time);
        nhietdo();
        pH();
        NH3Value = NH3(temp,pHValue);
        Status();
        Serial.printf("Nhiet do %s - pH %s - Do am %s - NH3 %s\r\n", String(temp,1).c_str(),String(pHValue,1).c_str(), String(humi,1).c_str(),String(NH3Value,1).c_str());
        FireBase();
                
        
}


// Lay gia tri nhiet do, do am

void nhietdo()
{
        temp = dht.readTemperature();
        humi = dht.readHumidity();
        if (isnan(temp) || isnan(humi)) 
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

//cap nhat trang thai
void Status()
{
  if(pHValue >= 12 || pHValue  <= 6.5)
  {
    status = "NGUY HIEM";
  }
  else if ((pHValue >= 8.6 && pHValue < 9.6)||(pHValue <7.5 && pHValue > 6.5))
  {
    status = "BAT THUONG";
  }
  else if (pHValue <8.5 && pHValue >= 7.5)
  {
    status = "AN TOAN";
  }
  
}

// Dua len FireBase
void FireBase() 
{
        Firebase.setFloat ("/Humidity", humi);                                  
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

