#ifndef RADIO_H
#define RADIO_H

#include <../src/header/LoraConfig.h>
#include <RadioLib.h>

// uncomment the following only on one
// of the nodes to initiate the pings
#define INITIATING_NODE

// Defaults for heltec_wifi_lora_32_v3
// NSS pin:        8
// DIO1 pin:      14
// NRST pin:      12
// BUSY pin:      13
// LORA MOSI pin: 10
// LORA MISO pin: 11
// LORA SCK pin:   9

// GPIO 45 -> 48 defines sync word

#define LORA_NSS 8
#define LORA_DIO1 14
#define LORA_NRST 12
#define LORA_BUSY 13

class Radio
{
protected:
    int transmitionState;
    bool transmitFlag;
    SX1262 radioLoRa = new Module(LORA_NSS, LORA_DIO1, LORA_NRST, LORA_BUSY);

    void setFlag(void);
    virtual void setupRadio() = 0;
    virtual void handleRadio() = 0;

public:
    Radio();
    ~Radio();
};

Radio::Radio()
{

    radioLoRa.setFrequency(865.0F);
    radioLoRa.setSpreadingFactor(7U);
    radioLoRa.setCodingRate(5U);
    radioLoRa.setOutputPower(1);
    transmitionState = RADIOLIB_ERR_NONE;
}

Radio::~Radio()
{
}

#endif