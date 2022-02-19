# IBS-Sensor-Lite
Interface to IBS 200 X Automotive Battery Sensor via LIN-Bus.

This repository is derivative work based on this existing library: https://github.com/mestrode/IBS-Sensor-Library

<br>

# Background
For my project I was looking for a way to add battery monitoring to my vehicle, due to the long cold winters in Canada; it is nice to monitor the characteristics of the battery. 

Lots of modern vehicles today include battery monitoring, usually in the form of either a Shunt based battery monitor ( Like the [IBS-200X](https://www.hella.com/truck/assets/media/GE7_Intelligent%20Battery%20Sensor%20(IBS)_HELLA_EN.pdf) ) or Hall Effect Sensor ( Like this [HAB 80-S](https://www.lem.com/sites/default/files/products_datasheets/hab%2080-s.pdf) )

As is with alot of my projects, I like to re-use existing things and find new purposes for them. So instead of buying a brand new sensor, I decided to take a look at what was available at my local wrecking yard.

Hella is an OEM provider to many existing car manufacturers as I discovered. I found IBS-200X for Hyundai/Kia and some Ford models as well. 

<br>

# Reverse Engineering
With some Automotive IBS-200X on hand I was now tasked with connecting to them and seeing what kind of data I could read from the sensor. I referred to the few established github projects that were already interfacing with IBS-200X only to notice that they were not working with any of the sensors I was able to salvage. 

After many head scratching hours, I came to conclusion that these Automotive IBS-200X likely use a very different firmware from the one that comes with official Hella branded IBS-200X. 

Automotive IBS-200X would only respond to request on address **0x50** ( also **0x92**, with static data.) 
This is very strange behavior because LIN format specifies an ID range from: **0x00-0x3F**
<br>


    Response frame for address 0x50

    IBS-200X connected to bench power supply with 12V + 3.0A load

    55      50      DD:73:62:97:1E:F8:EA:FF     62
    sync    addr    |  |  |  |   |               chksm
                    IL IU |  |   |              Ibat = (IU << 8)) | IL // current in milliamps
                          VL VU  |              Vbat = (VU << 8)) | VL // uncalibrated voltage value
                                 MULT           this byte changes when the current load goes above ~16amps.

<br>

In this repository I provide some [raw linbus output](docs/IBS_Readings.txt) with the sensor at various power characteristics ( different voltage, current, temperature. ) Feel free to read and suggest improvements to sensor reading formulas.  

<br>

# Dependencies
The Lin Interface utilizes a Hardware Serial (UART) of an ESP32. This requires a separate Library: https://github.com/mestrode/Lin-Interface-Library

<br>


# Example

Below is a very basic exmaple of how to use this library. 

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
        LinBus.baud = LIN_BAUDRATE_IBS_SENSOR; // 19200
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

<br>


# Hardware
This library was designed for use with the ESP32, however with some minor modifications can likely be tweaked to support most arduino compatible microcontrollers. 

You will also require a LIN Transceiver to convert the logic level serial output into appropriate 12V signaling required for LIN Bus. I used this transceiver board from Mikroe based on the MCP2003B: https://www.mikroe.com/mcp2003b-click

If you do not have access to a transceiver there is a way to do this using some basic transistors, I will provide a schematic of this in the future.

<br>

# References
If you use official Hella IBS-200X, please refer to the original creators repository: 
https://github.com/mestrode/IBS-Sensor-Library
