/*
  Ping))) Sensor

  This sketch reads a PING))) ultrasonic rangefinder and returns the distance
  to the closest object in range. To do this, it sends a pulse to the sensor to
  initiate a reading, then listens for a pulse to return. The length of the
  returning pulse is proportional to the distance of the object from the sensor.

*/

// this constant won't change. It's the pin number of the sensor's output:
#define triggerPin 2
#define echoPin 8

#include <autoDelay.h>

autoDelay pingControl;    // Used to control the timing of the ping functions

autoDelay sampleDelay;    //Used to delay individual sensor readings

uint32_t sampleDelayMs = 2000;   // Sample once a second






uint32_t pulseStart;
uint32_t pulseFinish;
uint32_t pulseDuration;

bool pulseDetected;
bool pulseComplete;






void setup() {
  // initialize serial communication:
  Serial.begin(115200);


  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(triggerPin, LOW);
}




bool triggerState;  // holds state of triggerPin
bool lastTriggerState;  //bool to hold the last trigger state. overkill but it means the digitalpin can be written to only when there is a state change

bool sampleNow;     // bool set by timer to trigger sensor sample

long duration_uS, cm;


uint8_t pingSequencer = 0; //controls sequence of ping pin


void loop() {


  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:


  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:


  if (sampleDelay.millisDelay(sampleDelayMs)) {                   // if delay time is up,
    sampleNow = true;                                              // Trigger Sample Bool
   // Serial.println("Sample Now:");
  }

  if (sampleNow) {                                             // if sampleNow is true

    if (pingSequencer == 0) {                                         // cycle through ping sequence
      digitalWrite(triggerPin, LOW);

      if (pingControl.microsDelay(2)) {
        pingSequencer++;
      }
    }

    if (pingSequencer == 1) {
      digitalWrite(triggerPin, HIGH);
      if (pingControl.microsDelay(5)) {
        pingSequencer++;
      }
    }

    if (pingSequencer == 2) {
      digitalWrite(triggerPin, LOW);
      //  duration_uS = pulseIn(echoPin, HIGH);                 // listen for the return pulse // This takes 29 microseconds per cm, if max distance is 300 cm, then max return time would be 9000uS or 9 milliseconds

      if (pingControl.microsDelay(9000)) {                           // wait for 9mS before resetting variables to start cycle again.

        pingSequencer = 0;
        sampleNow = false;
      }
    }
  }


  // Neat solution but it do not works

  /*
    if (triggerState != lastTriggerState){                        // If the state has changed
    digitalWrite(triggerPin, triggerState);                  // set the trigger pin based on the state set by the sequencer
    lastTriggerState = triggerState;                               // save the last state of the trigger pin
    }
  */

  timePulse();
  duration_uS = pulseDuration;

  if ( pulseComplete) {                            // if returns null it exits? if returns a figure it performs the

    cm = microsecondsToCentimeters(duration_uS);         // convert the time into a distance
    printDistance_cm(cm);
    pulseComplete = false;
  }







}


long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}


void printDistance_cm(uint32_t centimeters) {


  Serial.print(centimeters);
  Serial.print("cm");
  Serial.println();

}



uint8_t echoSequencer = 0;  // sequencer to control sequence of events for the returning pulse


void timePulse() {

  int pulseDetect = digitalRead(echoPin);

  Serial.println(pulseDetect);

  if (echoSequencer == 0) {                                // if pulse has not yet been detected

    if (pulseDetect == 1) {                                 // & echoPin reads HIGH
      pulseStart = micros();                              // Record time
      echoSequencer++;                               // advance sequencer

    }
  }

  if (echoSequencer == 1) {                                      // if pulse has been detected

    if (pulseDetect == 0) {                                      // if pulseDetect has read low
      pulseFinish = micros();                                 // record the finish time for the pulse
      echoSequencer++;
    }
  }

  if  ( echoSequencer == 3) {                                      // if the pulse is complete
    pulseComplete = true;                                     // set complete flag to false
    pulseDuration = (pulseFinish - pulseStart);                // Calculate the pulse duration
    Serial.println(pulseDuration);
    echoSequencer = 0;                                         // reset the sequencer ready to record the next event
  } else {
   // pulseDuration = 0;                                         // else return zero
  }
}
