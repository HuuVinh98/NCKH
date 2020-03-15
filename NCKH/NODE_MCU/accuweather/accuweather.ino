
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <time.h>
 

int timezone= 7*3600;
int dst=0;
char ssid[] = "HUU VINH";         //  your network SSID (name)
char pass[] = "N16DCVT084";     //  your network PASSWORD ()
String  apikey = "dP3pHJABR6TuXGEM2m7FXg4pGjEYNcIB"; //open weather map api key lay sau 10h ngay 25/5/2018
String  citycode = "6-353981_1_AL";   //the city you want the weather for
char    server[] = "dataservice.accuweather.com"; //Sever API
unsigned long  timex1;
unsigned long  timex2;
char    txt[3000];
String  line;
int i;

WiFiClient client;

void getWeather()
{
  
  if (client.connect(server, 80))
  {
    
    client.print("GET http://dataservice.accuweather.com/forecasts/v1/daily/5day/" + citycode + "?apikey=" + apikey + " HTTP/1.0\r\n");
    client.println("Connection: close");
    client.println();
    delay(10);
  }
  else
  {
    Serial.println("unable to connect");
  }
  while (client.connected())
  {
    line = client.readStringUntil('\n');
    //Serial.println(line);
    filter();
  }
}

void filter()
{
  DynamicJsonBuffer jsonBuffer(line.length() + 1);
  line.toCharArray(txt, line.length() + 1);         // Chuyen tu String sang char de xu li du lieu
  JsonObject & root = jsonBuffer.parseObject(txt);
  float     temp_max1      = root["DailyForecasts"][0]["Temperature"]["Maximum"]["Value"];
  float     temp_min1      = root["DailyForecasts"][0]["Temperature"]["Minimum"]["Value"];
  String  wea1       = root["DailyForecasts"][0]["Day"]["IconPhrase"];
  String wea2         =root["DailyForecasts"][0]["Night"]["IconPhrase"];
  float     temp_max2      = root["DailyForecasts"][1]["Temperature"]["Maximum"]["Value"];
  float     temp_min2      = root["DailyForecasts"][1]["Temperature"]["Minimum"]["Value"];
  String  wea3       = root["DailyForecasts"][1]["Day"]["IconPhrase"];
  String wea4         =root["DailyForecasts"][1]["Night"]["IconPhrase"];
 
  
  Serial.print(txt); //Hiển thị gói tin đầu tiên vừa nhận được !!
  if (temp_max1 != 0 )
  { Serial.println("");
    Serial.println("Temp_max: " + String((temp_max1-32)/1.8) + "°C");
    Serial.println(" ");
    Serial.println("Temp_min: " + String((temp_min1-32)/1.8) + "°C");
    Serial.println("\tWeather Day Status: " + wea1);
    Serial.println("\tWeather Night Status: " + wea2);
    Serial.println("\n");
    Serial.println("");
    Serial.print("Temp_max_NEXT_DAY: " + String((temp_max2-32)/1.8) + "°C");
    Serial.println(" ");
    Serial.println("Temp_min_NEXT_DAY: " + String((temp_min2-32)/1.8) + "°C");
    Serial.println("\tWeather Day Status: " + wea3);
    Serial.println("\tWeather Night Status: " + wea4);
    Serial.println("\n");


  }
}
void time_and_day()
{
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  String t = ctime(&now);
  String thu   = t.substring(0, 3);
  thu.toUpperCase();
  Serial.print(thu);
  Serial.print(",");

  Serial.print(p_tm->tm_mday);
  Serial.print("/");
  Serial.print(p_tm->tm_mon+1);
  Serial.print("/");
  Serial.print(p_tm->tm_year+1900);
  Serial.print("  ");
  Serial.print(p_tm->tm_hour);
  Serial.print(":");
  Serial.print(p_tm->tm_min);
  Serial.print(":");
  Serial.print(p_tm->tm_sec);
  Serial.print("  ");

}

void setup()
{
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) //Kiem tra ket noi wifi
  {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected to: ");
  Serial.println(ssid);

  configTime(timezone,dst,"pool.ntp.org","time.nist.gov");
  
  
 
}
void loop()
{
  time_and_day();
  getWeather();
  timex1=millis();
  timex2=millis();
  if ( (unsigned long) (millis() - timex1) > 1000)
 { time_and_day();
  timex1=millis();
 }
  if ( (unsigned long) (millis() - timex2) > 21600000)
    {
      getWeather();
      timex2 = millis();
    }
   


}

