#ifdef TX_DEVICE
#include "../src/header/Tx.h"

/*
  Configuration for deepsleep
*/
#define DEEPSLEEP_MINIMUM_TIME 10000 // 10 seconds

#if CONFIG_IDF_TARGET_ESP32
#define THRESHOLD 40   // Bigger the value more sensitivity
#else                  // ESP32-S2 and ESP32-S3 or other defalt chips
#define THRESHOLD 5000 // Lower the value more sensitivity
#endif

touch_pad_t touchPin = TOUCH_PAD_NUM7;

std::vector<std::pair<unsigned long, std::size_t>> TxRadio::pulseCounter; // Initialize static variable
unsigned long TxRadio::lastInterruptTime = 0;                             // Initialize static variable
volatile bool TxRadio::pulseFlag = false;                                 // Initialize static variable

TxRadio::TxRadio() {}

void TxRadio::setupRadio()
{
    touchSleepWakeUpEnable(T7, THRESHOLD);

    int state = this->setRadioConfig();

    log_d("[SX1262] Sending first packet ...");
    int transmissionState = radioLoRa.startTransmit("TimeSync::" + String(millis()) + "::" + String(getEspAdress()));
    this->StatusReport(transmissionState, "First Transmission");

    // Attach interrupt to GPIO pin
    pinMode(GPIO_SENSOR_PIN, INPUT_PULLUP); // Pull-up resistor
    attachInterrupt(digitalPinToInterrupt(GPIO_SENSOR_PIN), pulseISR, RISING);
}

TxRadio::~TxRadio() {}

void TxRadio::pulseISR()
{
    pulseFlag = true; // Set flag for the main loop to handle
}

void TxRadio::handleRadio()
{
    unsigned long currentTime = millis();
    deepSleepEnable(currentTime, lastInterruptTime);
    if (pulseFlag)
    {
        noInterrupts();
        pulseFlag = false; // Reset the flag

        // Protect shared variables from race conditions
        if (currentTime - lastInterruptTime >= DEBOUNCE_DELAY)
        {
            if (currentTime - lastInterruptTime >= TIMESTAMP_PACKAGE || pulseCounter.empty()) // Assemble packages in TIMESTAMP_PACKAGE
            {
                pulseCounter.push_back(std::make_pair(currentTime, 1)); // Increment the pulse counter
                lastInterruptTime = currentTime;                        // Update the last pulse time
            }
            else
            {
                pulseCounter.back().second++;
                log_d("\t pulseCount: %d", pulseCounter.back().second);
            }
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
    StatusReport(transmissionState, "Send Package");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    this->pulseCounter.clear(); // Reset the count
    this->lastTxTime = millis();
}

String TxRadio::assembleMessagePayload(void)
{
    String dataString{""};

    for (std::pair<unsigned long, std::size_t> &element : this->pulseCounter)
    {
        dataString += String(element.first) + "/" + String(element.second) + "//";
    }

    String payload = "Data:" + dataString + "::" + "ID:" + String(getEspAdress()) + "::" + "Time:" + String(millis());
    log_d("Assembled payload: %s", payload.c_str());

    return payload;
}

void TxRadio::deBounce()
{
    unsigned long now = millis();
    do
    {
        // on bounce, reset time-out
        if (digitalRead(GPIO_SENSOR_PIN) == LOW)
            now = millis();
    } while (digitalRead(GPIO_SENSOR_PIN) == LOW ||
             (millis() - now) <= DEBOUNCE_DELAY);
}

void TxRadio::deepSleepEnable(unsigned long &currentTime, unsigned long &lastInterruptTime)
{
    if (currentTime - lastInterruptTime > DEEPSLEEP_MINIMUM_TIME)
    {
        log_d("Enterring deepsleep");
        esp_deep_sleep_start();
    }
}
#endif