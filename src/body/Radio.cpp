#include "../header/Radio.h"

Radio::Radio()
{
    radioLoRa.setFrequency(865.0F);
    radioLoRa.setSpreadingFactor(7U);
    radioLoRa.setCodingRate(5U);
    radioLoRa.setOutputPower(1);
    transmitionState = RADIOLIB_ERR_NONE;
}

void Radio::ErrorReport(const int &state, const String &action)
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

void Radio::setSyncWord(void)
{
    // setting radio sync word based on logic state of pins 2, 3, 4 and 5
    for (size_t i = 0;
         i < 4; i++)
    {
        pinMode(radioSyncWordPins[i], INPUT_PULLUP);
        log_d("Setting sync word pin: %d", radioSyncWordPins[i]);
        this->syncWord |= (digitalRead(radioSyncWordPins[i]) << (3 - i));
    }
    log_d("Sync Word: 0x%02X (Binary: %s)", syncWord, toBinary(syncWord));

    if (radioLoRa.setSyncWord(syncWord) != RADIOLIB_ERR_NONE)
    {
        log_e("Unable to setSyncWord!");
        while (true)
            delay(10);
    }
}
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

Radio::~Radio()
{
}
