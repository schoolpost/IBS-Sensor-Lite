#include <ibs_lite.hpp>

// constructor of class IBS_Sensor
IBS_Lite::IBS_Lite() {
    // TODO
    // load values from EEPROM or FLASH 
    init = true;
};

bool IBS_Lite::readFrames(){
    bool chksum = Linbus->readFrame(IBS_LIN_ADDRESS);
    if(chksum){

        timer1 = millis();

        uint8_t multiplier  = 1;
        if(Linbus.LinMessage[4] & 0x80){
            multiplier = 10;
        }

        int16_t cur = ((int16_t)Linbus->LinMessage[1] << 8) | Linbus->LinMessage[0];
        int cDir = cur & 0x8000;
        if (cDir) {
            Ibat = (0x8000 + cur) * 1;
        } else {
            Ibat = (0x8000 - cur) * -1;
        }

        uint16_t volt = ((int16_t)Linbus->LinMessage[3] << 8) | Linbus->LinMessage[2];
        int16_t vDir = volt & 0x4000;
        Vbat = 11.9883 + (((volt & 0x3fff) - 5987) / 1000.0);

        if(debug){

            Serial.print(cDir);
            Serial.print(", ");
            Serial.print(Ibat);
            Serial.print(" | ");
            Serial.print(vDir);
            Serial.print(", ");
            Serial.println(Vbat);

        }


    }
    return chksum;
}

void IBS_Lite::loop(){

    if(millis() - timer2 > updateInterval){
        if(readFrames()){
            samples++;
            mAs = mAs + Ibat;
        }
        timer2 = millis();
    }

    if(millis() - timer3 > SECOND_AS_MS){
        float offset = ((float)mAs / (float)samples) / (float)SECONDS_PER_HOUR;
        mAh = mAh + offset;
        samples = 0;
        mAs = 0;
        timer3 = millis();
    }
}

float IBS_Lite::current(){
    return ((float)Ibat) / 1000.0;
}

float IBS_Lite::voltage(){
    return Vbat;
}

uint32_t IBS_Lite::capacity(){
    return (uint32_t)mAh;
}

void IBS_Lite::setCapacity(uint32_t cap){
    mAh = (float)cap;
}

void IBS_Lite::resetCapacity(){
    if(savedCapacity > -1){
        mAh = (float)savedCapacity;
    }
        
}
