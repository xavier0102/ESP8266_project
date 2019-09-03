#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <PubSubClient.h>

// Pines para los LEDs
#define REDLED 5
#define BLUELED 4
#define GREENLED 0

//Variables
int ledDelay = 1000;

const int sensorPin = 10;

float minBatteryCharge = 455;

const char* ssid = "Vedado_2G";
const char* password =  "C4nd3l0n3s";

const char* mqttServer = "192.168.1.103";
const int mqttPort = 1883;
//const char* mqttUser = "YourMqttUser";
//const char* mqttPassword = "YourMqttUserPassword";


ADC_MODE(ADC_VCC);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  Serial.begin(115200);
  setupInternalControlLed(); 

  connectWifi();  
  connectMqttServer();
  //publishHelloMessage();

  setupExternalControlLed();
}

void loop(){
  if (!mqttClient.connected()) {
    connectMqttServer();
  }
  int pinValue = digitalRead(sensorPin);  //lectura digital de pin
  if (pinValue == HIGH){
    mqttClient.publish("shirka/door/status","open");
    }
  else{
    mqttClient.publish("shirka/door/status","closed");
  }
  
  checkBatteryStatus();
  delay(2000);
}

void connectMqttServer() {
  
  mqttClient.setServer(mqttServer, mqttPort); // specify the address and the port of the MQTT server
  mqttClient.setCallback(onMqttRecived); //specify a handling function that is executed when a MQTT message is received
  
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("ESP8266Client")) {
      Serial.println("connected to mqtt server");
      publishHelloMessage();
    } else {
      Serial.print("failed connecting mqtt, state=");
      Serial.print(mqttClient.state());
      Serial.println("..try again in 5 seconds");
      delay(5000);
    }
  }
}


void onMqttRecived(char* topic, byte* payload, unsigned int length) {
  printMessageSerialPort(topic, payload, length);
}

void connectWifi(){
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  
//  WiFi.begin(ssid, password);
//  Serial.print("Connecting to WiFi..");
//  while (WiFi.status() != WL_CONNECTED) {
//      delay(500);
//      Serial.print(".");
//  }  
//  Serial.println("Connected to the WiFi network");
}

void printMessageSerialPort(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  } 
  Serial.println();
  Serial.println("-----------------------");
}

void publishHelloMessage(){
  mqttClient.publish("shirka/door/hello", "1");
}

void internalControlLed(bool light){  
  if (light){
    digitalWrite(LED_BUILTIN, LOW);  
  }
  else{
    digitalWrite(LED_BUILTIN, HIGH);
    }
  }

void setupInternalControlLed(){
  pinMode(sensorPin , INPUT);  //definir pin como entrada
  pinMode(LED_BUILTIN, OUTPUT);
  internalControlLed(false);
}

void checkBatteryStatus(){
  float vccVolt = ((float)ESP.getVcc())/1024;
  Serial.print(vccVolt);
  Serial.print("--");
  char* batteryStatus;
  
  if (vccVolt > minBatteryCharge){
    mqttClient.publish("shirka/battery/status","ok");
  }
  else{
    mqttClient.publish("shirka/battery/status","low");
    externalControlLedRed(true); 
  }
}

void setupExternalControlLed(){  
  pinMode(REDLED, OUTPUT);
  pinMode(BLUELED, OUTPUT);
  pinMode(GREENLED, OUTPUT);  
  externalControlLedRed(false);
  externalControlLedGreen(false);
  externalControlLedBlue(false);
  } 

void externalControlLedRed(bool light){  
  if (light){
    digitalWrite(REDLED, LOW);  
  }
  else{
    digitalWrite(REDLED, HIGH);
    }
  }

void externalControlLedBlue(bool light){  
  if (light){
    digitalWrite(BLUELED, LOW);  
  }
  else{
    digitalWrite(BLUELED, HIGH);
    }
}

void externalControlLedGreen(bool light){  
  if (light){
    digitalWrite(GREENLED, LOW);  
  }
  else{
    digitalWrite(GREENLED, HIGH);
    }
}
