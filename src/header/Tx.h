#ifdef TX_DEVICE
#include <../src/header/Radio.h>

class TxRadio : public Radio
{
private:
    unsigned long pulseCountSnapShot{0}, currentTime{0};
    static volatile unsigned long pulseCounter;      // Sensor counter
    static volatile unsigned long lastInterruptTime; // Last time the GPIO pin was activated
    static volatile bool pulseFlag;                  // Flag for pulse detection

    long counter = 0;
    uint64_t lastTxTime{0};
    bool transmitFlag = false;

    static void pulseISR();

public:
    TxRadio();
    virtual void setupRadio();
    virtual void handleRadio();
    ~TxRadio();
};

volatile unsigned long TxRadio::pulseCounter = 0;      // Initialize static variable
volatile unsigned long TxRadio::lastInterruptTime = 0; // Initialize static variable
volatile bool TxRadio::pulseFlag = false;              // Initialize static variable

TxRadio::TxRadio() {}

void TxRadio::setupRadio()
{
    pinMode(BUILTIN_LED, OUTPUT);
    int state = radioLoRa.begin();
    if (state == RADIOLIB_ERR_NONE)
    {
        log_d("\t\tRadio Setup Tx: SUCCESS!");
    }
    else
    {
        log_e("\t\tInitialization failed, code %d", state);
        while (true)
            ;
    }

    log_d("[SX1262] Sending first packet ...");
    int transmissionState = radioLoRa.startTransmit("Hello World!");
    if (transmissionState == RADIOLIB_ERR_NONE)
    {
        log_d("\t\tTransmission success!");
    }
    else
    {
        log_e("\t\tTransmission failed, code %d", transmissionState);
        while (true)
            ;
    }

    // Attach interrupt to GPIO pin
    pinMode(GPIO_SENSOR_PIN, INPUT_PULLUP); // Pull-up resistor
    attachInterrupt(digitalPinToInterrupt(GPIO_SENSOR_PIN), pulseISR, FALLING);
}

TxRadio::~TxRadio() {}

void TxRadio::pulseISR()
{
    pulseFlag = true; // Set flag for the main loop to handle
}

void TxRadio::handleRadio()
{
    unsigned long currentTime = millis();
    if (pulseFlag)
    {
        pulseFlag = false; // Reset the flag

        // Protect shared variables from race conditions
        noInterrupts();
        if (currentTime - lastInterruptTime >= DEBOUNCE_DELAY)
        {
            pulseCounter++;                  // Increment the pulse counter
            lastInterruptTime = currentTime; // Update the last pulse time
        }
        interrupts();
    }

    // Check for transmition interval and data
    if (currentTime - lastTxTime >= COUNTER_RESET_INTERVAL && pulseCounter > 0)
    {

        String payload = String(pulseCounter); // Create a dynamic payload
        int transmissionState = radioLoRa.startTransmit(payload.c_str());

        if (transmissionState == RADIOLIB_ERR_NONE)
        {
            log_d("Transmission success: %s", payload.c_str());
            digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED)); // Toggle LED
        }
        else
        {
            log_e("Transmission failed: %d", transmissionState);
        }

        pulseCounter = 0; // Reset the counter
        lastTxTime = currentTime;
        counter++;
    }
}

#endif
