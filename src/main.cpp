/*
   RadioLib SX126x Ping-Pong Example

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx126x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/

   Modified by: @amontero at Telegram
                ajmcalvo at GitHub
*/
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

// bool recievedFlag = false;
// void recieveFlag(void);

void setup()
{
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