/*
   Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

   This file is part of Wifly_Light.

   Wifly_Light is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Wifly_Light is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

// Simplelink includes
#include "simplelink.h"
#include "wlan.h"

// driverlib includes
#include "hw_types.h"
#include "timer.h"
#include "rom.h"
#include "rom_map.h"

// common interface includes
#include "SimplelinkDriver.h"
#include "timer_if.h"
#include "gpio_if.h"
#include "wifi.h"

// oslib include
#include "osi.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "firmware/trace.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

#define ASSERT_ON_ERROR(line_number, error_code) \
    { \
        if (error_code < 0) { \
            Trace(ZONE_ERROR, "%d: Assert Error: %d\r\n", line_number, error_code); \
            return error_code; \
        } \
    }

const std::string SimplelinkDriver::GET_TOKEN = "__SL_G_US";
const std::string SimplelinkDriver::POST_TOKEN = "__SL_P_US";

struct SimplelinkDriver::provisioningData SimplelinkDriver::ProvisioningData;
struct SimplelinkDriver::driverStatus SimplelinkDriver::Status;
struct SimplelinkDriver::statusInformation SimplelinkDriver::Info;
xSemaphoreHandle SimplelinkDriver::ProvisioningDataSemaphore;
xSemaphoreHandle SimplelinkDriver::ConnectionLostSemaphore;

void SimpleLinkWlanEventHandler(SlWlanEvent_t* pSlWlanEvent)
{
    SimplelinkDriver::SimpleLinkWlanEventHandler(pSlWlanEvent);
}

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t* pNetAppEvent)
{
    SimplelinkDriver::SimpleLinkNetAppEventHandler(pNetAppEvent);
}

void SimpleLinkGeneralEventHandler(SlDeviceEvent_t* pDevEvent)
{
    SimplelinkDriver::SimpleLinkGeneralEventHandler(pDevEvent);
}

void SimpleLinkSockEventHandler(SlSockEvent_t* pSock)
{
    SimplelinkDriver::SimpleLinkSockEventHandler(pSock);
}

void SimpleLinkHttpServerCallback(SlHttpServerEvent_t*    pSlHttpServerEvent,
                                  SlHttpServerResponse_t* pSlHttpServerResponse)
{
    SimplelinkDriver::SimpleLinkHttpServerCallback(pSlHttpServerEvent, pSlHttpServerResponse);
}

SimplelinkDriver::SimplelinkDriver(const bool accesspointMode)
{
    Trace(ZONE_VERBOSE, "Construct Driver... \n\r");
    osi_SyncObjCreate(&ProvisioningDataSemaphore);
    osi_SyncObjCreate(&ConnectionLostSemaphore);

    long retVal = ERROR;
    if (accesspointMode) {
        retVal = this->startAsAccesspoint();
    } else {
        unsigned int RETRY = 2;
        while (RETRY-- && (retVal == ERROR)) {
            retVal = this->startAsStation();
        }
    }
    if (retVal == ERROR) {
        this->disconnect();
        sl_Stop(SL_STOP_TIMEOUT);
        Trace(ZONE_VERBOSE, "Construct Driver failed...\n\r");
        reset();
    }
}

SimplelinkDriver::~SimplelinkDriver(void)
{
    this->disconnect();
    sl_Stop(SL_STOP_TIMEOUT);
    Trace(ZONE_VERBOSE, "Destruct Driver...\n\r");
}

long SimplelinkDriver::startAsStation(void)
{
    long retRes = configureSimpleLinkToDefaultState();
    ASSERT_ON_ERROR(__LINE__, retRes);

    retRes = sl_Start(NULL, NULL, NULL);
    ASSERT_ON_ERROR(__LINE__, retRes);

    Trace(ZONE_VERBOSE, "Started SimpleLink Device in STA Mode\n\r");

    return waitForConnectWithTimeout(CONNECT_TIMEOUT);
}

long SimplelinkDriver::startAsAccesspoint(void)
{
    long retRes = ERROR;

    retRes = configureSimpleLinkToDefaultState();
    ASSERT_ON_ERROR(__LINE__, retRes);

    retRes = scanForAccesspoints();
    ASSERT_ON_ERROR(__LINE__, retRes);

    retRes = configureAsAccesspoint();
    ASSERT_ON_ERROR(__LINE__, retRes);

    retRes = sl_Start(NULL, NULL, NULL);
    ASSERT_ON_ERROR(__LINE__, retRes);

    Trace(ZONE_VERBOSE, "Start AP\r\n");
    //Wait for Ip Acquired Event in AP Mode
    while (!Status.IPAcquired) {
        osi_Sleep(10);
    }

    return 0;
}

long SimplelinkDriver::configureAsAccesspoint(void)
{
    long retRes = sl_Start(NULL, NULL, NULL);
    ASSERT_ON_ERROR(__LINE__, retRes);

    //Switch to AP Mode
    sl_WlanSetMode(ROLE_AP);

    //Disable ROM WebPages
    unsigned char disable = 0;
    retRes = sl_NetAppSet(SL_NET_APP_HTTP_SERVER_ID, NETAPP_SET_GET_HTTP_OPT_ROM_PAGES_ACCESS, 1, &disable);
    ASSERT_ON_ERROR(__LINE__, retRes);

    // set domain name
    unsigned char domainName[] = "wylight.config";
    retRes = sl_NetAppSet(SL_NET_APP_DEVICE_CONFIG_ID, NETAPP_SET_GET_DEV_CONF_OPT_DOMAIN_NAME, sizeof(domainName),
                          domainName);
    ASSERT_ON_ERROR(__LINE__, retRes);

    // set Accesspoint SSID
    unsigned char ssid[] = "WyLightAP";
    retRes = sl_WlanSet(SL_WLAN_CFG_AP_ID, 0, sizeof(ssid), ssid);
    ASSERT_ON_ERROR(__LINE__, retRes);

    // get current Time
    SlDateTime_t dateTime;
    unsigned char option = SL_DEVICE_GENERAL_CONFIGURATION_DATE_TIME, length = sizeof(SlDateTime_t);
    retRes = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &option, &length, (unsigned char*)&dateTime);
    ASSERT_ON_ERROR(__LINE__, retRes);

    // compute random channel from current time
    unsigned char channel = (dateTime.sl_tm_sec % 13) + 1; // to avoid channel 0
    retRes = sl_WlanSet(SL_WLAN_CFG_AP_ID, 3, 1, &channel);
    ASSERT_ON_ERROR(__LINE__, retRes);
    Trace(ZONE_VERBOSE, "Accesspoint channel: %d\r\n", channel);

    sl_Stop(SL_STOP_TIMEOUT);

    Status.reset();

    return 0;
}

long SimplelinkDriver::configureSimpleLinkToDefaultState(void)
{
    reset();

    int Mode = sl_Start(NULL, NULL, NULL);
    ASSERT_ON_ERROR(__LINE__, Mode);

    // If the device is not in station-mode, try putting it in staion-mode
    if (ROLE_STA != Mode) {
        if (ROLE_AP == Mode)
            // If the device is in AP mode, we need to wait for this event before doing anything
            while (!Status.IPAcquired) {
                osi_Sleep(10);
            }

        // Switch to STA role and restart
        long retRes = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(__LINE__, retRes);

        retRes = sl_Stop(SL_STOP_TIMEOUT);
        ASSERT_ON_ERROR(__LINE__, retRes);

        Status.reset();

        retRes = sl_Start(NULL, NULL, NULL);
        ASSERT_ON_ERROR(__LINE__, retRes);

        // Check if the device is in station again
        if (ROLE_STA != retRes)
            // We don't want to proceed if the device is not up in STA-mode
            return ERROR;
    }

    // Device in station-mode. Disconnect previous connection if any
    // The function returns 0 if 'Disconnected done', negative number if already
    // disconnected Wait for 'disconnection' event if 0 is returned, Ignore
    // other return-codes
    SimplelinkDriver::disconnect();

    // Set connection policy to Auto + Fast Connection
    //      (Device's default connection policy)
    long retRes = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 1, 0, 0, 0), NULL, 0);
    ASSERT_ON_ERROR(__LINE__, retRes);

    // Enable DHCP client
    unsigned char value = 1;
    retRes = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE, 1, sizeof(value), &value);
    ASSERT_ON_ERROR(__LINE__, retRes);

    // Disable scan
    unsigned char configOpt = SL_SCAN_POLICY(0);
    retRes = sl_WlanPolicySet(SL_POLICY_SCAN, configOpt, NULL, 0);
    ASSERT_ON_ERROR(__LINE__, retRes);

    // Set Tx power level for station mode
    // Number between 0-15, as dB offset from max power - 0 will set max power
    unsigned char power = 0;
    retRes = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
                        WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, sizeof(power), (unsigned char*)&power);
    ASSERT_ON_ERROR(__LINE__, retRes);

    // Set PM policy to normal
    retRes = sl_WlanPolicySet(SL_POLICY_PM, SL_NORMAL_POLICY, NULL, 0);
    ASSERT_ON_ERROR(__LINE__, retRes);

    // Set URN-Name
    unsigned char urnName[] = "WyLight";
    retRes = sl_NetAppSet(SL_NET_APP_DEVICE_CONFIG_ID, NETAPP_SET_GET_DEV_CONF_OPT_DEVICE_URN, sizeof(urnName),
                          urnName);
    ASSERT_ON_ERROR(__LINE__, retRes);

    retRes = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(__LINE__, retRes);

    reset();

    return retRes;
}

long SimplelinkDriver::scanForAccesspoints(void)
{
    const unsigned long WAIT_TIME = 8000;
    const unsigned long SCAN_INTERVAL = 4;

    long retRes = sl_Start(NULL, NULL, NULL);
    ASSERT_ON_ERROR(__LINE__, retRes);

    sl_WlanDisconnect();

    // Set connection policy to zero, that no scan in background is performed
    //      (Device's default connection policy)
    retRes = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(0, 0, 0, 0, 0), NULL, 0);
    ASSERT_ON_ERROR(__LINE__, retRes);

    // Restart Simplelink
    sl_Stop(SL_STOP_TIMEOUT);

    Status.reset();

    retRes = sl_Start(NULL, NULL, NULL);
    ASSERT_ON_ERROR(__LINE__, retRes);

    const unsigned char parameterLen = sizeof(SCAN_INTERVAL);
    //Scan AP in STA mode
    retRes = sl_WlanPolicySet(SL_POLICY_SCAN, SL_SCAN_POLICY_EN(1), (unsigned char*)&SCAN_INTERVAL, parameterLen);
    ASSERT_ON_ERROR(__LINE__, retRes);

    Trace(ZONE_VERBOSE, "Scanning for SSID's\r\n----------------------------------------------\r\n");
    // wait for scan to complete
    osi_Sleep(WAIT_TIME);

    //Get Scan Result
    retRes = sl_WlanGetNetworkList(0, MAX_NUM_NETWORKENTRIES, &ProvisioningData.networkEntries[0]);
    ASSERT_ON_ERROR(__LINE__, retRes);

    int i;
    for (i = 0; i < retRes; i++) {
        Trace(ZONE_VERBOSE, "%d) SSID %s\n\r", i, ProvisioningData.networkEntries[i].ssid);
    }
    Trace(ZONE_VERBOSE, "\r\n----------------------------------------------\r\n");

    sl_Stop(SL_STOP_TIMEOUT);

    Status.reset();

    return 0;
}

void SimplelinkDriver::responseNetworkEntries(const unsigned long entryNumber, SlHttpServerResponse_t* response)
{
    memcpy(response->ResponseData.token_value.data,
           ProvisioningData.networkEntries[entryNumber].ssid,
           ProvisioningData.networkEntries[entryNumber].ssid_len);
    response->ResponseData.token_value.len = ProvisioningData.networkEntries[entryNumber].ssid_len;
}

long SimplelinkDriver::extractTokenNumber(SlHttpServerEvent_t const* const event)
{
    std::string serverGetToken((const char*)event->EventData.httpTokenName.data,
                               (size_t)event->EventData.httpTokenName.len);
    if (serverGetToken.find(GET_TOKEN) == std::string::npos) return -1;
    return (*(--serverGetToken.end())) - '0';
}

char SimplelinkDriver::extractTokenParameter(SlHttpServerEvent_t const* const event)
{
    std::string serverPostToken((const char*)event->EventData.httpPostData.token_name.data,
                                event->EventData.httpPostData.token_name.len);
    if (serverPostToken.find(POST_TOKEN) == std::string::npos) return 0;
    return *(--serverPostToken.end());
}

void SimplelinkDriver::setSecurityKey(SlHttpServerEvent_t const* const event)
{
    memcpy(
        ProvisioningData.wlanSecurityKey.data(), event->EventData.httpPostData.token_value.data,
        event->EventData.httpPostData.token_value.len);

    ProvisioningData.secParameters.Key = ProvisioningData.wlanSecurityKey.data();
    ProvisioningData.secParameters.KeyLen = event->EventData.httpPostData.token_value.len;
}

void SimplelinkDriver::setSecurityType(SlHttpServerEvent_t const* const event)
{
    switch (event->EventData.httpPostData.token_value.data[0]) {
    case '1':
        ProvisioningData.secParameters.Type = SL_SEC_TYPE_WEP;
        break;

    case '2':
        ProvisioningData.secParameters.Type = SL_SEC_TYPE_WPA;
        break;

    case '3':
        ProvisioningData.secParameters.Type = SL_SEC_TYPE_WPS_PBC;
        break;

    case '4':
        ProvisioningData.secParameters.Type = SL_SEC_TYPE_WPS_PIN;
        break;

    default:
        ProvisioningData.secParameters.Type = SL_SEC_TYPE_OPEN;
        break;
    }
}

void SimplelinkDriver::SimpleLinkWlanEventHandler(SlWlanEvent_t* pSlWlanEvent)
{
    switch (((SlWlanEvent_t*)pSlWlanEvent)->Event) {
    case SL_WLAN_CONNECT_EVENT:
        Status.connected = true;
        Status.connectFailed = false;
        memcpy(
            Info.ConnectionSSID.data(), pSlWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_name,
            pSlWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len);
        memcpy(Info.ConnectionBSSID.data(), pSlWlanEvent->EventData.STAandP2PModeWlanConnected.bssid, SL_BSSID_LENGTH);
        Trace(ZONE_INFO, "[WLAN EVENT] STA Connected to the AP: %s\n\r", Info.ConnectionSSID.data());
        break;

    case SL_WLAN_DISCONNECT_EVENT:
        Status.connected = false;
        Status.IPAcquired = false;
        Trace(ZONE_INFO, "[WLAN EVENT] Device disconnected from AP: %s\n\r", Info.ConnectionSSID.data());
        Info.ConnectionSSID.fill(0);
        Info.ConnectionBSSID.fill(0);
        osi_SyncObjSignalFromISR(&ConnectionLostSemaphore);
        break;

    case SL_WLAN_STA_CONNECTED_EVENT:
        {
            // when device is in AP mode and any client connects to device cc3xxx
            Status.connected = true;
            Status.connectFailed = false;
            slPeerInfoAsyncResponse_t* pEventData = &pSlWlanEvent->EventData.APModeStaConnected;
            Trace(ZONE_INFO, "[WLAN EVENT] Client connected: %x:%x:%x:%x:%x:%x\r\n", pEventData->mac[0],
                  pEventData->mac[1],
                  pEventData->mac[2], pEventData->mac[3], pEventData->mac[4], pEventData->mac[5]);
        }
        break;

    case SL_WLAN_STA_DISCONNECTED_EVENT:
        {
            // when client disconnects from device (AP)
            Status.connected = false;
            Status.IPAcquired = false;
            Status.IPLeased = false;
            slPeerInfoAsyncResponse_t* pEventData = &pSlWlanEvent->EventData.APModestaDisconnected;
            Trace(ZONE_INFO,
                  "[WLAN EVENT] Client disconnected: %x:%x:%x:%x:%x:%x\r\n",
                  pEventData->mac[0],
                  pEventData->mac[1],
                  pEventData->mac[2],
                  pEventData->mac[3],
                  pEventData->mac[4],
                  pEventData->mac[5]);
            osi_SyncObjSignalFromISR(&ConnectionLostSemaphore);
        }
        break;

    case SL_WLAN_CONNECTION_FAILED_EVENT:
        // If device gets any connection failed event
        Status.connected = false;
        Status.IPAcquired = false;
        Status.connectFailed = true;
        Trace(ZONE_INFO, "[WLAN EVENT] Connection failed\r\n");
        osi_SyncObjSignalFromISR(&ConnectionLostSemaphore);
        break;

    default:
        Trace(ZONE_INFO, "[WLAN EVENT] Unexpected event \n\r");
        break;
    }
}

void SimplelinkDriver::SimpleLinkNetAppEventHandler(SlNetAppEvent_t* pNetAppEvent)
{
    switch (pNetAppEvent->Event) {
    case SL_NETAPP_IPV4_ACQUIRED:
    case SL_NETAPP_IPV6_ACQUIRED:
        Status.IPAcquired = true;
        Trace(ZONE_INFO, "[NETAPP EVENT] IP Acquired\r\n");
        break;

    case SL_NETAPP_IP_LEASED:
        {
            Status.IPLeased = true;
            SlIpLeasedAsync_t* pEventData = &pNetAppEvent->EventData.ipLeased;
            Info.StationIpAddress = pEventData->ip_address;
            Trace(ZONE_INFO, "[NETAPP EVENT] IP Leased\r\n");
        }
        break;

    case SL_NETAPP_IP_RELEASED:
        Status.IPLeased = false;
        Info.StationIpAddress = 0;
        Trace(ZONE_INFO, "[NETAPP EVENT] IP Released\r\n");
        break;

    case SL_NETAPP_SOCKET_TX_FAILED:
        Trace(ZONE_INFO, "[NETAPP EVENT] Socket Error # %d \n\r", pNetAppEvent->EventData.sd);
        break;

    default:
        Trace(ZONE_INFO, "[NETAPP EVENT] Unexpected event \n\r");
        break;
    }
}

void SimplelinkDriver::SimpleLinkGeneralEventHandler(SlDeviceEvent_t* pDevEvent)
{
    Trace(ZONE_INFO,
          "[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n",
          pDevEvent->EventData.deviceEvent.status,
          pDevEvent->EventData.deviceEvent.sender);
}

void SimplelinkDriver::SimpleLinkSockEventHandler(SlSockEvent_t* pSock)
{
    if (pSock->Event == SL_NETAPP_SOCKET_TX_FAILED) {
        if (pSock->EventData.status == SL_ECLOSE)
            Trace(ZONE_ERROR,
                  "[SOCK ERROR] - close socket (%d) operation failed to transmit all queued packets\n\n",
                  pSock->EventData.sd);
        else
            Trace(ZONE_ERROR,
                  "[SOCK ERROR] - TX FAILED  :  socket (%d) , reason (%d)\n\n",
                  pSock->EventData.sd,
                  pSock->EventData.status);
    } else {
        Trace(ZONE_INFO, "[SOCK EVENT] - Unexpected Event [%x0x]\n\n", pSock->Event);
    }
}

void SimplelinkDriver::SimpleLinkHttpServerCallback(SlHttpServerEvent_t*    pSlHttpServerEvent,
                                                    SlHttpServerResponse_t* pSlHttpServerResponse)
{
    if (pSlHttpServerEvent->Event == SL_NETAPP_HTTPGETTOKENVALUE) {
        const unsigned long getTokenNumber = extractTokenNumber(pSlHttpServerEvent);
        if ((getTokenNumber < 0) || (getTokenNumber > MAX_NUM_NETWORKENTRIES))
            return;
        responseNetworkEntries(getTokenNumber, pSlHttpServerResponse);
    } else if (pSlHttpServerEvent->Event == SL_NETAPP_HTTPPOSTTOKENVALUE) {
        Trace(ZONE_VERBOSE, " token_name: %s ", pSlHttpServerEvent->EventData.httpPostData.token_name.data);
        Trace(ZONE_VERBOSE, " token_data: %s \r\n", pSlHttpServerEvent->EventData.httpPostData.token_value.data);

        const char postTokenParameter = extractTokenParameter(pSlHttpServerEvent);

        slHttpServerString_t const* const postString = &pSlHttpServerEvent->EventData.httpPostData.token_value;

        switch (postTokenParameter) {
        case 'C':
            if (std::string((const char*)postString->data, postString->len) == "Add")
                osi_SyncObjSignalFromISR(&ProvisioningDataSemaphore);
            break;

        case 'D':
            memcpy(ProvisioningData.wlanSSID.data(), postString->data, postString->len);
            break;

        case 'E':
            setSecurityType(pSlHttpServerEvent);
            break;

        case 'F':
            setSecurityKey(pSlHttpServerEvent);
            break;

        case 'G':
            ProvisioningData.priority = postString->data[0] - '0';
            break;

        default:
            break;
        }
    }
}

long SimplelinkDriver::waitForConnectWithTimeout(const unsigned int timeout_ms)
{
    static const unsigned int INTERVAL = 5;
    unsigned int timeoutCounter = 0;

    while ((timeoutCounter < timeout_ms) && !Status.IPAcquired) {
        osi_Sleep(INTERVAL);
        timeoutCounter += INTERVAL;
    }

    if (!Status.connected || !Status.IPAcquired) {
        Trace(ZONE_ERROR, "Connecting failed \r\n");
        return ERROR;
    } else {
        return 0;
    }
}

void SimplelinkDriver::driverStatus::reset(void)
{
    this->networkProcessorOn = false;
    this->connected = false;
    this->IPLeased = false;
    this->IPAcquired = false;
    this->connectFailed = false;
    this->pingDone = false;
    osi_SyncObjClear(&SimplelinkDriver::ConnectionLostSemaphore);
}

void SimplelinkDriver::statusInformation::reset()
{
    this->SimpleLinkStatus = 0;
    this->StationIpAddress = 0;
    this->GatewayIpAddress = 0;
    this->ConnectionSSID.fill(0);
    this->ConnectionBSSID.fill(0);
    this->ConnectionTimeDelayIndex = 0;
}

void SimplelinkDriver::provisioningData::reset(void)
{
    this->priority = 0;
    this->wlanSSID.fill(0);
    this->wlanSecurityKey.fill(0);
    memset(&this->secParameters, 0, sizeof(this->secParameters));
    memset(this->networkEntries, 0, sizeof(this->networkEntries));
    osi_SyncObjClear(&SimplelinkDriver::ProvisioningDataSemaphore);
}

void SimplelinkDriver::reset(void)
{
    Info.reset();
    Status.reset();
    ProvisioningData.reset();
}

void SimplelinkDriver::disconnect(void)
{
    Trace(ZONE_VERBOSE, "Disconnect...\n\r");
    if (sl_WlanDisconnect() == 0)
        while (Status.connected) {
            osi_Sleep(10);
        }
}

SimplelinkDriver::operator bool() const
{
    return this->isConnected();
}

bool SimplelinkDriver::isConnected(void) const
{
    return Status.connected || Status.IPAcquired;
}

void SimplelinkDriver::waitUntilConnectionLost(void) const
{
    Trace(ZONE_VERBOSE, "Wait until connection lost...\n\r");
    osi_SyncObjWait(&ConnectionLostSemaphore, OSI_WAIT_FOREVER);
}

void SimplelinkDriver::waitForNewProvisioningData(void) const
{
    do {
        Trace(ZONE_VERBOSE, "Wait for provisioning data...\n\r");
        osi_SyncObjWait(&ProvisioningDataSemaphore, OSI_WAIT_FOREVER);
    } while (this->addNewProfile() != 0);
}

long SimplelinkDriver::addNewProfile(void)
{
    if ((ProvisioningData.secParameters.Type == SL_SEC_TYPE_WPS_PBC) ||
        (ProvisioningData.secParameters.Type == SL_SEC_TYPE_WPS_PIN))
    {
        if (ProvisioningData.secParameters.Type == SL_SEC_TYPE_WPS_PBC) {
            ProvisioningData.secParameters.KeyLen = 0;
            ProvisioningData.secParameters.Key = nullptr;
        }
        Status.reset();

        sl_WlanConnect(ProvisioningData.wlanSSID.data(), strlen(
                           ProvisioningData.wlanSSID.data()), 0, &ProvisioningData.secParameters, 0);
        return SimplelinkDriver::waitForConnectWithTimeout(CONNECT_TIMEOUT * 4);
    } else {
        long retRes = ERROR;
        retRes = sl_WlanProfileAdd(ProvisioningData.wlanSSID.data(), strlen(
                                       ProvisioningData.wlanSSID.data()), 0, &ProvisioningData.secParameters, 0, ProvisioningData.priority,
                                   0);
        if (retRes < 0) {
            // Remove all profiles
            retRes = sl_WlanProfileDel(0xFF);
            ASSERT_ON_ERROR(__LINE__, retRes);
            // and try again
            retRes = sl_WlanProfileAdd(ProvisioningData.wlanSSID.data(), strlen(
                                           ProvisioningData.wlanSSID.data()), 0, &ProvisioningData.secParameters, 0, ProvisioningData.priority,
                                       0);
            ASSERT_ON_ERROR(__LINE__, retRes);
        }
        Trace(ZONE_VERBOSE, "Added Profile at index %d \r\n", retRes);
    }
    return 0;
}
