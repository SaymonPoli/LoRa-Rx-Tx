#include <Arduino.h>
ICACHE_RAM_ATTR

#if defined(ESP32)
ICACHE_RAM_ATTR
#endif

/*
  Import Tx if the device is Tx
*/
#ifdef TX_DEVICE
/*
  Import Tx class and initialize the radio
*/
#include "../src/header/Tx.h"
TxRadio *radio = new TxRadio;
#endif

/*
  Import if the device is Rx
*/
#ifdef RX_DEVICE
#include "../src/header/Rx.h"
RxRadio *radio = new RxRadio;
#endif

void setup() {
  // setCpuFrequencyMhz(40);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  radio->setupRadio();
}

void loop() { radio->handleRadio(); }
