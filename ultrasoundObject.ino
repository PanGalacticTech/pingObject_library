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
#define AUTO_PING false                      // If AUTO_PING true, pingLoop runs at delay speed, else waits for pingObject.triggerPing();
#define DATA_FILTERING false
#define FILTER_BIAS 0.5                       // 0 to 1: Higher numbers = faster response less filtering // Lower numbers = Slower response, more filtering

pingObject pingLeft(TRIGGER_OUTPUT_PIN_L, ECHO_RECEIVE_PIN_L, PING_SERIAL_OUTPUT, SENSOR_SAMPLE_DELAY_MS, AUTO_PING, DATA_FILTERING, FILTER_BIAS);

#define TRIGGER_OUTPUT_PIN_R 9
#define ECHO_RECEIVE_PIN_R 10

pingObject pingRight(TRIGGER_OUTPUT_PIN_R, ECHO_RECEIVE_PIN_R , PING_SERIAL_OUTPUT, SENSOR_SAMPLE_DELAY_MS, AUTO_PING, DATA_FILTERING, FILTER_BIAS);




void setup() {
  Serial.begin(115200); // if PING_SERIAL_OUTPUT is set true, Serial begin is handled by pingObject.begin(), else it must be called independently if Serial functions are required

  pingLeft.begin();     // Sets input & output pins & (starts serial communications (default 115200))(if PING_SERIAL_OUTPUT = true
  pingRight.begin();
}


uint8_t sensorSelect = 0;  //

uint32_t defenseCount = 0;

uint8_t previousMode;

void loop() {


  if (sensorSelect == 0) {
    pingLeft.pingLoop();                       // pingLoop must be called to generate Distance in centimeters
    Serial.println("CURRENTLY DOING PING.LEFT >>>");

  } else if (sensorSelect == 1) {
    pingRight.pingLoop();
    Serial.println("<<< currently doing pingRight. ");

  }


  if (slowPrinting.millisDelay(1000)) {            // State Machine

    if (sensorSelect == 0) {
      //   sensorSelect = 1;                               // if just one sensor is active put this state machine mode change here NO BAD NAH BADBOI
      pingLeft.triggerPing();
      Serial.println("Left Trigger Ping: ");
      Serial.println(".... >>>>");
    } else if (sensorSelect == 1) {
      //   sensorSelect = 0;
      pingRight.triggerPing();
      Serial.println("Right Trigger Ping: ");
      Serial.println(".... >>>>");
      // Serial.println("");
    }

  }







  if (pingLeft.pingComplete()) {
    Serial.print(pingLeft.centimeters);
    Serial.print(" cm left ||  ");
    Serial.println(" ");
    Serial.println("");
    sensorSelect = 1;                               // if both sensors are active, change mode here, as then the mode waits for the other sensor to report back before moving to the next one
  }                                                  // This then needs some defensive code to switch incase the echo is missed so it doesnt hang forever.


  if (pingRight.pingComplete()) {
    Serial.print(pingRight.centimeters);
    Serial.print(" cm Right ||  ");
    Serial.println(" ");
    Serial.println(" ");
    sensorSelect = 0;    //
  }




  if (previousMode == sensorSelect) {              // Start counting when previousMode is equal to the current mode
    defenseCount++;
    Serial.println(defenseCount);
  } else {                                       // if they are different
    previousMode = sensorSelect;                  // Save the current mode
    defenseCount = 0;                             // Reset the count
    Serial.println(defenseCount);
  }


  if (defenseCount >= 1000) {                  // If the count reaches 1000, assumed that loops has become stuck
    Serial.println("defensive mode switch");
    defenseCount = 0;                                // Reset the count
    if (sensorSelect == 0) {                             // Change the mode which resets the pingSequencer << NO WAIT IT DOES NOT DOES IT?!?!?
      sensorSelect = 1;
    } else {
      sensorSelect = 0;
    }
  }



  // NOTE: DO TOMORROW INSTEAD >>> IF THEY ARE EQUAL, START TIMER> IF TIMER UP CHANGE MODE> MUCH BETTER!!!?!?!

}
