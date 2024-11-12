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
    static std::vector<unsigned long> pulseCounter;  // Sensor counter

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

std::vector<unsigned long> TxRadio::pulseCounter;      // Initialize static variable
unsigned long TxRadio::lastInterruptTime = 0;          // Initialize static variable
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
    int transmissionState = radioLoRa.startTransmit("TimeSync::" + String(millis()) + "::" + String(getEspAdress()));
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
            pulseCounter.push_back(currentTime); // Increment the pulse counter
            lastInterruptTime = currentTime; // Update the last pulse time
        }
        interrupts();
    }

    // Check for transmition interval and data
    if (currentTime - lastTxTime >= COUNTER_RESET_INTERVAL && pulseCounter.size() > 0)
    {
        this->sendPackage();
    }
}

void TxRadio::sendPackage(void)
{
    int transmissionState = radioLoRa.startTransmit(assembleMessagePayload().c_str());

    if (transmissionState == RADIOLIB_ERR_NONE)
    {
        log_d("Transmission success");
        digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED)); // Toggle LED
    }
    else if (transmissionState == RADIOLIB_ERR_PACKET_TOO_LONG)
    {
        log_e("Packet, too long");
    }
    else
    {
        log_e("Transmission failed: %d", transmissionState);
    }

    this->pulseCounter.clear(); // Reset the count
    this->lastTxTime = millis();
    this->counter++;
}
String TxRadio::assembleMessagePayload(void)
{
    String dataString{""};

    for (unsigned long &element : this->pulseCounter)
    {
        log_d("Element from pulseCounter: %d", element);
        dataString += String(element) + "/";
    }

    String payload = "Data:" + dataString + "::" + "ID:" + String(getEspAdress()) + "::" + "Time:" + String(millis());
    log_d("Assembled payload: %s", payload.c_str()); // Check the payload

    return payload;
}
uint64_t TxRadio::getEspAdress(void)
{
    uint32_t low = ESP.getEfuseMac() & 0xFFFFFFFF;
    uint32_t high = (ESP.getEfuseMac() >> 32) % 0xFFFFFFFF;

    log_d("\tESP32 LOW esfuse mac: %d", low);
    log_d("\tESP32 HIGH efuse mac: %d\n", high);

    return word(low, high);
}

#endif
