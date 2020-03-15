#include <JsonListener.h>
#include <ESP8266WiFi.h>
#include "Wire.h"
#include <WundergroundClient.h>
#include "forts.h"
#include <Ticker>
#include "TimeClient.h>
SSD1306 display(0x3c,D6,D5);
WundergroundClient wunderground(true);
float uttoffset = 5.30;
TimeClient timeClient(uttoffset);

String apikey ="7a543549a70dbd8324141083daf5de74";
String country = "Vietnam";
String city ="Ho Chi Minh";
void (*franeCallbacks[])(int x, int y) = (drawFrame1,drawFrame2,drawFrame3,drawFrame4);
int numeroffames = 4;
bool raeadyForWeatherUpdate = false;
String lastUpdate = "__";
Ticker ticker;
void setup()
{
  Serial.begin(1152000);
  Serial.println();
  Serial.println();
  display.unit();
  display.clear();
  display.display();
  WiFi.begin("HUU VINH","N16DCVT084");
  int counter=0;
  while (WiFi.status()!= WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    counter++;
  }
  display.setFrameCallbacks(numberofframes,framesCallbacks);
  display.setFrameTransitionTicks(10);
  display.setFrameWaitTicks(150);
  
  
  }



