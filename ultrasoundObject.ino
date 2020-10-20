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
#define PING_SERIAL_OUTPUT true
#define SENSOR_SAMPLE_DELAY_MS 200
#define MANUAL_PING false                       // If MANUAL_PING true, pingLoop waits for trigger, if false runs at delay speed
#define DATA_FILTERING false
#define FILTER_BIAS 0.5                       // 0 to 1: Higher numbers = faster response less filtering // Lower numbers = Slower response, more filtering

pingObject pingLeft(TRIGGER_OUTPUT_PIN_L, ECHO_RECEIVE_PIN_L , PING_SERIAL_OUTPUT, SENSOR_SAMPLE_DELAY_MS, MANUAL_PING, DATA_FILTERING, FILTER_BIAS);

#define TRIGGER_OUTPUT_PIN_R 1
#define ECHO_RECEIVE_PIN_R 0

pingObject pingRight(TRIGGER_OUTPUT_PIN_R, ECHO_RECEIVE_PIN_R , PING_SERIAL_OUTPUT, SENSOR_SAMPLE_DELAY_MS, MANUAL_PING, DATA_FILTERING, FILTER_BIAS);




void setup() {
  Serial.begin(115200); // if PING_SERIAL_OUTPUT is set true, Serial begin is handled by pingObject.begin(), else it must be called independently if Serial functions are required 
  
  pingLeft.begin();     // Sets input & output pins & (starts serial communications (default 115200))(if PING_SERIAL_OUTPUT = true
// pingRight.begin();
}






void loop() {

  pingLeft.pingLoop();                       // pingLoop must be called to generate Distance in centimeters
 // pingRight.pingLoop();


// Serial.print(pingLeft.pingDistance());     // Used to return just the distance in centimeters
// Serial.print(pingLeft.centimeters);
 // Serial.println(" cm left || ");

 // Serial.print(pingRight.centimeters);
 // Serial.println(" cm right ");
  

  
}
