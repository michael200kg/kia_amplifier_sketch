#include <SPI.h>
#include <EEPROM.h>
#include <kia_amplifier_serial.h>

void KiaAmplifierSerial::init() {
  while (!Serial);
  Serial.begin(BOUND_RATE);
  SPI.begin();
};

CanDataKeeper KiaAmplifierSerial::readEprom() {
	  //reading stored settings
  byte stnTmp[8];
  int sumTmp;
  boolean isDataValid = true;
  for (int i = 0; i < 8; i++) {
    stnTmp[i] = EEPROM.read(i);
    sumTmp = sumTmp + EEPROM.read(i);
  }

  Serial.println("Summ in EEPROM = " + String(sumTmp));
  if (sumTmp != 388 && sumTmp != 2048) {
    Serial.println(F("loaded from eeprom"));
    memccpy(sendingData, stnTmp, "", 8);
    for (int i = 0; i < sizeof sendingData; i++) {
      Serial.println(sendingData[i], HEX);
    }
  } else {
    Serial.println(F("Error while loading from EEPROM"));
    isDataValid = false;
  }
  return CanDataKeeper(isDataValid, sendingData);
};

CanDataKeeper KiaAmplifierSerial::generateNewData() {
  int ret = 0;
  int CheckSum = 0;
  boolean isDataValid = true;

  for (int i = 0; i < sizeof receive - 2; i++) {
    CheckSum = (int) receive[i] + CheckSum;
  }

  if (((CheckSum & 255) ^ 255) == (int)receive[8]) {
    ret = 1;
    memccpy(sendingData, receive, "", 8);

    for (int i = 0; i < sizeof sendingData - 1; i++) {
      EEPROM.write(i, sendingData[i]);
    }

  } else {
    Serial.println(F("Chsumm fail"));
    isDataValid = false;
  }

  return CanDataKeeper(isDataValid, sendingData);
};

CanDataKeeper KiaAmplifierSerial::readNewValues(byte sData[8]) {
    for(int ii=0;ii<8;ii++) {
      sendingData[ii] = sData[ii];
    }
    Serial.setTimeout(20);
    Serial.readBytesUntil(77, receive, 10);
    Serial.println("Recieved bytes via serial: ");
    for(int ii=0;ii<10;ii++) {
      Serial.print( String(receive[ii]) + " " );
    }
    Serial.println("");
    return generateNewData();
};