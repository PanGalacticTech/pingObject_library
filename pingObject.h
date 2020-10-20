/* ~~~~~~~ pingObject.h ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    UltraSonic Distance Sensor Object

    Modification of the example program, this one removes all delay() functions.
    Instead uses a state machine with a sequencer to control the timing
    of input & output events.

    This lets the sensor run around other, time critical software functions.



  Ping))) Sensor

  This sketch reads a PING))) ultrasonic rangefinder and returns the distance
  to the closest object in range. To do this, it sends a pulse to the sensor to
  initiate a reading, then listens for a pulse to return. The length of the
  returning pulse is proportional to the distance of the object from the sensor.

*/




#ifndef pingObject_h
#define pingObject_h

#if (ARDUINO >=100)
#include <Arduino.h>
#else
#include <wProgram.h>
#endif

#include <autoDelay.h>  // Requires Autodelay instance to handle timing functions. Library can be found @: https://github.com/PanGalacticTech/autoDelay_Library
#include "dataObject.h"   // Requires dataObject instance to handle filtering of sensor readings. Library can be found @ Locally, within this folder.



#define FILTER_SERIAL_OUT_TRUE 1
#define FILTER_SERIAL_OUT_FALSE 0


class pingObject {

  public:



    // Constructor

    pingObject(int16_t triggerOutputPin = 2, int16_t echoInputPin = 8, bool serialMonitor = true, uint32_t sensorSampleDelay = 100, bool manualTrigger = false, bool dataFilter = true, float filterBalance = 0.9):
      triggerPin(triggerOutputPin),
      echoPin(echoInputPin),
      printSerial(serialMonitor),
      sampleDelayMs(sensorSampleDelay),
      manualPing(manualTrigger),
      filterData(dataFilter),
      dataLib(filterBalance, FILTER_SERIAL_OUT_FALSE)
    {
    }


    //Setup

    void begin(uint32_t baudrate = 115200);

    //Methods

    void pingLoop();    // Calls all nessisary functions to return centimeters from ultrasound sensor

   int32_t pingDistance();  // Returns centimeters. Requires pingLoop to be called to calculate distance first.


    
    // Methods Used by pingLoop    
    void triggerPing(uint32_t delayMs = 100);

    void sendPing();

    void timeEcho();

    int32_t pingCalc(uint32_t echoDuration);


    // Other Methods (called by methods called by pingLoop())

    uint32_t microsecondsToCentimeters(uint32_t microseconds);

    // Serial Output Methods

    void printDistance_cm(int32_t distance_cm);

    void printFiltered_data(int32_t input);


    // Depreciated
    void pulseMeasure();


    // Global Variables

    int32_t centimeters;        // This is the important returned value to pass to other functions



  private:

    // Constructors for Other Objects:

    autoDelay pingControl, sampleDelay;   // First delay object handles the timings of hardware operations. //2nd delay object handles the delay for triggering sensor samples

    dataObject dataLib;                  // Data library used for recursive filter function & data smoothing

    // Global Variables

    uint32_t pulseDuration;             // returned as global variable every time new value is calculated from pingEcho

    uint8_t pingSequencer = 0;         //controls events sequence

    // Constants

    int16_t triggerPin;              //

    int16_t echoPin;

    // Option Variables (set in constructor)

    bool printSerial;

    bool filterData;

    bool manualPing;

    uint32_t sampleDelayMs = 100;   // Sample twice a second


    // Depreciated/unused

    void outputPinControl(bool input = false);

    bool triggerState;
    bool lastTriggerState;

    //  uint32_t pulseMeasure();

    //  uint32_t pulseStart;
    //  uint32_t pulseFinish;


};


#endif
