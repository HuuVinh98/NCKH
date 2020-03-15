#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
WiFiClient client;
WiFiClientSecure sclient;
LiquidCrystal_I2C lcd(0x3F,16,20);
String weatherMain = "";
String weatherDescription="";
String weatherLocation="";
String country;
int humidity;
int pressure;
int temp;
int tempMin,tempMax;
int clouds;
int windSpeed;
String weatherString;
char tempT[5],tempMinT[5],tempMaxT[5];
char humi[5],presS[5],WindSpeed[5];
const char* ssid ="HUU VINH";
const char* pass ="N16DCVT084";
String weatherKey ="89a619a4a6382776c76327928ed9a78e";
String weatherLang="&lang=en";
const char *weatherHost="api.openweathermap.org";
byte up[8]= {   
                  0b00100,
                  0b01110,
                  0b10101,
                  0b00100,
                  0b00100,
                  0b00100,
                  0b00100,
                  0b00000

};
byte down[8] = {
                  0b00000,
                  0b00100,
                  0b00100,
                  0b00100,
                  0b00100,
                  0b10101,
                  0b01110,
                  0b00100
};
void setup ()
{
  Wire.begin(2,0);
  Serial.begin(115200);
  lcd.begin();
  lcd.display();
  lcd.backlight();
 
  Serial.print("Connecting to ");
  Serial.print(ssid);
  lcd.setCursor(0,0);
  lcd.print("Connecting to ");
  lcd.setCursor(0,1);
  lcd.print(ssid);
  WiFi.begin(ssid,pass);
  while(WiFi.status()!= WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  lcd.clear();
}
  void printDigits2(int digits)
  {
    if (digits<1000)
    {
      lcd.print("0");
      lcd.print(digits);
      
    }
    else
    {
      lcd.print(digits);
    }
  }
void pressHumiClound()
{
  lcd.setCursor(2,1);
  lcd.print("Pressure:");
  lcd.setCursor(11,1);
  printDigits2(pressure);
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("                             ");
  lcd.setCursor(2,1);
  lcd.print("Humidity:");
  lcd.setCursor(11,1);
  printDigits2(humidity);
  lcd.print("%");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("                       ");
  lcd.setCursor(1,1);
  lcd.print("WinSpeed:");
  lcd.setCursor(11,1);
  printDigits2(windSpeed);
  lcd.print("m/s");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("                       ");
  lcd.setCursor(2,1);
  lcd.print("Clounds:");
  lcd.setCursor(9,1);
  printDigits2(clouds);
  lcd.print("%");
  delay(1000);
}
void getWeatherData()
{
  Serial.print("Connecting to ");
  Serial.println(weatherHost);
  delay(4000);
  lcd.clear();
  if(client.connect(weatherHost,80))
  {
    client.println(String("GET /data/2.5/weather?q=Thanh pho Ho Chi Minh,VN&")+"&units=metric&appid=" + weatherKey + weatherLang + "\r\n" + "Host: " + weatherHost + "\r\nUser-Agent: ArduinoWiFi/1.1\r\n" + "Connection: close\r\n\r\n");}
    else
    {
      Serial.println("connection failed");
      return;
    }
    String line;
    int repeatCounter = 0;
    while (!client.available() && repeatCounter <10)
    {
      delay(500);
      Serial.println("w.");
      repeatCounter++;
    }
    while(client.connected() && client.available())
    {
      char c = client.read();
      if (c == '[' or c == ']') 
      {c = ' ';
      line +=c;}
    }
    client.stop();
    
    DynamicJsonBuffer jsonBuf;
    JsonObject &root = jsonBuf.parseObject(line);
    if(!root.success());
    {
      Serial.println("parseObject() failed");
      return;
    }
    weatherDescription = root["weather"]["decription"].as<String>();
    weatherDescription.toLowerCase();
    temp = root["main"]["temp"];
    humidity = root["main"]["humidity"];
    pressure = root["main"]["pressure"];
    tempMin = root["main"]["temp_min"];
    tempMax = root["main"]["temp_max"];
    windSpeed = root["wind"]["speed"];
    clouds = root["clouds"]["all"];
    Serial.println(String("temp=")+temp);
    Serial.println(String("temp_min=")+tempMin);
    Serial.println(String("temp_max=")+tempMax);
    Serial.println(String("pressure")+pressure);
    Serial.println(String("humidity=")+humidity);
    Serial.println(String("wind_speed=")+windSpeed);
    Serial.println(String("clouds=")+clouds);
    lcd.setCursor(0,0);
    lcd.print("TEMP:");
    lcd.setCursor(5,0);
    lcd.print(temp);
    lcd.setCursor(7,0);
    lcd.print(",");
    lcd.setCursor(8,0);
    lcd.print(tempMin);
    lcd.setCursor(10,0);
    lcd.write(2);
    lcd.setCursor(11,0);
    lcd.print(",");
    lcd.setCursor(12,0);
    lcd.print(tempMax);
    lcd.setCursor(14,0);
    lcd.write(1);
    pressHumiClound();
}
void loop()
{
  getWeatherData();
  delay(5000);
}
      
    
    


