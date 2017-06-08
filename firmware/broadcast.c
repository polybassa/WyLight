/**
   Copyright (C) 2017 Nils Weiss, Patrick Bruenn.

   This file is part of WyLight.

   WyLight is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   WyLight is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#include "platform.h"

static int wifi_alive = 0;
static const uint16_t BROADCAST_PORT = 55555;

static const uint8_t capturedBroadcastMessage[110] = {
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
    'W', 'i', 'F', 'l', 'y', '-', 'E', 'Z', 'X',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', //deviceid
    0x4e, 0x00, //boottime
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //sensors
};

static void wifi_task(void* pvParameters)
{
    uint8_t status = 0;
    uint8_t retries = 30;
    struct sdk_station_config config = {
        .ssid = "Accesspoint",
        .password = "WyLight1",
    };

    int udpSocket;
    do {
        udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    } while (-1 == udpSocket);

    struct sockaddr_in broadcastAddress;
    broadcastAddress.sin_family = AF_INET;
    broadcastAddress.sin_port = htons(BROADCAST_PORT);
    broadcastAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    int val = 1;
    setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));

    printf("%s: Connecting to WiFi\n\r", __func__);
    sdk_wifi_set_opmode(STATION_MODE);
    sdk_wifi_station_set_config(&config);
    sdk_wifi_station_connect();

    while (1) {
        wifi_alive = 0;

        while ((status != STATION_GOT_IP) && (retries)) {
            status = sdk_wifi_station_get_connect_status();
            printf("%s: status = %d\n\r", __func__, status);

            switch (status) {
            case STATION_WRONG_PASSWORD:
                printf("WiFi: wrong password\n\r");
                break;

            case STATION_NO_AP_FOUND:
                printf("WiFi: AP not found\n\r");
                break;

            case STATION_CONNECT_FAIL:
                printf("WiFi: connection failed\r\n");
                break;

            default:
                break;
            }

            vTaskDelay(1000 / portTICK_PERIOD_MS);
            --retries;
        }

        while ((status = sdk_wifi_station_get_connect_status()) ==
               STATION_GOT_IP)
        {
            if (wifi_alive == 0) {
                printf("WiFi: Connected\n\r");
                wifi_alive = 1;
            }

            sendto(udpSocket,
                   capturedBroadcastMessage,
                   sizeof(capturedBroadcastMessage),
                   0,
                   (struct sockaddr*)&broadcastAddress,
                   sizeof(broadcastAddress));
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }

        wifi_alive = 0;
        printf("WiFi: disconnected\n\r");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
