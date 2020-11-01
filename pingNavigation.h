/* || ::::::  pingNavigation.h :::::: ||

    Library to automate pairs of ultrasound sensors
    in an integrated navigation system.


  Creating one instance of the pingNavigation library calls 2 pingObjects,
  integrated into one sensor system, which returns distance to the target
  from two different directions, and a quantized ranged output which can
  be used to control state logic, simplifying higher level decisions made
  by a navigating vehicle.

  Variables/Options modified from pingObject:
  PING_SERIAL_OUTPUT disabled by default
  AUTO_PING          disabled by default
  SENSOR_SAMPLE_DELAY (default value 200 is passed as argument
  but disabled. Instead this variable is used for the delay
  controlling the pair of sensors)

*/


PING_SERIAL_OUTPUT disabled by default
AUTO_PING          disabled by default
  SENSOR_SAMPLE_DELAY (default value 200 is passed as argument
                           but disabled. Instead this variable is used for the delay
                           controlling the pair of sensors)
        * /


#ifndef pingNavigation_h
#define pingNavigation_h

#if (ARDUINO >=100)
#include <Arduino.h>
#else
#include <wProgram.h>
#endif


#include "pingObject.h"
#include <autoDelay.h>


        //#define NUM_SENSORS 2

#define SENSOR_LEFT 0
#define SENSOR_RIGHT 1

#define EXTRA_MONITOR_OUTPUT false




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


            void navSetup(uint32_t baudRate = 115200);

            void masterLoop();

            void pingNavLoop();                 // "mirror" of pingLoop from pingObject, but splits up loop time between SENSOR_LEFT, & SENSOR_RIGHT

            void autoTriggerPing();

            void manualTriggerPing(uint8_t selectSensor = SENSOR_LEFT);

            void completePings();

            uint8_t distanceStateCalculator(uint32_t distance_cm);        // input distance in cm, output distanceState.

            void timeoutSensor();

            uint16_t pingsRxed = 0;   // variable to count number of pings rxed for debugging

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

            char naText[14]          = {"N/A          "};
            char tooCloseText[14]    = {"Too Close    "};
            char dangerCloseText[14] = {"Danger Close "};
            char veryCloseText[14]   = {"Very Close   "};
            char closeText[14]       = {"Close        "};
            char midCloseText[14]    = {"Medium Close "};
            char midText[14]         = {"Medium       "};
            char midFarText[14]      = {"Medium Far   "};
            char farText[14]         = {"Far          "};
            char blankText[14]       = {"             "};




            char *distanceStateText[11] = {naText, tooCloseText, dangerCloseText, veryCloseText, closeText, midCloseText, midText, midFarText, farText, blankText};

            char distanceStatePrintout[16];   // this is the string that gets printed

#define FAR_LIMIT 300
#define MIDFAR_LIMIT 200
#define MID_LIMIT 100
#define MIDCLOSE_LIMIT 60
#define CLOSE_LIMIT 40
#define VERYCLOSE_LIMIT 30
#define DANGERCLOSE_LIMIT 11
#define TOOCLOSE_LIMIT 10

            uint32_t timeoutTime = 2000;   // Times out and switches sensors after 2 seconds without a reply

            uint32_t lastPing;

          private:

            bool sensorArmed = true;   // bool to lock out retriggers of sensor if waiting for a pulse

            bool serialMonitor;   //


            uint8_t sensorSelect = 0;  //

            uint32_t sampleDelay;


        };

#endif
