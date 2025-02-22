#ifdef TX_DEVICE
#include "../src/header/Tx.h"

/*
    Configurations for deepsleep
*/
#define DEEPSLEEP_MINIMUM_TIME 10000 // 10 seconds

#if CONFIG_IDF_TARGET_ESP32
#define THRESHOLD 40   // Bigger the value more sensitivity
#else                  // ESP32-S2 and ESP32-S3 or other defalt chips
#define THRESHOLD 4000 // Lower the value more sensitivity
#endif

touch_pad_t touchPin = TOUCH_PAD_NUM7;

/*
    Initialize static variables
*/
std::vector<std::pair<unsigned long, std::size_t>> TxRadio::pulseCounter;
unsigned long TxRadio::lastInterruptTime = 0;
volatile bool TxRadio::pulseFlag = false;

TxRadio::TxRadio() {}

/*
    @brief Initialize the radio and set up the interrupt
*/
void TxRadio::setupRadio()
{
    // touchSleepWakeUpEnable(T7, THRESHOLD);

    int state = this->setRadioConfig();

    log_d("[SX1262] Sending first packet ...");
    int16_t transmissionState = radioLoRa.startTransmit("TimeSync::" + String(millis()) + "::" + String(getEspAdress()));
    this->StatusReport(transmissionState, "First Transmission");

    // Attach interrupt to GPIO pin
    pinMode(GPIO_SENSOR_PIN, INPUT_PULLUP); // Pull-up resistor
    attachInterrupt(digitalPinToInterrupt(GPIO_SENSOR_PIN), pulseISR, RISING);

    // If coming back from sleep add a pulse to the counter
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)
    {
        log_d("returning from deepsleep");
        incrementPulseCounter(millis());
    }
}

/*
    @brief Handle radio functions as sending packages and incrementing pulse counter
*/
void TxRadio::handleRadio()
{
    unsigned long currentTime = millis();
    deepSleepEnable(currentTime);
    if (pulseFlag)
    {
        noInterrupts();
        pulseFlag = false; // Reset the flag
        incrementPulseCounter(currentTime);
        interrupts();
    }

    // Check for transmition interval and data
    if (currentTime - lastTxTime >= COUNTER_RESET_INTERVAL && pulseCounter.size() > 0)
    {
        this->sendPackage();
    }
}

/*
    @brief Increments the pulse counter
    @param currentTime Current time in milliseconds
*/
const void TxRadio::incrementPulseCounter(const unsigned long &currentTime)
{
    if (currentTime - this->lastInterruptTime >= TIMESTAMP_PACKAGE || pulseCounter.empty()) // Assemble packages in TIMESTAMP_PACKAGE
    {
        pulseCounter.push_back(std::make_pair(currentTime, 1)); // Increment the pulse counter
        this->lastInterruptTime = currentTime;                  // Update the last pulse time
        log_d("Incrementing");
    }
        else
        {
            // TODO: arrumar o debounce
            // deBounce();
            pulseCounter.back().second++;
            log_d("\t pulseCount: %d", pulseCounter.back().second);
        }
}

/*
    @brief Sends the assembled package
*/
void TxRadio::sendPackage(void)
{
    int16_t transmissionState = this->radioLoRa.startTransmit(assembleMessagePayload().c_str());
    StatusReport(transmissionState, "Send Package");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    this->pulseCounter.clear(); // Reset the count
    this->lastTxTime = millis();
}

/*
    @brief Assembles the message payload
    @returns payload as a string
*/
String TxRadio::assembleMessagePayload(void)
{
    String dataString{""};

    for (std::pair<unsigned long, std::size_t> &element : this->pulseCounter)
    {
        dataString += String(element.first) + "/" + String(element.second) + "//";
    }

    String payload = "Data:" + dataString + "::" + "ID:" + String(getEspAdress()) + "::" + "Time:" + String(millis());
    log_d("Assembled payload:\n\t %s", payload.c_str());

    return payload;
}

/*
    @brief Confirms the if enough time has passed to enter deepsleeps
*/
void TxRadio::deepSleepEnable(unsigned long &currentTime)
{
    if (currentTime - this->lastInterruptTime > DEEPSLEEP_MINIMUM_TIME)
    {
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
        log_d("Enterring deepsleep");
        esp_deep_sleep_start();
    }
}

/*
    @brief Interrupt Service Routine for the pulse sensor
*/
void TxRadio::pulseISR()
{
    pulseFlag = true; // Set flag for the main loop to handle
}

TxRadio::~TxRadio() {}

#endif