// library import
#include "FirebaseESP8266.h"
#include <WiFiManager.h>        // For WiFi config AP
#include <ESP8266WiFi.h>        // For check MacAddress
#include <WebSocketsServer.h>   // For WebSocketComm
#include <SPI.h>                // For OLED
#include <Wire.h>               // For OLED
#include <Adafruit_SSD1306.h>   // For OLED
#include <Adafruit_NeoPixel.h>  // For Neopixel
#include "credential.h"
#include "definitions.h"
#include "externalFunc.h"


void setup()
{

  Serial.begin(9600);

  pinMode(LEDSPIN, OUTPUT);
  
  wifiManager.autoConnect(ssid_ap, password_ap); // wifi connect
  // waiting wifi connect
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  FirebaseSetup();
  DisplaySetup();
  GetExternalIP();
  
  pixels.begin();
  pixels.show();
}

void loop()
{
  if (Firebase.getString(ledData, "/Controller/" + macID + "/ledData"))
  {
    Serial.println(ledData.stringData());
    if (ledData.stringData())
    {
      mg = ledData.stringData().substring(1, 4).toInt();
      mr = ledData.stringData().substring(4, 7).toInt();
      mb = ledData.stringData().substring(7, 10).toInt();
      uint8_t brightness = ledData.stringData().substring(10, 13).toInt();
      Serial.println(ledData.stringData());
      Serial.println(mr);
      Serial.println(mg);
      Serial.println(mb);
      Serial.println(brightness);
      for (int i = 0; i < NUMPIXELS; i++)
      {
        pixels.setPixelColor(i, pixels.Color(mr, mg, mb));
      }
      pixels.setBrightness(brightness);
      pixels.show();
      delay(1000);
    }
  }
}