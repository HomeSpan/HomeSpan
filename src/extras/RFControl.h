
////////////////////////////////////
//       RF Control Module        //
////////////////////////////////////

#include "driver/rmt.h"
#include <vector>

using std::vector;

class RFControl {
  private:
    rmt_config_t *config=NULL;
    vector<uint32_t> data;
    boolean lowWord=true;
    boolean refClock;
    static uint8_t nChannels;
                               
  public:    
    RFControl(uint8_t pin, boolean refClock=true);                                    // creates transmitter on pin, using 1-MHz Ref Tick clock
    
    void start(uint32_t *data, int nData, uint8_t nCycles=1, uint8_t tickTime=1);     // starts transmission of pulses from specified data pointer, repeated for numCycles, where each tick in pulse is tickTime microseconds long
    void start(uint8_t nCycles=1, uint8_t tickTime=1);                                // starts transmission of pulses from internal data structure, repeated for numCycles, where each tick in pulse is tickTime microseconds long    
    
    void clear();                                         // clears transmitter memory
    void add(uint32_t onTime, uint32_t offTime);          // adds pulse of onTime ticks HIGH followed by offTime ticks LOW
    void phase(uint32_t nTicks, uint8_t phase);           // adds either a HIGH phase or LOW phase lasting numTicks ticks
    void enableCarrier(uint32_t freq, float duty=0.5);    // enables carrier wave if freq>0, else disables carrier wave; duty is a fraction from 0-1
    void disableCarrier(){enableCarrier(0);}              // disables carrier wave
};

// Helper macro for creating your own storage of uint32_t data array elements - used with first variation of start() above

#define RF_PULSE(highTicks,lowTicks) (1 << 15 | highTicks | lowTicks << 16)
   
