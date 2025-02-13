diff --git a/.gitignore b/.gitignore
index 60dfd95..dd2f410 100644
--- a/.gitignore
+++ b/.gitignore
@@ -1,4 +1,3 @@
 .pio
 .vscode
-/serialReader
 /src/header/MqttConfig.h
\ No newline at end of file
diff --git a/.vscode/c_cpp_properties.json b/.vscode/c_cpp_properties.json
index e9d97f6..035aeb3 100644
--- a/.vscode/c_cpp_properties.json
+++ b/.vscode/c_cpp_properties.json
@@ -11,9 +11,8 @@
                 "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/include",
                 "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/src",
                 "/home/saymon/.platformio/packages/framework-arduinoespressif32/libraries/WiFi/src",
-                "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/libdeps/Rx/RadioLib/src",
+                "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/libdeps/Tx/RadioLib/src",
                 "/home/saymon/.platformio/packages/framework-arduinoespressif32/libraries/SPI/src",
-                "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/libdeps/Rx/PubSubClient/src",
                 "/home/saymon/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32s3/include/newlib/platform_include",
                 "/home/saymon/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32s3/include/freertos/include",
                 "/home/saymon/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32s3/include/freertos/include/esp_additions/freertos",
@@ -213,7 +212,6 @@
                 "/home/saymon/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32s3/qio_qspi/include",
                 "/home/saymon/.platformio/packages/framework-arduinoespressif32/cores/esp32",
                 "/home/saymon/.platformio/packages/framework-arduinoespressif32/variants/heltec_wifi_lora_32_V3",
-                "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/libdeps/Rx/Unity/src",
                 "/home/saymon/.platformio/packages/framework-arduinoespressif32/libraries/ArduinoOTA/src",
                 "/home/saymon/.platformio/packages/framework-arduinoespressif32/libraries/AsyncUDP/src",
                 "/home/saymon/.platformio/packages/framework-arduinoespressif32/libraries/BLE/src",
@@ -253,9 +251,8 @@
                     "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/include",
                     "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/src",
                     "/home/saymon/.platformio/packages/framework-arduinoespressif32/libraries/WiFi/src",
-                    "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/libdeps/Rx/RadioLib/src",
+                    "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/libdeps/Tx/RadioLib/src",
                     "/home/saymon/.platformio/packages/framework-arduinoespressif32/libraries/SPI/src",
-                    "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/libdeps/Rx/PubSubClient/src",
                     "/home/saymon/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32s3/include/newlib/platform_include",
                     "/home/saymon/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32s3/include/freertos/include",
                     "/home/saymon/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32s3/include/freertos/include/esp_additions/freertos",
@@ -455,7 +452,6 @@
                     "/home/saymon/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32s3/qio_qspi/include",
                     "/home/saymon/.platformio/packages/framework-arduinoespressif32/cores/esp32",
                     "/home/saymon/.platformio/packages/framework-arduinoespressif32/variants/heltec_wifi_lora_32_V3",
-                    "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/libdeps/Rx/Unity/src",
                     "/home/saymon/.platformio/packages/framework-arduinoespressif32/libraries/ArduinoOTA/src",
                     "/home/saymon/.platformio/packages/framework-arduinoespressif32/libraries/AsyncUDP/src",
                     "/home/saymon/.platformio/packages/framework-arduinoespressif32/libraries/BLE/src",
@@ -497,7 +493,7 @@
                 "ARDUINO_RUNNING_CORE=1",
                 "ARDUINO_EVENT_RUNNING_CORE=1",
                 "CORE_DEBUG_LEVEL=5",
-                "RX_DEVICE",
+                "TX_DEVICE",
                 "HAVE_CONFIG_H",
                 "MBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\"",
                 "UNITY_INCLUDE_CONFIG_H",
diff --git a/.vscode/launch.json b/.vscode/launch.json
index 58fed1a..7a6ea38 100644
--- a/.vscode/launch.json
+++ b/.vscode/launch.json
@@ -12,21 +12,21 @@
             "type": "platformio-debug",
             "request": "launch",
             "name": "PIO Debug",
-            "executable": "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/build/Rx/firmware.elf",
-            "projectEnvName": "Rx",
+            "executable": "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/build/Tx/firmware.elf",
+            "projectEnvName": "Tx",
             "toolchainBinDir": "/home/saymon/.platformio/packages/toolchain-xtensa-esp32s3/bin",
             "internalConsoleOptions": "openOnSessionStart",
             "preLaunchTask": {
                 "type": "PlatformIO",
-                "task": "Pre-Debug (Rx)"
+                "task": "Pre-Debug (Tx)"
             }
         },
         {
             "type": "platformio-debug",
             "request": "launch",
             "name": "PIO Debug (skip Pre-Debug)",
-            "executable": "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/build/Rx/firmware.elf",
-            "projectEnvName": "Rx",
+            "executable": "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/build/Tx/firmware.elf",
+            "projectEnvName": "Tx",
             "toolchainBinDir": "/home/saymon/.platformio/packages/toolchain-xtensa-esp32s3/bin",
             "internalConsoleOptions": "openOnSessionStart"
         },
@@ -34,8 +34,8 @@
             "type": "platformio-debug",
             "request": "launch",
             "name": "PIO Debug (without uploading)",
-            "executable": "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/build/Rx/firmware.elf",
-            "projectEnvName": "Rx",
+            "executable": "/home/saymon/Documents/Sycrom/LoRa-Rx-Tx/.pio/build/Tx/firmware.elf",
+            "projectEnvName": "Tx",
             "toolchainBinDir": "/home/saymon/.platformio/packages/toolchain-xtensa-esp32s3/bin",
             "internalConsoleOptions": "openOnSessionStart",
             "loadMode": "manual"
diff --git a/src/body/Tx.cpp b/src/body/Tx.cpp
index 931c907..47b67ab 100644
--- a/src/body/Tx.cpp
+++ b/src/body/Tx.cpp
@@ -42,7 +42,10 @@ void TxRadio::setupRadio()
 
     // If coming back from sleep add a pulse to the counter
     if (sleepFlag)
+    {
+        log_d("returning from deepsleep");
         pulseFlag = true;
+    }
 }
 
 /*
@@ -82,6 +85,7 @@ void TxRadio::incrementPulseCounter(unsigned long &currentTime)
         }
         else
         {
+            deBounce();
             pulseCounter.back().second++;
             log_d("\t pulseCount: %d", pulseCounter.back().second);
         }
diff --git a/src/header/Tx.h b/src/header/Tx.h
index f0abe7b..894ee03 100644
--- a/src/header/Tx.h
+++ b/src/header/Tx.h
@@ -17,7 +17,6 @@ private:
     static std::vector<std::pair<unsigned long, std::size_t>> pulseCounter; // Sensor counter
     long counter = 0;
     uint64_t lastTxTime{0};
-    bool transmitFlag = false;
     bool sleepFlag = false;
 
     /*
diff --git a/src/main.cpp b/src/main.cpp
index 94fba2e..821661c 100644
--- a/src/main.cpp
+++ b/src/main.cpp
@@ -22,10 +22,15 @@ RxRadio *radio = new RxRadio;
 #endif
 
 void setup() {
+  Serial.begin(115200);
+  while (!Serial)
+  {
+    delay(100);
+  } // wait for serial to be available (if the device is Rx
+
   // setCpuFrequencyMhz(40);
   pinMode(LED_BUILTIN, OUTPUT);
 
-  Serial.begin(115200);
   radio->setupRadio();
 }
 
