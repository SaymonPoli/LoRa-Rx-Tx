#ifdef TX_DEVICE
#include "../src/header/Tx.h"

std::vector<std::pair<unsigned long, std::size_t>> TxRadio::pulseCounter; // Initialize static variable
unsigned long TxRadio::lastInterruptTime = 0;                             // Initialize static variable
volatile bool TxRadio::pulseFlag = false;                                 // Initialize static variable

TxRadio::TxRadio() {}

void TxRadio::setupRadio()
{
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
    if (pulseFlag)
    {
        pulseFlag = false; // Reset the flag
        noInterrupts();

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

void deBounce()
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

#endif