
#define DEBUG

#include <WiFi.h>
#include <WebSocketServer.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include <QueueList.h>

#define ECHO_TEST_TXD  (GPIO_NUM_4)
#define ECHO_TEST_RXD  (GPIO_NUM_5)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

// if you want to send long messages (more than 1024 bytes)
// change this.
#define BUF_SIZE (4096)

//WiFiServer server(80);
QueueList<String> recv_q;
QueueList<String> send_q;

const char* ssid = "Stanford Visitor"; // Change this
const char* password =  "Visitor"; // Change this

// The server will be at 192.168.4.1
// and this device will have the IP address 192.168.4.30
//IPAddress local_IP(192, 168, 4, 30);
//IPAddress gateway(192, 168, 4, 1);
//IPAddress subnet(255, 255, 255, 0);

// start a thread to handle uart send/recv
void app_main()
{
    xTaskCreate(echo_task, "uart_echo_task", BUF_SIZE, NULL, 10, NULL);
}

void setup() {
    // the UART baud is 115,200 bps
    Serial.begin(115200);

    // The blue LED is used to indicate that there is a websocket connection
    pinMode(LED_BUILTIN, OUTPUT);

    // attempt to connect to Wifi and set up a websocket connection
    connect_client();

    // start the UART listening routine*/
    app_main();
}

// this is the UART handling routine
static void echo_task(void *pvParameters)
{
    /*  Configure parameters of an UART driver,
        communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);

    // Configure a temporary buffer for the incoming data
    // Also set up a buffer for the base64 data, so we can send all
    // 0-255 byte values
    char *data = (char *) malloc(BUF_SIZE);
    char *base64data = (char *) malloc(BUF_SIZE);

    while (1) {
        // If there is anything in the receive queue (from WiFi), send it on UART
        if (!recv_q.isEmpty()) {
            // need to make this thread safe!
            // Get the data out of the queue
            String base64data = recv_q.pop();

            // Convert from base64 back to bytes to send on UART
            int len = base64_decode(data, (char *)base64data.c_str(), base64data.length());

#ifdef DEBUG
            Serial.print("sending on uart: ");
            Serial.println(data);
#endif

            uart_write_bytes(UART_NUM_1, data, len);
        }
        // If there is anything waiting on UART, send it to the other device
        int len = uart_read_bytes(UART_NUM_1, (uint8_t *)data, BUF_SIZE, 20 / portTICK_RATE_MS);
        if (len > 0) {
            int len_encoded = base64_encode(base64data, (char *)data, len);
            send_q.push(base64data);
#ifdef DEBUG
            Serial.print("Received from UART: '");
            Serial.print(base64data);
            Serial.println("'");

            /*  if (len == 1 && data[0] == 0x15) {
                Serial.println("nak");
                //continue;
                }*/
#endif

        }

        // If there is anything in the receive queue, send it on UART

        delay(10);
    }
    free(data);
    free(base64data);
}

void loop() {

    String data;
    WiFiClient client;
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("connected");
        if (client.connect("stanford.edu", 80)) {
            client.println("GET /~evandeo/cgi-bin/gettime.py");
            client.println();
            Serial.println("connecting to gettime.py");
            while (!client.available()) {};
            while (client.available()) {
                char c = client.read();
                Serial.write(c);
            }
            if (!client.connected()) {
                Serial.println();
                Serial.println("disconnecting from server.");
                client.stop();
            }
            delay(1000);
        }
    } else {
        Serial.println("Client disconnected.");
        digitalWrite(LED_BUILTIN, LOW);
        connect_client();
    }
}

void connect_client() {
    digitalWrite(LED_BUILTIN, LOW);
    /*  if (!WiFi.config(local_IP, gateway, subnet)) {

        Serial.println("STA Failed to configure");
        }*/

    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("ESP Mac Address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway IP: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS: ");
    Serial.println(WiFi.dnsIP());
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
}
