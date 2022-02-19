#include <Lin_Interface.hpp>

#define LIN_BAUDRATE_IBS_SENSOR 19200
#define SECOND_AS_MS 1000
#define SECONDS_PER_HOUR 3600
#define IBS_LIN_ADDRESS 0x50

class IBS_Lite
{
public:
    // constructor
    IBS_Lite();

    // how often we read new data from the sensor ( ms )
    int updateInterval = 100;
    int debug = 0;

    // Lin Interface will be used to send and request frames
    Lin_Interface *Linbus = nullptr;

    float current();
    float voltage();
    uint32_t capacity();

    void resetCapacity();
    void setCapacity(uint32_t cap);

    void loop();

private:

    bool init = false;

    int battCap = -1;
    int savedCapacity = -1;

    bool readFrames();

    int Ibat = 0; // Current in MilliAmpere
    float Vbat = 0.0; // Battery Voltage in Volt

    float mAh = 0;
    int mAs = 0;    

    uint8_t samples = 0;

    unsigned long timer1 = 0;
    unsigned long timer2 = 0;
    unsigned long timer3 = 0;
};
