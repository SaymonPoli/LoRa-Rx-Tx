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
    setCpuFrequencyMhz(40);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    radio->setupRadio();
}

void loop()
{
    radio->handleRadio();
}
