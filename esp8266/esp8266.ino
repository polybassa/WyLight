#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiUdp.h>

#define ESP8266_LED 5

static const char WiFiSSID[] = "Accesspoint";
static const char WiFiPSK[] = "WyLight1";

//NOTES:
// http://esp8266.github.io/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html#web-browser
void setupWifi()
{
    byte ledStatus = LOW;

    // Set WiFi mode to station (as opposed to AP or AP_STA)
    WiFi.mode(WIFI_STA);

    // WiFI.begin([ssid], [passkey]) initiates a WiFI connection
    // to the stated [ssid], using the [passkey] as a WPA, WPA2,
    // or WEP passphrase.
    WiFi.begin(WiFiSSID, WiFiPSK);

    // Use the WiFi.status() function to check if the ESP8266
    // is connected to a WiFi network.
    while (WiFi.status() != WL_CONNECTED) {
        // Blink the LED
        digitalWrite(ESP8266_LED, ledStatus); // Write LED high/low
        ledStatus = (ledStatus == HIGH) ? LOW : HIGH;

        // Delays allow the ESP8266 to perform critical tasks
        // defined outside of the sketch. These tasks include
        // setting up, and maintaining, a WiFi connection.
        delay(100);
        // Potentially infinite loops are generally dangerous.
        // Add delays -- allowing the processor to perform other
        // tasks -- wherever possible.
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println("HUHU");
    pinMode(ESP8266_LED, OUTPUT);
    setupWifi();
}

unsigned char capturedBroadcastMessage[110] = {
    0x00, 0x0f, 0xb5, 0xb2, 0x57, 0xfa, //MAC
    0x07, //channel
    0x3f, //rssi
    0x07, 0xd0, //port
    0x00, 0x00, 0x24, 0xb1, //rtc
    0x0b, 0xff, //battery
    0x0d, 0x11, //gpio
    0x54, 0x69, 0x6d, 0x65, 0x20, 0x4e, 0x4f, 0x54, 0x20, 0x53, 0x45, 0x54, 0x00, 0x00, //time
    0x57, 0x69, 0x46, 0x6c, 0x79, 0x20, 0x56, 0x65, 0x72, 0x20, 0x32, 0x2e, 0x33, 0x36,
    0x2c, 0x20, 0x30, 0x38, 0x2d, 0x32, 0x32, 0x2d, 0x32, 0x30, 0x31, 0x32, 0x00, 0x00, //version
    'W', 'i', 'F', 'l', 'y', '-', 'E', 'S', 'P', '8', '2', '6', '6',
    ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', //deviceid
    0x4e, 0x00, //boottime
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //sensors
};
WiFiUDP Udp;

static void sendBroadcast()
{
    IPAddress broadcast {0xffffffff};
    Udp.beginPacket(broadcast, 55555);
    Udp.write(capturedBroadcastMessage, sizeof(capturedBroadcastMessage));
    Udp.endPacket();
}

static void runUpdate()
{
    //const auto status = ESPhttpUpdate.update("http://brünn.net/slow_blink.bin");
    const auto status = ESPhttpUpdate.update("http://192.168.43.1:12345/slow_blink.bin");

    switch (status) {
    case HTTP_UPDATE_FAILED:
        Serial.println("[update] Update failed.");
        Serial.println(ESPhttpUpdate.getLastErrorString());
        break;

    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("[update] Update no Update.");
        break;

    case HTTP_UPDATE_OK:
        Serial.println("[update] Update ok."); // may not called we reboot the ESP
        break;
    }
}

static int count = 3;
void loop()
{
    if (--count)
        runUpdate();
    Serial.println(WiFi.localIP());
    sendBroadcast();
    digitalWrite(ESP8266_LED, HIGH);
    delay(3500);
    sendBroadcast();
    digitalWrite(ESP8266_LED, LOW);
    delay(1500);
}
