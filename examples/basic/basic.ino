#include <Arduino.h>
#include <Lin_Interface.hpp>
#include <ibs_lite.hpp>

#DEFINE SERIAL_OUTPUT_INTERVAL_MS 1000

// using UART 2 for LinBus
Lin_Interface LinBus(2);
IBS_Lite BattSensor;

void setup()
{
  Serial.begin(115200);
  LinBus.baud = LIN_BAUDRATE_IBS_SENSOR;
  //  LinBus.verboseMode = 1;
  //  BattSensor.debug = 1;
  BattSensor.Linbus = &LinBus;
  // BattSensor.setCapacity(60000)
}

unsigned long timer = 0;

void loop()
{
  BattSensor.loop();

  if (timer - millis() > SERIAL_OUTPUT_INTERVAL_MS) {

    
    Serial.print("Battery Current: ");
    Serial.println(BattSensor.current(), 3);
    Serial.print("Battery Voltage: ");
    Serial.println(BattSensor.voltage());
    Serial.print("Battery Capacity: ");
    Serial.println(BattSensor.capacity());
    timer millis()

  }

}
