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




class pingObject {

  public:

    autoDelay pingControl, sampleDelay;   // First delay object handles the timings of hardware operations. //2nd delay object handles the delay for triggering sensor samples

    // Constructor

    pingObject(int16_t triggerInputPin = 2, int16_t echoInputPin = 8, bool serialMonitor = true):
      triggerPin(triggerInputPin),
      echoPin(echoInputPin),
      printSerial(serialMonitor)
    {
    }


    //Setup

    void begin(uint32_t baudrate = 115200);

    //Methods

    void sendPing();

    void timeEcho();


    uint32_t microsecondsToCentimeters(uint32_t microseconds)



    void printDistance_cm(uint32_t centimeters)

    void printFiltered_data(int16_t input)


    // Depreciated
    void pulseMeasure();


    // Variables

    uint32_t cm;



  private:

    uint32_t pulseDuration;

    uint8_t pingSequencer = 0;         //controls events sequence

    bool printSerial;

};


#endif
