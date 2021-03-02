#include <kia_amplifier_serial.h>
#include <kia_amplifier_can.h>
#include <Arduino.h>

#define CS_PIN 10

KiaAmplifierCan amplifierCan(CAN_100KBPS, MCP_8MHZ, CS_PIN);
KiaAmplifierSerial amplifierSerial(38400);

byte sendingData[8] = {10, 10, 10, 10, 10, 10, 255, 255};

long lastTime = 0;
const int interval = 1000;

void setup() { 
  amplifierSerial.init();
  amplifierCan.init();
  CanDataKeeper keeper = amplifierSerial.readEprom();
  if(keeper.isDataValid) {
     for(int ii=0;ii<8;ii++) {
       sendingData[ii] = keeper.data[ii];
     };
  }  
}

void loop() {

  if (Serial.available() > 0)  {
    CanDataKeeper keeper = amplifierSerial.readNewValues(sendingData);
    if(keeper.isDataValid) {
      for(int ii=0;ii<8;ii++) {
        sendingData[ii] = keeper.data[ii];
      }
      amplifierCan.sendCAN(sendingData);
    }
  }

  if (millis() - lastTime > interval) { 
    lastTime = millis();
    amplifierCan.sendCAN(sendingData);
  }

}
