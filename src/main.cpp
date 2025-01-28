#include <Arduino.h>
ICACHE_RAM_ATTR

#if defined(ESP32)
ICACHE_RAM_ATTR
#endif
#ifdef TX_DEVICE
#include "../src/header/Tx.h"
TxRadio *radio = new TxRadio;
#endif

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
