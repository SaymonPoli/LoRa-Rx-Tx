#include <string>
#include "config.h"

void onGPIOInterrupt();

volatile unsigned long pulseCounter = 0; // Sensor counter
unsigned long lastInterruptTime = 0;     // Last time the GPIO pin was activated
unsigned long lastTxTime = 0;            // Last time data was sent via LoRa

String rxdata;
volatile bool rxFlag = false;
long counter = 0;
uint64_t last_tx = 0;
uint64_t tx_time;
uint64_t minimum_pause;

void rx();
void pulseISR();

void setup()
{
    heltec_setup();

    RADIOLIB_OR_HALT(radio.begin());
    radio.setDio1Action(rx);

    both.printf("Frequency: %.2f MHz\n", FREQUENCY);
    RADIOLIB_OR_HALT(radio.setFrequency(FREQUENCY));
    both.printf("Bandwidth: %.1f kHz\n", BANDWIDTH);
    RADIOLIB_OR_HALT(radio.setBandwidth(BANDWIDTH));
    both.printf("TX power: %i dBm\n", TRANSMIT_POWER);
    RADIOLIB_OR_HALT(radio.setOutputPower(TRANSMIT_POWER));

#ifdef TX_DEVICE
    // Configure for transmission
    pinMode(GPIO_INPUT_PIN, INPUT_PULLUP); // Pull-up resistor

    // Attach interrupt to GPIO pin
    attachInterrupt(digitalPinToInterrupt(GPIO_INPUT_PIN), pulseISR, FALLING); // Adjust based on your requirements

    both.println("Configuring for TX");
    delay(200);
#endif

#ifdef RX_DEVICE
    // Configure for reception
    both.println("Configuring for RX");
    RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));
    // delay(200);
#endif
}

void loop()
{
    heltec_loop();

#ifdef TX_DEVICE
    unsigned long currentTime = millis();

    unsigned long pulseCountSnapShot;

    noInterrupts();
    pulseCountSnapShot = pulseCounter;
    interrupts();

    // Check if 5 seconds have passed since the last transmission
    if (currentTime - lastTxTime >= COUNTER_RESET_INTERVAL)
    {
        // Transmit the counter value
        both.printf("TX [%d] Counter: [%d] \n", counter, pulseCounter);
        radio.clearDio1Action();
        heltec_led(50); // Turn on the LED to indicate transmission
        tx_time = millis();
        RADIOLIB(radio.transmit(String(pulseCounter).c_str())); // Transmit the pulse counter
        tx_time = millis() - tx_time;
        heltec_led(0);

        if (_radiolib_status != RADIOLIB_ERR_NONE)
        {
            both.printf("fail (%i)\n", _radiolib_status);
        }

        pulseCounter = 0; // reset the counter

        lastTxTime = currentTime;
        counter++;
    }
#endif

#ifdef RX_DEVICE
    // If a packet was received, display it and the RSSI and SNR
    if (rxFlag)
    {
        rxFlag = false;
        radio.readData(rxdata);
        if (_radiolib_status == RADIOLIB_ERR_NONE)
        {
            both.printf("RX [%s]\n", rxdata.c_str());
            both.printf("  RSSI: %.2f dBm\n", radio.getRSSI());
            both.printf("  SNR: %.2f dB\n", radio.getSNR());
        }
        RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));
    }
#endif
}

// Can't do Serial or display things here, takes too much time for the interrupt
void rx()
{
    rxFlag = true;
}

void pulseISR()
{
    unsigned long currentTime = millis(); // Get current time in microseconds
    // Debouncing: Ignore pulses that occur within the debounce delay
    if (currentTime - lastInterruptTime >= DEBOUNCE_DELAY)
    {
        pulseCounter++;                  // Increment the pulse counter
        lastInterruptTime = currentTime; // Update the last pulse time
    }
}