
////////////////////////////////////
//       RF Control Module        //
////////////////////////////////////

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
    static void phase(uint16_t numTicks, uint8_t phase);    // adds either a HIGH phase or LOW phase lasting numTicks ticks
    void start(uint8_t _numCycles, uint8_t tickTime=1);     // starts transmission of pulses, repeated for numCycles, where each tick in pulse is tickTime microseconds long
};

   
