#include "AutoConnect.h"
#include "Irremote.h"

Irremote irremote;
AutoConnect autoconnect;

void setup() {
  // put your setup code here, to run once:
  autoconnect.setupAutoConnect();
  irremote.setupIrremote();
}

void loop() {
  
  irremote.loopIrremote(autoconnect.client);
  
  // put your main code here, to run repeatedly:
  autoconnect.loopAutoConnect();
  
}
