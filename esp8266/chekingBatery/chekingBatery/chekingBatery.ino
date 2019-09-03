// Pines para los LEDs
#define REDLED 5
#define BLUELED 4
#define GREENLED 0
//#define ANALOGPILA 0

//Variables
int ledDelay = 1000;
 
// Umbrales
float minBatteryCharge = 455;

ADC_MODE(ADC_VCC);

void setup() {
   Serial.begin(115200);
   setupExternalControlLed();
}

void loop() {
  checkBatteryStatus();
  delay(2000);
}

void checkBatteryStatus(){
  float vccVolt = ((float)ESP.getVcc())/1024;
  Serial.print(vccVolt);
  Serial.print("--");
  char* batteryStatus;
  
  if (vccVolt > minBatteryCharge){
    batteryStatus = "ok";
  }
  else{
    batteryStatus = "low";
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
