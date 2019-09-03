

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// Example of data captured by IRrecvDumpV2.ino
uint16_t rawData[181] = {2562, 428,  1378, 430,  768, 428,  1374, 428,  774, 426,  1376, 428,  774, 428,  772, 428,  1376, 428,  774, 428,  772, 428,  774, 428,  772, 25806,  2562, 428,  1374, 428,  774, 428,  1374, 428,  774, 428,  1374, 428,  774, 428,  774, 428,  1376, 428,  772, 430,  772, 430,  772, 430,  772, 25804,  2562, 428,  1374, 428,  774, 428,  1372, 432,  772, 428,  1374, 428,  774, 428,  770, 430,  1374, 428,  774, 428,  774, 430,  772, 428,  772, 25804,  2562, 428,  1374, 428,  772, 428,  1372, 432,  774, 428,  1374, 428,  774, 428,  772, 428,  1376, 428,  772, 430,  772, 428,  774, 428,  774, 25800,  2562, 430,  1374, 428,  772, 430,  1372, 430,  772, 428,  1376, 428,  772, 430,  774, 428,  1374, 428,  774, 428,  774, 428,  774, 428,  774, 25804,  2562, 428,  1374, 428,  772, 430,  1374, 430,  772, 428,  1374, 430,  774, 428,  770, 430,  1374, 428,  774, 428,  772, 428,  774, 428,  774, 25802,  2562, 428,  1374, 428,  772, 428,  1374, 430,  772, 430,  1374, 428,  774, 428,  772, 432,  1374, 428,  774, 428,  772, 430,  774, 428,  774};  // UNKNOWN CBAED686

void setup() {
  irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
 

  Serial.println("a rawData capture from IRrecvDumpV2");
  irsend.sendRaw(rawData, 181, 38);  // Send a raw data capture at 38kHz.
  delay(10000);

}
