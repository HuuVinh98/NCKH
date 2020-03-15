  #include <ESP8266WiFi.h>
  #include <WiFiUdp.h>
  #include <NTPClient.h>
  #include <time.h>
  #include <ArduinoJson.h>
  WiFiUDP u;
  NTPClient n(u,"3.asia.pool.ntp.org",7*3600);
char WFname[] = "openlab1";
char WFpass[] = "phonglab1";
String apikey = "IsLlkzdfgwCrRHhHAtSYjsKwIiQ0XhD7";
String citycode = "6-353981_1_AL";
char sever[] = "dataservice.accuweather.com";
unsigned long timex;
char txt[3000];
String line;
int i;
WiFiClient client;
void getWeather()
{
  Serial.println("\nStarting connection to server... " + String(server));
  if (client.connect(server, 80))
  {
    Serial.println("Connected to server\n");
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
    Serial.print("Temp_max: " + String((temp_max1-32)/1.8) + "°C");
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




void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WFname,WFpass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print(WFname);
  Serial.println(" is connected!");
  n.begin();
  configTime(7*3600,0,"pool.ntp.org","time.nist.gov");

}

void loop() 
{
  
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  Serial.print(p_tm->tm_mday);
  Serial.print("/");
  Serial.print(p_tm->tm_mon+1);
  Serial.print("/");
  Serial.print(p_tm->tm_year+1900);
  Serial.print("  ");
  n.update();
  Serial.println(n.getFormattedTime());

  if (millis() - timex > 1000)
    getWeather();
  timex = millis();
}
