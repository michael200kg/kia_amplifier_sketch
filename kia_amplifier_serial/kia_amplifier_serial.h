#include <SPI.h>
#include <EEPROM.h>
#ifndef KiaAmplifierSerial_h
#define KiaAmplifierSerial_h

struct CanDataKeeper {
   boolean isDataValid;
   byte data[8];
   CanDataKeeper(boolean isDataValid_, byte data_[8]) {
     isDataValid = isDataValid_;
     for(int ii=0;ii<8;ii++) {
        data[ii] = data_[ii];
     }
   }; 
};

class KiaAmplifierSerial {
   
   public:
   	  KiaAmplifierSerial(long bound_rate) : 
   	     BOUND_RATE {bound_rate},
   	     sendingData {10, 10, 10, 10, 10, 10, 255, 255} {};
      void init();
      CanDataKeeper readEprom();
      CanDataKeeper generateNewData();
      CanDataKeeper readNewValues(byte sData[8]);

   private:
      long BOUND_RATE; 
      byte sendingData[8];
      byte receive[10];
};

#endif