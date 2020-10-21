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






#include <autoDelay.h>

//autoDelay pingControl;    // Used to control the timing of the ping functions

//autoDelay sampleDelay;    //Used to delay individual sensor readings


autoDelay dualSensor;

#define DUAL_SENSOR_DELAY 500

autoDelay slowPrinting;


// All dataObject stuff handled in pingObject.h
//#include "dataObject.h"

//#define FILTER_BIAS 0.5   // 0 to 1: Higher numbers = faster response less filtering // Lower numbers = Slower response, more filtering
//#define FILTER_SERIAL_OUT_TRUE 1
//#define FILTER_SERIAL_OUT_FALSE 0

//dataObject dataLib(FILTER_BIAS, FILTER_SERIAL_OUT_FALSE);


#include "pingObject.h"

// Options
#define TRIGGER_OUTPUT_PIN_L 2
#define ECHO_RECEIVE_PIN_L 8
#define PING_SERIAL_OUTPUT false
#define SENSOR_SAMPLE_DELAY_MS 200
#define AUTO_PING false                      // If AUTO_PING true, pingLoop runs at delay speed, else waits for pingObject.pingTrigger();
#define DATA_FILTERING false
#define FILTER_BIAS 0.5                       // 0 to 1: Higher numbers = faster response less filtering // Lower numbers = Slower response, more filtering

pingObject pingLeft(TRIGGER_OUTPUT_PIN_L, ECHO_RECEIVE_PIN_L , PING_SERIAL_OUTPUT, SENSOR_SAMPLE_DELAY_MS, AUTO_PING, DATA_FILTERING, FILTER_BIAS);

#define TRIGGER_OUTPUT_PIN_R 9
#define ECHO_RECEIVE_PIN_R 10

pingObject pingRight(TRIGGER_OUTPUT_PIN_R, ECHO_RECEIVE_PIN_R , PING_SERIAL_OUTPUT, SENSOR_SAMPLE_DELAY_MS, AUTO_PING, DATA_FILTERING, FILTER_BIAS);




void setup() {
  Serial.begin(115200); // if PING_SERIAL_OUTPUT is set true, Serial begin is handled by pingObject.begin(), else it must be called independently if Serial functions are required

  pingLeft.begin();     // Sets input & output pins & (starts serial communications (default 115200))(if PING_SERIAL_OUTPUT = true
  pingRight.begin();
}


uint8_t sensorSelect = 0;  //


void loop() {





  if (sensorSelect == 0) {
    pingLeft.pingLoop();                       // pingLoop must be called to generate Distance in centimeters
  } else if (sensorSelect == 1) {
    pingRight.pingLoop();
  }

  if (slowPrinting.millisDelay(1000)) {

    sensorSelect++;
    if (sensorSelect == 2) {
      sensorSelect = 0;
    }
    if (sensorSelect == 0) {
      pingLeft.triggerPing();
      Serial.print("Left Trigger Ping: ");
      Serial.println(pingLeft.pingSequencer);
    } else if (sensorSelect == 1) {
      pingRight.triggerPing();
      Serial.print("Right Trigger Ping: ");
      Serial.println(pingRight.pingSequencer);
    }

  }





  // Serial.println(pingLeft.pingSequencer);

  // pingLeft.pingComplete();
  // Serial.print(pingLeft.pingDistance());     // Used to return just the distance in centimeters

pingLeft.pingComplete();  // Checks to see if ping has been completed

  if (pingLeft.completePing) {
    Serial.print(pingLeft.centimeters);
    Serial.print(" cm left || pingSequencer:  ");
    //Serial.print(pingRight.centimeters);
    // Serial.println(" cm right ");
    Serial.print(" || loopEscape: ");
    Serial.print(pingLeft.loopEscape);
    Serial.println(" ");
    // pingLeft.completePing = false;
  } 

  
  if (pingRight.completePing) {
    Serial.print(pingRight.centimeters);
    Serial.print(" cm Right || pingSequencer:  ");

    Serial.print(" || loopEscape: ");
    Serial.print(pingRight.loopEscape);
    Serial.println(" ");
    // pingLeft.completePing = false;
  } 

  
}
