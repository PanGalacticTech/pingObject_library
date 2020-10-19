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

#define SENSOR_SAMPLE_DELAY_MS 100

#include "dataObject.h"

#define FILTER_BIAS 0.5   // 0 to 1: Higher numbers = faster response less filtering // Lower numbers = Slower response, more filtering
#define FILTER_SERIAL_OUT_TRUE 1
#define FILTER_SERIAL_OUT_FALSE 0

dataObject dataLib(FILTER_BIAS, FILTER_SERIAL_OUT_FALSE);


#include "pingObject.h"

#define TRIGGER_OUTPUT_PIN 2
#define ECHO_RECEIVE_PIN 8
#define PING_SERIAL_OUTPUT_TRUE 1

pingObject pingLeft(TRIGGER_OUTPUT_PIN, ECHO_RECEIVE_PIN , PING_SERIAL_OUTPUT_TRUE);









void setup() {
pingLeft.begin();     // Sets input & output pins & starts serial communications (default 115200) 

}











void loop() {




  
  if (pingSequencer == 0)   {                                         // State 0: Waiting to send ping
pingLeft.triggerPing(SENSOR_SAMPLE_DELAY_MS);
  }

  // States 1 to 3: - Send Output Ping
  sendPing();                                   // Send ping runs every loop, activates timed sequence when is only active when pingSequencer goes to 1.

  // States 4 to 6: - Wait for echo and measure the length of the return pulse
  timeEcho();

  // State 7: - When ping has returned calculate the distance & print value
  if (pingSequencer == 7) {                                     // When pingSequencer reaches 7, ping has been sent and recieved
   
    cm = microsecondsToCentimeters(pulseDuration);         // convert the time into a distance
     

    
    pingLeft.printDistance_cm(cm);                                    // Print the distance to serial monitor
   pingLeft.printFiltered_data(dataLib.recursiveFilter(cm));
    pingSequencer = 0;                                           // distance has been calculated so reset pingSequencer ready for the next trigger
  }
}
