#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <PubSubClient.h>

#define mqtt_server       "xxx.cloudmqtt.com"
#define mqtt_port         "12345"

#define humidity_topic    "hola/javier"

WiFiClient espClient;
PubSubClient client(espClient);

WiFiManager wifiManager;

bool shouldSaveConfig = false;


WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
      



class AutoConetWifiMqtt{
    
    void saveConfigCallback () {
      Serial.println("Should save config");
      shouldSaveConfig = true;
    }

public:

    void BegingAutoConnet() {
      
      Serial.begin(115200);
      Serial.println();

      Serial.println("mounting FS...");

      if (SPIFFS.begin()) {
        Serial.println("mounted file system");
        if (SPIFFS.exists("/config.json")) {
          //file exists, reading and loading
          Serial.println("reading config file");
          File configFile = SPIFFS.open("/config.json", "r");
          if (configFile) {
            Serial.println("opened config file");
            size_t size = configFile.size();
            // Allocate a buffer to store contents of the file.
            std::unique_ptr<char[]> buf(new char[size]);

            configFile.readBytes(buf.get(), size);
            DynamicJsonDocument jsonBuffer(1024);
            deserializeJson(jsonBuffer, buf.get());
            serializeJson(jsonBuffer, Serial);
            if (serializeJson(jsonBuffer, Serial)) {
              Serial.println("\nparsed json");

              strcpy(mqtt_server, jsonBuffer["mqtt_server"]);
              strcpy(mqtt_port, jsonBuffer["mqtt_port"]);


            } else {
              Serial.println("failed to load json config");
            }
          }
        }
      } else {
        Serial.println("failed to mount FS");
      }
      //end read

      wifiManager.addParameter(&custom_mqtt_server);
      wifiManager.addParameter(&custom_mqtt_port);

      if (!wifiManager.autoConnect("AutoConnectAP")) {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        //reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(5000);
      }
      wifiManager.resetSettings();
      

      strcpy(mqtt_server, custom_mqtt_server.getValue());
      strcpy(mqtt_port, custom_mqtt_port.getValue());

      //save the custom parameters to FS
      if (shouldSaveConfig) {
        Serial.println("saving config");
        DynamicJsonDocument jsonBuffer(1024);

        jsonBuffer["mqtt_server"] = mqtt_server;
        jsonBuffer["mqtt_port"] = mqtt_port;


        File configFile = SPIFFS.open("/config.json", "w");
        if (!configFile) {
          Serial.println("failed to open config file for writing");
        }

        serializeJson(jsonBuffer, Serial);
        serializeJson(jsonBuffer, configFile);
        configFile.close();
        //end save
      }

      Serial.println("local ip");
      Serial.println(WiFi.localIP());
      //client.setServer(mqtt_server, 12025);
      const uint16_t mqtt_port_x = 1883;
      client.setServer(mqtt_server, mqtt_port_x);
    }



      
    void reconnect() {
      // Loop until we're reconnected
      while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        // If you do not want to use a username and password, change next line to
        // if (client.connect("ESP8266Client")) {
        if (client.connect("ESP8266Client")) {
          Serial.println("connected");
        } else {
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 5 seconds");
          // Wait 5 seconds before retrying
          delay(5000);
        }
      }
    }


    void Reconecting() {
      // put your main code here, to run repeatedly:
      if (!client.connected()) {
        reconnect();
      }
      client.loop();
      client.publish(humidity_topic, "que vuelti");
      Serial.println("que vuelti");
      delay(5000);
      client.subscribe("hola/javier");
    }
};
