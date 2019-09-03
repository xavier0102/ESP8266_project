codigo para reset de wifi configuration
https://github.com/tzapu/WiFiManager/issues/142

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
void setup()
{
  WiFi.disconnect();
//  WiFiManager.resetSettings();
//  ESP.eraseConfig();
//  ESP.reset();
//  ESP.restart();
}
void loop()
{
  yield();
}