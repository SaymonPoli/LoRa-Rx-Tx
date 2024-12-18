#include <Arduino.h>
ICACHE_RAM_ATTR
#include "../src/header/Tx.h"
#include "../src/header/Rx.h"

#ifdef TX_DEVICE
TxRadio *radio = new TxRadio;
#endif

#ifdef RX_DEVICE
RxRadio *radio = new RxRadio;
#endif

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    radio->setupRadio();
}

void loop()
{
    radio->handleRadio();

    // #ifdef RX_DEVICE
    //     if (recievedFlag)
    //     {
    //         String str;
    //         int state = this->radioLoRa.readData(str);

    //         log_d("Recieved new message.");

    //         if (state == RADIOLIB_ERR_NONE)
    //         {
    //             log_d("Recieved message: %s", str);
    //         }
    //         else if (state == RADIOLIB_ERR_CRC_MISMATCH)
    //         {
    //             log_d("crc error!");
    //         }
    //         else
    //         {
    //             log_d("failed, code: %d", state);
    //         }
    //     }
    // #endif
}

// void recieveFlag()
// {
//     recievedFlag = true;
// }