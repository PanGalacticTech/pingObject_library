/* || ::::::  pingNavigation.h :::::: ||

    Library to automate pairs of ultrasound sensors
    in an integrated navigation system.

    Inputs:

    Left Trigger Pin
    Left Echo Pin

    Right Trigger pin
    Right Echo Pin

    Sample Delay

   Data Filtering
   Filtering Bias

   monitorOutput

   distanceBand (Close, mediumClose, medium, mediumFar, far)


   Outputs:

   distanceLeft
   distanceRight

   distanceBandLeft
   distanceBandRight



   PING_SERIAL_OUTPUT disabled by default
   AUTO_PING          disabled by default
   SENSOR_SAMPLE_DELAY (default value 200 is passed as argument
   but disabled. Instead this variable is used for the delay
   controlling the pair of sensors)
*/


#ifndef pingNavigation_h
#define pingNavigation_h

#if (ARDUINO >=100)
#include <Arduino.h>
#else
#include <wProgram.h>
#endif


#include "pingObject.h"
#include <autoDelay.h>


#define NUM_SENSORS 2

#define SENSOR_LEFT 0
#define SENSOR_RIGHT 1

#define EXTRA_MONITOR_OUTPUT true




class pingNavigation {

  private:

    pingObject pingLeft, pingRight;



  public:

    //Constructor

    pingNavigation(int LEFTtriggerPin = A0, int LEFTechoPin = A1,
                   int RIGHTtriggerPin = A2, int RIGHTechoPin = A3,
                   uint32_t pingSampleDelay = 1000,
                   bool dataFilter = true, float filterBalance = 0.9, bool monitorOutput = true):
      pingLeft(LEFTtriggerPin, LEFTechoPin, false, 200, false, dataFilter, filterBalance),
      pingRight(RIGHTtriggerPin, RIGHTechoPin, false, 200, false, dataFilter, filterBalance)
    {
      serialMonitor = monitorOutput;
      sampleDelay = pingSampleDelay;
    }


    void navSetup(uint32_t baudRate = 152000);

    void pingNavLoop();

    uint8_t distanceStateCalculator(uint32_t distance_cm);   // input distance in cm, output distanceState.

    uint16_t distanceLeft_cm;
    uint16_t distanceRight_cm;

    uint8_t distanceState_LEFT = 0;
    uint8_t distanceState_RIGHT = 0;

    enum distanceState
    {
      NA = 0,
      TOO_CLOSE = 1,
      DANGER_CLOSE = 2,
      VERY_CLOSE = 3,
      CLOSE = 4,
      MIDCLOSE = 5,
      MEDIUM = 6,
      MIDFAR = 7,
      FAR = 8
    };

    autoDelay pingTimer;



  private:

    bool serialMonitor;   //


    uint8_t sensorSelect = 0;  //

    uint32_t sampleDelay;


};

#endif
