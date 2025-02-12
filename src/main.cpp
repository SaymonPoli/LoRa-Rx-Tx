#include <Arduino.h>
ICACHE_RAM_ATTR

#if defined(ESP32)
ICACHE_RAM_ATTR
#endif

/*
  Import if the device is Tx
*/
#ifdef TX_DEVICE
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
  Serial.begin(115200);
  while (!Serial)
  {
    delay(100);
  } // wait for serial to be available

  // setCpuFrequencyMhz(40);
  pinMode(LED_BUILTIN, OUTPUT);

  radio->setupRadio();
}

void loop() { radio->handleRadio(); }
