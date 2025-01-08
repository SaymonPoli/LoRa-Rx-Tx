#ifdef TX_DEVICE
#include <time.h>
#include <vector>
#include <../src/header/Radio.h>

struct data
{
    unsigned long timestamp;
};

class TxRadio : public Radio
{
private:
    unsigned long pulseCountSnapShot{0}, currentTime{0};
    static volatile bool pulseFlag;                  // Flag for pulse detection
    static unsigned long lastInterruptTime;          // Last time the GPIO pin was activated
    static std::vector<std::pair<unsigned long, std::size_t>> pulseCounter; // Sensor counter

    long counter = 0;
    uint64_t lastTxTime{0};
    bool transmitFlag = false;

    static void pulseISR();
    void sendPackage();
    String assembleMessagePayload(void);

public:
    TxRadio();
    uint64_t getEspAdress();
    virtual void setupRadio();
    virtual void handleRadio();
    ~TxRadio();
};

#endif
