#ifdef RX_DEVICE1
// #include <header/Rx.h>

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
    int state = this->setRadioConfig();

    // Set callback for packet reception
    radioLoRa.setPacketReceivedAction(RxRadio::setFlag);
    radioLoRa.startReceive();

    // Setup WiFi and connect to MQTT broker
    setupWifi();
    // this->client->setServer(brokerServer, mqttPort);
    // client->connect("teste");
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
    //     while (!client->connected())
    //     {
    //         log_d("\nConnecting to MQTT: ");
    //         log_d("%s", brokerServer);

    //         if (client->connect(clientId))
    //         {
    //             log_d("Connected to broker");
    //             client->publish(topic, "Hello from ESP32!");
    //         }
    //         else
    //         {
    //             log_d("Failed to connect, rc =, %i \n trying again in 5 seconds", client->state());
    //             delay(5000);
    //         }
    //     }
}

void RxRadio::handleRadio()
{

    if (this->client != nullptr && this->client->connected())
    {
        reconnect();
    }
    // client->loop(); // Maintain connection and handle callbacks
    noInterrupts();
    if (receivedFlag)
    {
        digitalWrite(LED_BUILTIN, true);
        String message;
        int state = this->radioLoRa.readData(message);
        receivedFlag = false;
        log_d("Recieved new message.");

        if (state == RADIOLIB_ERR_NONE)
        {
            log_d("Recieved message: %s ", message.c_str());
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
    interrupts();
}

void RxRadio::mqttPublish(String message)
{
    if (this->client != nullptr)
    {
        String timestamp = String(millis());
        String status = "StatusOK";

        String finalMessage = status + "::" + message;

        client->publish(topic, finalMessage.c_str());
        log_d("Published message: %s", finalMessage.c_str());
        digitalWrite(LED_BUILTIN, false);
    }
    else
    {
        log_d("Client not initialzed");
    }
}

// Can't do Serial or display things here, takes too much time for the interrupt
void RxRadio::setFlag(void)
{
    RxRadio::receivedFlag = true;
}
#endif