/* ~~~~~~~~~~~~~ dataObject.h ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Data Filtering Algorithms

   Library to store various data filtering
   & sorting algorithms



*/




#ifndef dataObject_h
#define dataObject_h

#if (ARDUINO >=100)
#include <Arduino.h>
#else
#include <wProgram.h>
#endif

#define basicFilter recursiveFilter   // Synonyms for basic methods



class dataObject
{

  public:
    // Constructor

    dataObject(float filterBias = 0.9, bool serialMonitor = false):
      printSerial(serialMonitor),
      w(filterBias)
    {
    }

    void begin(uint32_t baudrate = 115200);    // Serial Comms

    int32_t recursiveFilter(int32_t Xn);




    // Constants




    // Variables



  private:

    int32_t Ypre;   //Y(n-1) Variable used for recursive filter

    float w;             // bias value for recursive filter

    bool printSerial;

};




#endif
