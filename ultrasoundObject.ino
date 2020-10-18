/* ~~~~~~~ UltraSonic Distance Sensor Object ~~~~~~~

    Modification of the example program, this one removes any delay() functions
    and instead uses a state machine with a sequencer to control the timing
    of input & output events.

    This lets the sensor run around other, time critical software functions.



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

uint32_t sampleDelayMs = 100;   // Sample twice a second







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
bool lastTriggerState = LOW;  //bool to hold the last trigger state. overkill but it means the digitalpin can be written to only when there is a state change

bool sampleNow;     // bool set by timer to trigger sensor sample

uint32_t cm;


uint8_t pingSequencer = 0; //controls sequence of ping pin


float filterBias = 0.75;   // Higher numbers = faster response less filtering // Lower numbers = Slower response, more filtering

int32_t recursiveFilter(int32_t Xn, float w = 0.9);  // Declared here as a bug fix (arduino usually does this itself but sometimes generates compiler errors)


void loop() {


  // State 0: - waiting for trigger
  if (pingSequencer == 0)   {                                         // State 0: Waiting to send ping
    if (sampleDelay.millisDelay(sampleDelayMs)) {                   // if delay time is up,
      pingSequencer++;                                            // Trigger Sample Bool
    }
  }

  // States 1 to 3: - Send Output Ping
  sendPing();                                   // Send ping runs every loop, activates timed sequence when is only active when pingSequencer goes to 1.

  // States 4 to 6: - Wait for echo and measure the length of the return pulse
  timePulse();

  // State 7: - When ping has returned calculate the distance & print value
  if (pingSequencer == 7) {                                     // When pingSequencer reaches 7, ping has been sent and recieved
    cm = microsecondsToCentimeters(pulseDuration);         // convert the time into a distance
    int32_t filteredData;
    
    filteredData = recursiveFilter(cm, filterBias);
    
    printDistance_cm(cm);                                    // Print the distance to serial monitor
   printFiltered_data(filteredData);
    pingSequencer = 0;                                           // distance has been calculated so reset pingSequencer ready for the next trigger
  }
}





void sendPing() {

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  if (pingSequencer == 1) {                                         // cycle through ping sequence
    digitalWrite(triggerPin, LOW);                                  // Send Low pulse
    if (pingControl.microsDelay(2)) {                              // time a 2 microsecond delay, before advancing the program
      pingSequencer++;
      sampleNow = 0;
    }
  }


  if (pingSequencer == 2) {
    digitalWrite(triggerPin, HIGH);

    if (pingControl.microsDelay(5)) {
      pingSequencer++;
    }
  }

  if (pingSequencer == 3) {
    digitalWrite(triggerPin, LOW);
    pingSequencer++;

  }
  // Still not a fan of this method, seems to run really slowly
  /*
    if (triggerState != lastTriggerState) {                       // If the state has changed
      digitalWrite(triggerPin, triggerState);                  // set the trigger pin based on the state set by the sequencer
      lastTriggerState = triggerState;                               // save the last state of the trigger pin
    }
  */

}









uint32_t microsecondsToCentimeters(uint32_t microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}





void printDistance_cm(uint32_t centimeters) {


  Serial.print(centimeters);
  Serial.print(" cm   ");
  Serial.print("");

}


void printFiltered_data(int16_t input) {

  Serial.print(input);
  Serial.print(" cm - Filtered");
  Serial.println();

}





void timePulse() {


  uint32_t pulseStart;                           // These variables are not required outside of this function so they are both called here
  uint32_t pulseFinish;


  if (pingSequencer == 4) {                                   // Sequencer has been advanced to 3 by the sendPing function,
    if (digitalRead(echoPin)) {                                 // Read the echoPin untill echoPin reads HIGH
      pulseStart = micros();                              // Record pulse start time
      pingSequencer++;                               // advance sequencer
    }
  }

  if (pingSequencer == 5) {                                      // if pulse has been detected & Sequencer is advanced to 4
    if (!digitalRead(echoPin)) {                                      // Read the echoPin untill echoPin reads LOW
      pulseFinish = micros();                                 // record the finish time for the pulse
      pingSequencer++;                                           // advance the pingSequencer
    }
  }

  if  ( pingSequencer == 6) {                                      // if the pulse is complete
    pulseDuration = (pulseFinish - pulseStart);                // Calculate the pulse duration
    pingSequencer++;                                         // advance the pingSequencer
  }
}






// Old Depreciated Method

void pulseMeasure() {


  //  duration_uS = pulseIn(echoPin, HIGH); // Old Depreciated version                // listen for the return pulse // This takes 29 microseconds per cm, if max distance is 300 cm, then max return time would be 9000uS or 9 milliseconds


}
