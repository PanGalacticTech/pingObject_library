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


#include "pingNavigation.h"
//include <autoDelay.h>


void pingNavigation::navSetup(uint32_t baudrate) {
  if (serialMonitor) {
    Serial.begin(baudrate);
  }
  pingLeft.begin();     // Sets input & output pins & (starts serial communications (default 115200))(if PING_SERIAL_OUTPUT == true)
#if NUM_SENSORS == 2
  pingRight.begin();
#endif
}



void pingNavigation::masterLoop() {                    // Contains all methods to trigger pairs of sensors, return distance values & states

  pingNavigation::pingNavLoop();
  pingNavigation::autoTriggerPing();
  pingNavigation::completePings();


}





void pingNavigation::pingNavLoop() {

  if (sensorSelect == SENSOR_LEFT) {
    pingLeft.pingLoop();                      // pingLoop must be called to generate Distance in centimeters

  } else if (sensorSelect == SENSOR_RIGHT) {
#if NUM_SENSORS == 2
    pingRight.pingLoop();
#endif
#if NUM_SENSORS == 1
    sensorSelect = SENSOR_LEFT;    // Put this line here if not using the right sensor, else program will hang here. Comment line out if using right sensor
#endif
  }

}



void pingNavigation::autoTriggerPing() {


  if (pingTimer.millisDelay(sampleDelay)) {            // Timer to Trigger the pingSequence

    if (sensorSelect == SENSOR_LEFT) {
      pingLeft.triggerPing();
      if (EXTRA_MONITOR_OUTPUT) {
        Serial.println("Left Trigger Ping: ");
        Serial.println("|.... >>>>");
      }
    } else if (sensorSelect == SENSOR_RIGHT) {
#if NUM_SENSORS == 2
      pingRight.triggerPing();
      if (EXTRA_MONITOR_OUTPUT) {
        Serial.println("Right Trigger Ping: ");
        Serial.println("<<<< .....||");
      }
#endif
    }

  }
}





void pingNavigation::manualTriggerPing(uint8_t selectSensor) {

  if (selectSensor == SENSOR_LEFT) {
    pingLeft.triggerPing();
  } else if (selectSensor == SENSOR_RIGHT) {
    pingRight.triggerPing();
  }
}




void pingNavigation::completePings() {

  if (pingLeft.pingComplete()) {                      // function returns true if ping has been detected
    distanceLeft_cm = pingLeft.centimeters;
    distanceState_LEFT = pingNavigation::distanceStateCalculator(distanceLeft_cm);

    if (serialMonitor) {
      Serial.print(distanceLeft_cm);
      Serial.print(" cm left ||  ");
      Serial.println(" ");
      Serial.println("");
    }
    sensorSelect = SENSOR_RIGHT;
  }                                                  // This then needs some defensive code to switch incase the echo is missed so it doesnt hang forever.


  //if not using right sensor, comment out entire if statement (whats what endif does now)

#if NUM_SENSORS == 2
  if (pingRight.pingComplete()) {
    distanceRight_cm = pingRight.centimeters;
    distanceState_RIGHT = pingNavigation::distanceStateCalculator(distanceRight_cm);

    if (serialMonitor) {
      Serial.print(distanceRight_cm);
      Serial.print(" cm Right ||  ");
      Serial.println(" ");
      Serial.println(" ");
    }
    sensorSelect = SENSOR_LEFT;    //
  }
#endif

}



// Define limits for state machine. Turns a sliding scale of distance measurements into descrete options to simplify behavoural moddeling.
#define FAR_LIMIT 300
#define MIDFAR_LIMIT 200
#define MID_LIMIT 100
#define MIDCLOSE_LIMIT 60
#define CLOSE_LIMIT 40
#define VERYCLOSE_LIMIT 30
#define DANGERCLOSE_LIMIT 11
#define TOOCLOSE_LIMIT 10



uint8_t pingNavigation::distanceStateCalculator(uint32_t distance_cm) { // input distance in cm, output distanceState.


  switch (distance_cm) {
    case (distance_cm > FAR_LIMIT):
      return FAR;
      break;

    case (distance_cm > MIDFAR_LIMIT):
      return MIDFAR;
      break;

    case (distance_cm > MID_LIMIT):
      return MEDIUM;
      break;

    case (distance_cm > MIDCLOSE_LIMIT):
      return MIDCLOSE;
      break;

    case (distance_cm > CLOSE_LIMIT):
      return CLOSE;
      break;

    case (distance_cm > VERYCLOSE_LIMIT):
      return VERY_CLOSE;
      break;

    case (distance_cm >= DANGERCLOSE_LIMIT):
      return DANGER_CLOSE;
      break;

    case (distance_cm < TOOCLOSE_LIMTI):
      return TOO_CLOSE;
      break;

    default:
      return NA;
      break;

  }


}
