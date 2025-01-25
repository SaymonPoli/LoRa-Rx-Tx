#ifndef LORA_CONFIG_H
#define LORA_CONFIG_H

// Pause between transmitted packets in seconds.
// Set to zero to only transmit a packet when pressing the user button
// Will not exceed 1% duty cycle, even if you set a lower value.
#ifndef PAUSE
#define PAUSE 1000
#endif

// Frequency in MHz. Keep the decimal point to designate float.
#ifndef FREQUENCY
#define FREQUENCY 433.0 // for Brazil change to 915.0
#endif

// LoRa bandwidth. Keep the decimal point to designate float.
// Allowed values are 7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125.0, 250.0 and 500.0 kHz.
#ifndef BBANDWIDTH
#define BANDWIDTH 125.0
#endif

// Number from 5 to 12. Higher means slower but higher "processor gain",
// meaning (in nutshell) longer range and more robust against interference.
#ifndef SPREADING_FACTOR
#define SPREADING_FACTOR 7
#endif

#ifndef CODDING_RATE
#define CODING_RATE 5
#endif

// Transmit power in dBm. 0 dBm = 1 mW, enough for tabletop-testing. This value can be
// set anywhere between -9 dBm (0.125 mW) to 22 dBm (158 mW). Note that the maximum ERP
// (which is what your antenna maximally radiates) on the EU ISM band is 25 mW, and that
// transmissting without an antenna can damage your hardware.
#define TRANSMIT_POWER 0

#define GPIO_SENSOR_PIN 7           // Input pin to sensor reading
#define COUNTER_RESET_INTERVAL 5000 // Transmit information
#define TIMESTAMP_PACKAGE 10000     // Group Packages
#define DEBOUNCE_DELAY 500          // Button debounce

#endif
