
////////////////////////////////////
//       RF Control Module        //
////////////////////////////////////

#define RF433_PIN   22            // pin used for 433MHz transmitter
#define RF315_PIN   23            // pin used for 315MHz transmitter

class RFControl {
  private:
    int pin;
    static volatile int numCycles;
    static boolean configured;
    static uint32_t *pRMT;
    static int pCount;
    static void eot_int(void *arg);                         

  public:  
    RFControl(int pin);                                     // creates transmitter on pin
    static void clear();                                    // clears transmitter memory
    static void add(uint16_t onTime, uint16_t offTime);     // adds pulse of onTime ticks HIGH followed by offTime ticks LOW
    void start(int _numCycles, int tickTime=1);             // starts transmission of pulses, repeated for numCycles, where each tick in pulse is tickTime microseconds long
};

// Two transmitters are defined

extern RFControl RF433;                                   
extern RFControl RF315;
    
