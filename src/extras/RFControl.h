
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
    static uint8_t nChannels;
                               
  public:    
    RFControl(uint8_t pin);                                                           // creates transmitter on pin   
    
    void start(uint32_t *data, int nData, uint8_t nCycles=1, uint8_t tickTime=1);     // starts transmission of pulses from specified data pointer, repeated for numCycles, where each tick in pulse is tickTime microseconds long
    void start(uint8_t nCycles=1, uint8_t tickTime=1);                                // starts transmission of pulses from internal data structure, repeated for numCycles, where each tick in pulse is tickTime microseconds long    
    
    void clear();                                         // clears transmitter memory
    void add(uint32_t onTime, uint32_t offTime);          // adds pulse of onTime ticks HIGH followed by offTime ticks LOW
    void phase(uint32_t nTicks, uint8_t phase);           // adds either a HIGH phase or LOW phase lasting numTicks ticks
};

// Helper macro for creating your own storage of uint32_t data array elements - used with first variation of start() above

#define RF_PULSE(highTicks,lowTicks) (1 << 15 | highTicks | lowTicks << 16)
   
