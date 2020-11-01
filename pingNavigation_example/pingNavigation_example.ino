/* ~~~~~~~~~~~|| << pingNavigation - Example >> ||~~~~~~~~~~

   ~~~~~~Using ~ UltraSonic Distance Sensor Object ~~~~~~~

  This example sketch outlines the basic method for using the

  pingNavigation.h

  library.

  Creating one instance of the pingNavigation library calls 2 pingObjects,
  integrated into one sensor system, which returns distance to the target
  from two different directions, and a quantized ranged output which can
  be used to control state logic, simplifying higher level decisions made
  by a navigating vehicle.


*/





// All dataObject stuff handled in pingObject.h
//#include "dataObject.h"

//#define FILTER_BIAS 0.5   // 0 to 1: Higher numbers = faster response less filtering // Lower numbers = Slower response, more filtering
//#define FILTER_SERIAL_OUT_TRUE 1
//#define FILTER_SERIAL_OUT_FALSE 0

//dataObject dataLib(FILTER_BIAS, FILTER_SERIAL_OUT_FALSE);


//#include "pingObject.h"

#include "pingNavigation.h"

// Options
#define TRIGGER_OUTPUT_PIN_L A0
#define ECHO_RECEIVE_PIN_L A1
#define PING_SERIAL_OUTPUT false
#define SENSOR_SAMPLE_DELAY_MS 200
#define AUTO_PING false                      // If AUTO_PING true, pingLoop runs at delay speed, else waits for pingObject.triggerPing();
#define DATA_FILTERING false
#define FILTER_BIAS 0.5                       // 0 to 1: Higher numbers = faster response less filtering // Lower numbers = Slower response, more filtering

//pingObject pingLeft(TRIGGER_OUTPUT_PIN_L, ECHO_RECEIVE_PIN_L, PING_SERIAL_OUTPUT, SENSOR_SAMPLE_DELAY_MS, AUTO_PING, DATA_FILTERING, FILTER_BIAS);

#define TRIGGER_OUTPUT_PIN_R A2
#define ECHO_RECEIVE_PIN_R A3

//pingObject pingRight(TRIGGER_OUTPUT_PIN_R, ECHO_RECEIVE_PIN_R , PING_SERIAL_OUTPUT, SENSOR_SAMPLE_DELAY_MS, AUTO_PING, DATA_FILTERING, FILTER_BIAS);

#define MONITOR_OUTPUT false
#define PING_TIMER_MS 1000                    // Controls delay between sensorPings

pingNavigation robotNav(TRIGGER_OUTPUT_PIN_L, ECHO_RECEIVE_PIN_L, TRIGGER_OUTPUT_PIN_R, ECHO_RECEIVE_PIN_R, PING_TIMER_MS, DATA_FILTERING, FILTER_BIAS, MONITOR_OUTPUT);


#include<autoDelay.h>

autoDelay testDelay;

uint32_t printDelay = 500;





void setup() {
  Serial.begin(115200); // if PING_SERIAL_OUTPUT is set true, Serial begin is handled by pingObject.begin(), else it must be called independently if Serial functions are required

  robotNav.navSetup();

}







char printString[42];


void loop() {


  robotNav.masterLoop();


  // if MONITOR_OUTPUT is false then data can be accessed with the following API

  if (testDelay.millisDelay(printDelay)) {
    sprintf(printString, "%i cm ||%s Left", robotNav.distanceLeft_cm, robotNav.distanceStateText[robotNav.distanceState_LEFT]);
    Serial.println(printString);

    sprintf(printString, "%i cm ||%s Right", robotNav.distanceRight_cm, robotNav.distanceStateText[robotNav.distanceState_RIGHT]);
    Serial.println(printString);


  }



}

//# ~~ End of Main Loop ~~ #
