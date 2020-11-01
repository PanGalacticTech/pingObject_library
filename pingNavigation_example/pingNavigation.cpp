/* || ::::::  pingNavigation.cpp :::::: ||

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


#include "pingNavigation.h"



void pingNavigation::navSetup(uint32_t baudrate) {
  if (serialMonitor) {
    Serial.begin(baudrate);
  }
  pingLeft.begin();     // Sets input & output pins & (starts serial communications (default 115200))(if PING_SERIAL_OUTPUT == true)
  //#if NUM_SENSORS == 2
  pingRight.begin();
  //#endif
}



void pingNavigation::masterLoop() {                    // Contains all methods to trigger pairs of sensors, return distance values & states

  pingNavigation::pingNavLoop();
  pingNavigation::autoTriggerPing();
  pingNavigation::completePings();
  pingNavigation::timeoutSensor();


}





void pingNavigation::pingNavLoop() {

  if (sensorSelect == SENSOR_LEFT) {
    pingLeft.pingLoop();                      // pingLoop must be called to generate Distance in centimeters

  } else if (sensorSelect == SENSOR_RIGHT) {
    //#if NUM_SENSORS == 2
    pingRight.pingLoop();
    //#endif
    //#if NUM_SENSORS == 1
    //    sensorSelect = SENSOR_LEFT;    // Put this line here if not using the right sensor, else program will hang here. Comment line out if using right sensor
    //#endif
  }

}



void pingNavigation::autoTriggerPing() {

  if (sensorArmed) {

    if (pingTimer.millisDelay(sampleDelay)) {            // Timer to Trigger the pingSequence

      if (sensorSelect == SENSOR_LEFT) {
        pingLeft.triggerPing();
        if (EXTRA_MONITOR_OUTPUT) {
          Serial.println("Left Trigger Ping: ");
          Serial.println("|.... >>>>");
        }
      } else if (sensorSelect == SENSOR_RIGHT) {
        //#if NUM_SENSORS == 2
        pingRight.triggerPing();
        if (EXTRA_MONITOR_OUTPUT) {
          Serial.println("Right Trigger Ping: ");
          Serial.println("<<<< .....||");
        }
        //#endif
      }
      sensorArmed = false;
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

    lastPing = millis();   // Can be used to escape a stuck loop
    pingsRxed++;

    if (serialMonitor) {
      Serial.print(distanceLeft_cm);
      Serial.print(" cm left ||  ");
    //  memset(distanceStatePrintout, 0, 16);
    
      sprintf(distanceStatePrintout, "%s", distanceStateText[distanceState_LEFT]);
      Serial.print(distanceStatePrintout);
    
      Serial.print("     |");
      Serial.print(pingsRxed);
      //Serial.print(distanceState_LEFT);
      Serial.println(" ");
      Serial.println("");
    }
    sensorSelect = SENSOR_RIGHT;
    sensorArmed = true;
  }                                                  // This then needs some defensive code to switch incase the echo is missed so it doesnt hang forever.


  //if not using right sensor, comment out entire if statement (whats what endif does now)

  //#if NUM_SENSORS == 2
  if (pingRight.pingComplete()) {
    distanceRight_cm = pingRight.centimeters;
    distanceState_RIGHT = pingNavigation::distanceStateCalculator(distanceRight_cm);

    lastPing = millis();   // Can be used to escape a stuck loop
    pingsRxed++;

    if (serialMonitor) {
      Serial.print(distanceRight_cm);
      Serial.print(" cm Right ||  ");

      //   memset(distanceStatePrintout, 0, 16);
   //   distanceStatePrintout = &distanceStateText[10];  // Write a blank line

      sprintf(distanceStatePrintout, "%s", distanceStateText[distanceState_RIGHT]);
      Serial.print(distanceStatePrintout);

      Serial.print("     |");
      Serial.print(pingsRxed);
      //  Serial.print(distanceState_RIGHT);
      Serial.println(" ");
      Serial.println(" ");

    }
    sensorSelect = SENSOR_LEFT;    //
    sensorArmed = true;
  }
  //#endif

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

  uint8_t output;

  if (distance_cm > FAR_LIMIT) {
    output = FAR;

  } else if (distance_cm > MIDFAR_LIMIT) {
    output = MIDFAR;

  } else if (distance_cm  > MID_LIMIT) {
    output = MEDIUM;

  } else if (distance_cm > MIDCLOSE_LIMIT) {
    output = MIDCLOSE;

  } else if (distance_cm > CLOSE_LIMIT) {
    output = CLOSE;

  } else if (distance_cm > VERYCLOSE_LIMIT) {
    output = VERY_CLOSE;

  } else if ( distance_cm > DANGERCLOSE_LIMIT) {
    output = DANGER_CLOSE;

  } else if (distance_cm < TOOCLOSE_LIMIT) {
    output = TOO_CLOSE;

  } else {
    output =  NA;
  }

  return output;

}



void pingNavigation::timeoutSensor() {   // method to switch sensors if


  if (millis() - lastPing >= timeoutTime) { // Can be used to escape a stuck loop

    Serial.println("~~~~~~~~~~SWITCH~~~~~~~~~~~~~");

    if (sensorSelect == SENSOR_LEFT) {
      sensorSelect = SENSOR_RIGHT;
    } else {
      sensorSelect = SENSOR_LEFT;
    }

    pingRight.pingSequencer = 0;
    pingLeft.pingSequencer = 0;
    sensorArmed = true;
    lastPing = millis();
  }

}
