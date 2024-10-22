#ifdef TX_DEVICE
#include <../src/header/Radio.h>

class TxRadio : public Radio
{
private:
    unsigned long pulseCountSnapShot, currentTime;

    volatile unsigned long pulseCounter = 0; // Sensor counter
    unsigned long lastInterruptTime = 0;     // Last time the GPIO pin was activated
    unsigned long lastTxTime = 0;            // Last time data was sent via LoRa

    long counter = 0;
    uint64_t last_tx = 0;
    uint64_t tx_time;
    uint64_t minimum_pause;

    void pulseISR();

public:
    TxRadio(/* args */);
    virtual void setupRadio();
    virtual void handleRadio();

    ~TxRadio();
};

TxRadio::TxRadio(/* args */)
{
}

void TxRadio::setupRadio()
{

    pinMode(BUILTIN_LED, OUTPUT);
    int state = radioLoRa.begin();
    if (state == RADIOLIB_ERR_NONE)
    {
        log_d("\t\tRadio Setup Tx: SUCESS!");
    }
    else
    {
        log_d("\t\tInitialization failed, code %d", state);
        while (true)
            ;
    }

    log_d("[SX1262] Sending first packet ...");
    transmitionState = radioLoRa.startTransmit("Hello World!");
    if (state == RADIOLIB_ERR_NONE)
    {
        log_d("\t\tReception success!");
    }
    else
    {
        log_d("\t\tReception failed, code %d", state);
        while (true)
            ;
    }
}

TxRadio::~TxRadio()
{
}

void pulseISR();

// save transmission states between loops
int transmissionState = RADIOLIB_ERR_NONE;
void intializeTxDevice(SX1262 &radio)
{
    // Configure for transmission
    transmissionState = radio.startTransmit("Hello, World!");
    log_d("\t\t [SX1262] Starting to recieve");

    // Attach interrupt to GPIO pin
    pinMode(GPIO_INPUT_PIN, INPUT_PULLUP);                                     // Pull-up resistor
    attachInterrupt(digitalPinToInterrupt(GPIO_INPUT_PIN), pulseISR, FALLING); // Adjust based on your requirements

    delay(200);
}

void TxRadio::pulseISR()
{
    unsigned long currentTime = millis(); // Get current time in microseconds
    // Debouncing: Ignore pulses that occur within the debounce delay
    if (currentTime - lastInterruptTime >= DEBOUNCE_DELAY)
    {
        pulseCounter++;                  // Increment the pulse counter
        lastInterruptTime = currentTime; // Update the last pulse time
    }
}

void TxRadio::handleRadio()
{
    unsigned long currentTime = millis();

    unsigned long pulseCountSnapShot;

    noInterrupts();
    pulseCountSnapShot = pulseCounter;
    interrupts();

    // Check if 5 seconds have passed since the last transmission
    if (transmitFlag)
    {
        String str;
        transmissionState = radioLoRa.startTransmit("Hello world!: %d", pulseCountSnapShot);

        if (transmissionState == RADIOLIB_ERR_NONE)
        {
            log_d("Transmition sucess: %d", pulseCountSnapShot);
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        }
        else
        {
            log_d("Transmition failed: %d", transmissionState);
        }

        transmitFlag = false;
        pulseCounter = 0; // reset the counter

        lastTxTime = currentTime;
        counter++;
    }

    if (currentTime - lastTxTime >= COUNTER_RESET_INTERVAL)
    {
        transmitFlag = true;
    }
}

#endif