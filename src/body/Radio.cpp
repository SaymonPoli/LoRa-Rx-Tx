#include "../header/Radio.h"

Radio::Radio()
{
    // setting radio sync word based on logic state of pins 2, 3, 4 and 5
    for (size_t i = 0; i < 4; i++)
    {
        pinMode(radioSyncWordPins[i], INPUT_PULLUP);
        this->syncWord |= (digitalRead(radioSyncWordPins[i]) << (3 - i));
    }
    log_d("Sync Word: 0x%02X (Binary: %s)", this->syncWord, toBinary(syncWord));

    // this->radioLoRa.setFrequency(865.0F);
    // this->radioLoRa.setSpreadingFactor(7U);
    // this->radioLoRa.setCodingRate(5U);
    // this->radioLoRa.setOutputPower(1);
    transmitionState = RADIOLIB_ERR_NONE;
}

void Radio::StatusReport(const int &state, const String &action)
{
    if (state != RADIOLIB_ERR_NONE)
    {
        log_e("ERROR!, error code %d, actior: %s", state, action.c_str());
        while (true)
            delay(100);
    }
    else
    {
        log_d("%s, SUCCESS", action.c_str());
    }
}

int Radio::setRadioConfig(void)
{

    // int result = this->radioLoRa.begin(FREQUENCY, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, syncWord, TRANSMIT_POWER);
    int result = this->radioLoRa.begin();
    if (this->radioLoRa.setFrequency(FREQUENCY) == RADIOLIB_ERR_INVALID_FREQUENCY)
    {
        log_e("Selected frequency is invalid for this module!");
        while (true)
        {
            delay(10);
        }
    }

    // set bandwidth to 250 kHz
    if (this->radioLoRa.setBandwidth(250.0) == RADIOLIB_ERR_INVALID_BANDWIDTH)
    {
        log_e("Selected bandwidth is invalid for this module!");
        while (true)
        {
            delay(10);
        }
    }

    // set spreading factor to 10
    if (this->radioLoRa.setSpreadingFactor(10) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR)
    {
        log_e("Selected spreading factor is invalid for this module!");
        while (true)
        {
            delay(10);
        }
    }

    // set coding rate to 6
    if (this->radioLoRa.setCodingRate(7) == RADIOLIB_ERR_INVALID_CODING_RATE)
    {
        log_e("Selected coding rate is invalid for this module!");
        while (true)
        {
            delay(10);
        }
    }

    // set LoRa sync word to syncWord
    if (this->radioLoRa.setSyncWord(0xAB) != RADIOLIB_ERR_NONE) // TODO: make the syncWord hardware
    {
        log_e("Unable to set sync word!");
        while (true)
        {
            delay(10);
        }
    }

    // set output power to 10 dBm (accepted range is -17 - 22 dBm)
    if (this->radioLoRa.setOutputPower(2) == RADIOLIB_ERR_INVALID_OUTPUT_POWER)
    {
        log_e("Selected output power is invalid for this module!");
        while (true)
        {
            delay(10);
        }
    }

    StatusReport(result, "Radio Configuration");

    return result;
}

uint8_t Radio::getSyncWord(void) { return this->syncWord; }

const char *Radio::toBinary(byte b)
{
    static char binaryString[9]; // 8 bits + null terminator
    for (int i = 7; i >= 0; i--)
    {
        binaryString[7 - i] = (b & (1 << i)) ? '1' : '0';
    }
    binaryString[8] = '\0'; // Null-terminate the string
    return binaryString;
}

uint64_t Radio::getEspAdress(void)
{
    uint32_t low = ESP.getEfuseMac() & 0xFFFFFFFF;
    uint32_t high = (ESP.getEfuseMac() >> 32) % 0xFFFFFFFF;

    return word(low, high);
}

Radio::~Radio()
{
}
