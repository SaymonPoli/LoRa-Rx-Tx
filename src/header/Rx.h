#ifdef RX_DEVICE
// #define RX_DEVICE

#include "../src/header/Radio.h"
#include "../src/header/MqttConfig.h"
#include <WiFi.h>
#include <PubSubClient.h>

class RxRadio : public Radio
{
private:
    static volatile bool transmitFlag;
    static volatile bool recievedFlag;
    // WifiClient *espClient = nullptr;
    PubSubClient *client = nullptr;

public:
    RxRadio(/* args */);

    static void setFlag(void);
    void setupRadio(void);
    void handleRadio(void);
    void reconnect();
    void setupWifi();
    ~RxRadio();
};

volatile bool RxRadio::recievedFlag = false;

RxRadio::RxRadio(/* args */)
{
}

RxRadio::~RxRadio()
{
}

WiFiClient espClient;
PubSubClient client(espClient);

void RxRadio::setupRadio()
{
    // Configure for reception

    int state = radioLoRa.begin();
    if (state == RADIOLIB_ERR_NONE)
    {
        log_d("\t\tRadio Setup Rx: SUCESS!");
    }
    else
    {
        log_d("\t\tInitialization failed, code %d", state);
        while (true)
            ;
    }

    radioLoRa.setPacketReceivedAction(RxRadio::setFlag);
    radioLoRa.startReceive();
    setupWifi();
    // client.setServer(brokerServer, mqttPort);
}

void RxRadio::setupWifi()
{
    delay(1000);

    log_d("\t\tConnecting to: %s", ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(2000);
    }
    log_d("\t\tConnected");
}

void RxRadio::reconnect()
{
    //     while (!client.connected())
    //     {
    //         Serial.print("\nConnecting to MQTT: ");
    //         Serial.println(brokerServer);

    //         if (client.connect(clientId))
    //         {
    //             Serial.println("Connected to broker");
    //             // client.publish(topic, "Hello from ESP32!"); // Publish a message
    //         }
    //         else
    //         {
    //             Serial.print("Failed to connect, rc=");
    //             Serial.print(client.state());
    //             Serial.println(" try again in 5 seconds");
    //             delay(5000);
    //         }
    //     }
}

void RxRadio::handleRadio()
{

    //     if (!client.connected())
    //     {
    //         reconnect();
    //     }
    //     client.loop(); // Maintain connection and handle callbacks

    if (recievedFlag)
    {
        String str;
        int state = this->radioLoRa.readData(str);
        recievedFlag = false;
        log_d("Recieved new message.");

        if (state == RADIOLIB_ERR_NONE)
        {
            log_d("Recieved message: %s ", str.c_str());
        }
        else if (state == RADIOLIB_ERR_CRC_MISMATCH)
        {
            log_d("crc error!");
        }
        else
        {
            log_d("failed, code: %d", state);
        }
    }
}

// Can't do Serial or display things here, takes too much time for the interrupt
void RxRadio::setFlag(void)
{
    log_d("recieveflag");
    RxRadio::recievedFlag = true;
}

#endif