#include "AutoConetWifiMqtt.h"
#include "IrRemote.h"

AutoConetWifiMqtt autoconect;
IrRemote irremote;

void setup() {
  // put your setup code here, to run once:
  autoconect.BegingAutoConnet();
  irremote.BeginingIrSensor();
}

void loop() {
  // put your main code here, to run repeatedly:
  autoconect.Reconecting();
  irremote.mqttMessageRecibedIrSend();

}
