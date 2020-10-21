/* ~~~~~~~ pingObject.cpp ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

#include "pingObject.h"




// Begin Method

void pingObject::begin(uint32_t baudrate) {
  if (printSerial) {
    Serial.begin(baudrate);
  }
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(triggerPin, LOW);
}




//  sequencer function

void pingObject::pingLoop() {

  if (autoPing) {
    if (pingSequencer == 0) {                                         // state 0 - Armed Mode: function waits for delay timer to trigger the sequence by
      pingObject::timeTrigger(sampleDelayMs);                   //incrementing pingSequencer to 1;
    }
  }

  // pingSequencer 1 - 3;                                           // States 1 to 3 - Ping Output Mode: control hardware output pins to send a pulse HIGH to ultrasound sensor
  pingObject::sendPing();

  //pingSequencer 4-6;                                               // States 4 to 6 - Echo Return & Detect Mode: detect and measure the echo pulse from the ultrasound sensor
  pingObject::timeEcho();                                             // Returns pulseDuration as global variable


  if (pingSequencer == 7) {                                          // State 7 - Calculation Mode: triggers calculation sub routines to calculate distance from pulseDuration
    centimeters = pingObject::pingCalc(pulseDuration);                              // Sub routine resets pingSequencer to 0, and puts the program back in armed mode
  }

}



int32_t pingObject::pingDistance() {
  int32_t d = centimeters;
  return d;
}


void pingObject::triggerPing() {    // triggers sendPing by advancing pingSequencer to 1.
  pingSequencer = 1;                                           // Trigger Sample Bool
}


void pingObject::timeTrigger(uint32_t delayMs) {               // Counts elasped time since last ping recieved
  if (sampleDelay.millisDelay(delayMs)) {                   // if delay time is up,
    pingObject::triggerPing();
  }
}


void pingObject::sendPing() {

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  if (pingSequencer == 1) {                                         // cycle through ping sequence
    digitalWrite(triggerPin, LOW);                                  // Send Low pulse
    //  triggerState = LOW;
    if (pingControl.microsDelay(2)) {                              // time a 2 microsecond delay, before advancing the program
      pingSequencer++;
    }
  }


  if (pingSequencer == 2) {
    digitalWrite(triggerPin, HIGH);
    //  triggerState = HIGH;
    if (pingControl.microsDelay(5)) {
      pingSequencer++;
    }
  }

  if (pingSequencer == 3) {
    digitalWrite(triggerPin, LOW);
    //   triggerState = LOW;
    pingSequencer++;
  }
 

}




void pingObject::timeEcho() {


  uint32_t pulseStart;                           // These variables are not required outside of this function so they are both called here
  uint32_t pulseFinish;


  if (pingSequencer == 4) {                                   // Sequencer has been advanced to 3 by the sendPing function,
    if (digitalRead(echoPin)) {                                 // Read the echoPin untill echoPin reads HIGH
      pulseStart = micros();                              // Record pulse start time
      pingSequencer++;                               // advance sequencer
      loopEscape = 0;
    } else {
      loopEscape++;
      //  Serial.println(loopEscape);
    }
  }

  if (pingSequencer == 5) {                                      // if pulse has been detected & Sequencer is advanced to 4
    if (!digitalRead(echoPin)) {                                      // Read the echoPin untill echoPin reads LOW
      pulseFinish = micros();                                 // record the finish time for the pulse
      pingSequencer++;                                           // advance the pingSequencer
      loopEscape = 0;
    } else {
      loopEscape++;
      //  Serial.println(loopEscape);
    }
  }

  if  ( pingSequencer == 6) {                                      // if the pulse is complete
    pulseDuration = (pulseFinish - pulseStart);                // Calculate the pulse duration
    pingSequencer++;                                         // advance the pingSequencer
  }

  if (loopEscape >= 2000) {                                     // Method for escaping a loop if the echo is missed
    loopEscape = 0;
    pingSequencer = 0;
  }
}






int32_t pingObject::pingCalc(uint32_t echoDuration) {

  int32_t cm = microsecondsToCentimeters(echoDuration);         // convert the time into a distance

  int32_t cmFiltered = dataLib.recursiveFilter(cm);                                          // variable to hold filtered data value

  if (autoPing) {
    pingSequencer = 0;                                           //if autoPing is set, distance has been calculated so reset pingSequencer ready for the next trigger
  } else {
    pingSequencer++;                                             // else, advance pingSequencer to 8
  }

  if (printSerial) {
    pingObject::printDistance_cm(cm);                                    // Print the distance to serial monitor
    if (filterData) {
      pingObject::printFiltered_data(cmFiltered);                                 // If filterData also Print Filtered Data
    }
  }

  if (filterData) {
    return cmFiltered;
  } else {
    return cm;
  }
}




void pingObject::pingComplete() {
  Serial.println(pingSequencer);
  if (pingSequencer == 8) {   
    pingSequencer = 0;
    completePing = true;
    Serial.println("pingComplete");
    // return true;
  } else {
  //  return false;
  }
}

uint32_t pingObject::microsecondsToCentimeters(uint32_t microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}









void pingObject::printDistance_cm(int32_t distance_cm) {

  Serial.print(distance_cm);
  Serial.print(" cm   ");

  if (!filterData) {
    Serial.println("");
  } else {
    Serial.print("");
  }
}


void pingObject::printFiltered_data(int32_t input) {

  Serial.print(input);
  Serial.print(" cm - Filtered");
  Serial.println();

}





// Not currently in use > May be implemented later
void pingObject::outputPinControl(bool input) {

  if (input != lastTriggerState) {                       // If the state has changed
    digitalWrite(triggerPin, input);                  // set the trigger pin based on the state set by the sequencer
    lastTriggerState = input;                               // save the last state of the trigger pin

  }
}


// Old Depreciated Method

/*

  uint32_t pingObject::pulseMeasure() {


  uint32_t  duration_uS = pulseIn(echoPin, HIGH); // Old Depreciated version                // listen for the return pulse // This takes 29 microseconds per cm, if max distance is 300 cm, then max return time would be 9000uS or 9 milliseconds

  // This is the origional method, program paused while waiting for end of echoReturn

  return duration_uS;

  }


*/
