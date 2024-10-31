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
    static volatile bool receivedFlag;
    WiFiClient *espClient = nullptr;
    PubSubClient *client = nullptr;

    void mqttPublish(String &);
    void mqttPublish(String);
    void messageCallback(char *topic, byte *payload, unsigned int length);

public:
    RxRadio();
    ~RxRadio();

    static void setFlag(void);
    void setupRadio(void);
    void handleRadio(void);
    void reconnect();
    void setupWifi();
};

volatile bool RxRadio::receivedFlag = false;

RxRadio::RxRadio(/* args */)
{
    this->espClient = new WiFiClient;
    client = new PubSubClient(*espClient);
}

RxRadio::~RxRadio()
{
    delete this->espClient;
    delete this->client;
}

void RxRadio::setupRadio()
{
    // Configure radio for reception
    int state = radioLoRa.begin();
    if (state == RADIOLIB_ERR_NONE)
    {
        log_d("\t\tRadio Setup Rx: SUCCESS!");
    }
    else
    {
        log_d("\t\tInitialization failed, code %d", state);
        while (true)
            ;
    }

    // Set callback for packet reception
    radioLoRa.setPacketReceivedAction(RxRadio::setFlag);
    radioLoRa.startReceive();

    // Setup WiFi and connect to MQTT broker
    setupWifi();
    this->client->setServer(brokerServer, mqttPort);
    client->connect(clientId);
}

void RxRadio::setupWifi()
{
    log_d("\t\tConnecting to: %s", ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        log_d(".");
        delay(2000);
    }
    log_d("\t\tConnected");
}

void RxRadio::reconnect()
{
    while (!client->connected())
    {
        log_d("\nConnecting to MQTT: ");
        log_d("%s", brokerServer);

        if (client->connect(clientId))
        {
            log_d("Connected to broker");
            client->publish(topic, "Hello from ESP32!"); // Publish a message
        }
        else
        {
            log_d("Failed to connect, rc =, %i \n trying again in 5 seconds", client->state());
            delay(5000);
        }
    }
}

void RxRadio::handleRadio()
{

    if (this->client != nullptr && this->client->connected())
    {
        reconnect();
    }
    client->loop(); // Maintain connection and handle callbacks

    if (receivedFlag)
    {
        String message;
        int state = this->radioLoRa.readData(message);
        receivedFlag = false;
        log_d("Recieved new message.");

        if (state == RADIOLIB_ERR_NONE)
        {
            // log_d("Recieved message: %s ", str.c_str());
            mqttPublish(message.c_str());
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

void RxRadio::mqttPublish(String message)
{
    if (this->client != nullptr)
    {
        client->publish(topic, "%s", message.c_str());
        log_d("Published message: %s", message.c_str());
    }
    else
    {
        log_d("Client not initialzed");
    }
}

// Callback function to handle received MQTT messages
// void RxRadio::messageCallback(char *topic, byte *payload, unsigned int length)
// {
//     // Convert payload to string
//     String message = "";
//     for (unsigned int i = 0; i < length; i++)
//     {
//         message += (char)payload[i];
//     }
//     log_d("Received message on topic %s: %s", topic, message.c_str());
// }

// Can't do Serial or display things here, takes too much time for the interrupt
void RxRadio::setFlag(void)
{
    RxRadio::receivedFlag = true;
}

#endif