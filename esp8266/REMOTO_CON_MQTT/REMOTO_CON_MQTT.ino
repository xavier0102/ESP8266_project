#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// Example of data captured by IRrecvDumpV2.ino
uint16_t rawData[181] = {2562, 428,  1378, 430,  768, 428,  1374, 428,  774, 426,  1376, 428,  774, 428,  772, 428,  1376, 428,  774, 428,  772, 428,  774, 428,  772, 25806,  2562, 428,  1374, 428,  774, 428,  1374, 428,  774, 428,  1374, 428,  774, 428,  774, 428,  1376, 428,  772, 430,  772, 430,  772, 430,  772, 25804,  2562, 428,  1374, 428,  774, 428,  1372, 432,  772, 428,  1374, 428,  774, 428,  770, 430,  1374, 428,  774, 428,  774, 430,  772, 428,  772, 25804,  2562, 428,  1374, 428,  772, 428,  1372, 432,  774, 428,  1374, 428,  774, 428,  772, 428,  1376, 428,  772, 430,  772, 428,  774, 428,  774, 25800,  2562, 430,  1374, 428,  772, 430,  1372, 430,  772, 428,  1376, 428,  772, 430,  774, 428,  1374, 428,  774, 428,  774, 428,  774, 428,  774, 25804,  2562, 428,  1374, 428,  772, 430,  1374, 430,  772, 428,  1374, 430,  774, 428,  770, 430,  1374, 428,  774, 428,  772, 428,  774, 428,  774, 25802,  2562, 428,  1374, 428,  772, 428,  1374, 430,  772, 430,  1374, 428,  774, 428,  772, 432,  1374, 428,  774, 428,  772, 430,  774, 428,  774};  // UNKNOWN CBAED686


const char* ssid = "MIWIFI_2G_XdeY";
const char* password =  "XgECfcmQ";

const char* mqttServer = "192.168.1.165";
const int mqttPort = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  // put your setup code here, to run once:

  basicSetup();
  digitalWrite(LED_BUILTIN, HIGH);

  connectWifi();  
  connectMqttServer();
  mqttClient.subscribe("hola/javier");

    irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266

}

void loop() {
  // put your main code here, to run repeatedly:
    if (!mqttClient.connected()) {
    connectMqttServer();
    }
    mqttClient.loop();
  }


void connectMqttServer() {
  
  mqttClient.setServer(mqttServer, mqttPort); // specify the address and the port of the MQTT server
  mqttClient.setCallback(onMqttRecived); //specify a handling function that is executed when a MQTT message is received
  
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("ESP8266Client")) {
      Serial.println("connected to mqtt server");
      mqttClient.subscribe("hola/javier");
    } else {
      Serial.print("failed connecting mqtt, state=");
      Serial.print(mqttClient.state());
      Serial.println("..try again in 5 seconds");
      delay(5000);
    }
  }
}

void connectWifi(){
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }  
  Serial.println("Connected to the WiFi network");
}

void basicSetup(){
  Serial.begin(115200);   //iniciar puerto serie
  pinMode(kIrLed , OUTPUT);  //definir pin como SALIDA
  pinMode(LED_BUILTIN, OUTPUT);
}

void onMqttRecived(char* topic, byte* payload, unsigned int length) {
  printMessageSerialPort(topic, payload, length);
}

void printMessageSerialPort(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  char PAYLOAD[10]="";
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    PAYLOAD[i]=(char)payload[i];
  }
  Serial.print(PAYLOAD);
    if(String(PAYLOAD[0])=="1"){
      
      Serial.println("a rawData capture from IRrecvDumpV2");
      irsend.sendRaw(rawData, 181, 38);  // Send a raw data capture at 38kHz.
      
    }
  
  Serial.println();
  Serial.println("-----------------------");
}
