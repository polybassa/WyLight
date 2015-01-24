/**
                Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

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

#include "ConfigControl.h"
#include "trace.h"

namespace WyLight
{
    static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING;
    
	const std::list<std::string> ConfigControl::RN171_DEFAULT_PARAMETERS = {
		"set broadcast interval 0x1\r\n",    	// to support fast broadcast recognition
		"set comm close 0\r\n",            		// Disable *CLOS* string
		"set comm open 0\r\n",             		// Disable *OPEN* string
		"set comm remote 0\r\n",           		// Disable *Hello* string
		"set comm time 5\r\n", 					// Set flush timer to 5 ms
		"set comm idle 240\r\n",				// Set idle timer to 240 s, to close tcp connections after 240s if there's no traffic
		"set dns name rn.microchip.com\r\n",    // set dns of updateserver
		"set ip flags 0x6\r\n",                	// if the module loses the accesspoint connection, the connection is closed
		"set ip dhcp 1\r\n",               		// enable DHCP client
		//"set ftp address 169.254.7.57\r\n",	// configure localhost as ftp server in ad-hoc connection
		"set ftp pass Pass123\r\n",        		// configure ftp password
		"set ftp user roving\r\n",         		// configure ftp username
		"set opt deviceid Wifly_Light\r\n", 	// Set deviceid which appears in broadcastmsg to "Wifly_Light"
		"set uart baud 115200\r\n",        		// PIC uart parameter
		"set uart flow 0\r\n",             		// PIC uart parameter
		"set uart mode 0\r\n",             		// PIC uart parameter
		"set wlan channel 0\r\n",              	// Set the wlan channel to 0 to perform an automatic scan for a free channel
		"set wlan auth 4\r\n",             		// use WPA2 protection
		"set wlan join 1\r\n",             		// scan for ap and auto join
		"set wlan rate 0\r\n",             		// slowest datarate but highest range
		"set wlan tx 12\r\n",              		// Set the Wi-Fi transmit power to maximum
		"set sys printlvl 0\r\n",				// Disables Debug Messages to UART
		"set ip p 11\r\n",                      // Enable UDP, TCP_CLIENT and TCP Protocol
		//"set sys launch_string wps_app"	   	// Choose Wps mode
	};
	const std::list<std::string> ConfigControl::RN171_BASIC_PARAMETERS = {
		"set broadcast interval 0x1\r\n",    	// to support fast broadcast recognition
		"set comm close 0\r\n",            		// Disable *CLOS* string
		"set comm open 0\r\n",            	 	// Disable *OPEN* string
		"set comm remote 0\r\n",           		// Disable *Hello* string
		"set comm time 5\r\n", 					// Set flush timer to 5 ms
		"set comm idle 240\r\n",				// Set idle timer to 240 s, to close tcp connections after 240s if there's no traffic
		"set dns name rn.microchip.com\r\n",    // set dns of updateserver
		"set ip flags 0x6\r\n",                	// if the module loses the accesspoint connection, the connection is closed
		"set ip dhcp 1\r\n",               		// enable DHCP client
		"set ftp pass Pass123\r\n",        		// configure ftp password
		"set ftp user roving\r\n",         		// configure ftp username
		"set uart baud 115200\r\n",        		// PIC uart parameter
		"set uart flow 0\r\n",             		// PIC uart parameter
		"set uart mode 0\r\n",             		// PIC uart parameter
		"set wlan channel 0\r\n",               // Set the wlan channel to 0 to perform an automatic scan for a free channel
		"set wlan auth 4\r\n",             		// use WPA2 protection
		"set wlan join 1\r\n",             		// scan for ap and auto join
		"set wlan rate 0\r\n",             		// slowest datarate but highest range
		"set wlan tx 12\r\n",              		// Set the Wi-Fi transmit power to maximum
		"set sys printlvl 0\r\n",				// Disables Debug Messages to UART
		"set ip p 11\r\n",                      // Enable UDP, TCP_CLIENT and TCP Protocol
		//"set sys launch_string wps_app"	   	// Choose Wps mode
	};
	const std::list<std::string> ConfigControl::RN171_SOFT_AP_DEFAULT_PARAMETERS = {
		"set broadcast interval 1\r\n",    		// to support fast broadcast recognition
		"set comm close 0\r\n",            		// Disable *CLOS* string
		"set comm open 0\r\n",             		// Disable *OPEN* string
		"set comm remote 0\r\n",           		// Disable *Hello* string
		"set comm time 5\r\n", 					// Set flush timer to 5 ms
		"set comm idle 240\r\n",				// Set idle timer to 240 s, to close tcp connections after 240s if there's no traffic
		//"set dns name rn.microchip.com\r\n",	// set dns of updateserver
		"set ip dhcp 4\r\n",               		// enable DHCP server
		"set ftp address 169.254.7.57\r\n", 	// configure localhost as ftp server in ad-hoc connection
		"set ftp pass Pass123\r\n",        		// configure ftp password
		"set ftp user roving\r\n",         		// configure ftp username
		"set uart baud 115200\r\n",        		// PIC uart parameter
		"set uart flow 0\r\n",             		// PIC uart parameter
		"set uart mode 0\r\n",             		// PIC uart parameter
		"set wlan join 7\r\n",             		// enable AP mode
		"set wlan rate 0\r\n",             		// slowest datarate but highest range
		"set wlan tx 12\r\n",              		// Set the Wi-Fi transmit power to maximum
		"set wlan channel 1\r\n",              	// Set the wlan channel to 0 to perform an automatic scan for a free channel
		"set ip a 1.2.3.4\r\n",            		// Set ip address for accespoint
		"set ip g 0.0.0.0\r\n",                	// Set gateway address to zero
		"set ip n 255.255.255.0\r\n",      		// Set netmask for accespoint
		"set sys printlvl 0\r\n",				// Disables Debug Messages to UART
		"set ip p 11\r\n",                     	// Enable UDP, TCP_CLIENT and TCP Protocol
	};

	const std::list<std::string> ConfigControl::RN171_FACTORY_RESET_PARAMETER = {
		"factory RESET\r\n"
	};

	ConfigControl::ConfigControl(const TelnetProxy &telnet)
		: mTelnet(telnet)
	{
	}

	bool ConfigControl::ModuleForWlan(const std::string& phrase, const std::string& ssid, const std::string& deviceId) const
	{
		if(!FactoryReset()) {
			Trace(ZONE_ERROR, "factory RESET failed\n");
			return false;
		}

		if(!SetDefaults()) {
			Trace(ZONE_ERROR, "set defaults failed\n");
			return false;
		}

		if(!SetWlan(phrase, ssid)) {
			Trace(ZONE_ERROR, "set wlan phrase and ssid failed\n");
			return false;
		}

		if(!SetDeviceId(deviceId)) {
			Trace(ZONE_ERROR, "set device name failed\n");
			return false;
		}

		return RebootWlanModule();
	}

	bool ConfigControl::SetParameters(std::list<std::string> commands) const
	{
		if(!mTelnet.Open()) {
				Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}

		for(auto command : commands) {
			if(!mTelnet.Send(command)) {
				Trace(ZONE_ERROR, "command: '%s' failed -> exit without saving\n", command.c_str());
				return mTelnet.Close(false);
			}
		}
		return mTelnet.Close(true);
	}

	bool ConfigControl::FactoryReset(void) const
	{
		if(!mTelnet.Open()) {
			Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}

		if(!mTelnet.Send(RN171_FACTORY_RESET_PARAMETER.front(), FACTORY_RESET_ACK)) {
			Trace(ZONE_ERROR, "factory RESET failed\n");
			return false;
		}

		mTelnet.ClearResponse();
		return mTelnet.Close(true);
	}

	bool ConfigControl::SetDefaults(void) const
	{
		return SetParameters(RN171_DEFAULT_PARAMETERS);
	}

	bool ConfigControl::SetWlan(const std::string& phrase, const std::string& ssid) const
	{
		static const size_t PHRASE_MAX = 63;
		static const size_t SSID_MAX = 32;

		if((phrase.size() < 1) || (phrase.size() > PHRASE_MAX) || 0 != std::count_if(phrase.begin(), phrase.end(), iscntrl)) {
			Trace(ZONE_WARNING, "Invalid wlan passphrase '%s'\n", phrase.data());
			return false;
		}

		if((ssid.size() < 1) || (ssid.size() > SSID_MAX)) {
			Trace(ZONE_WARNING, "Invalid wlan ssid '%s'\n", ssid.data());
			return false;
		}

		if(!mTelnet.Open()) {
			Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}

		if(!mTelnet.SendString("set wlan phrase ", phrase)) {
			Trace(ZONE_ERROR, "set wlan phrase to '%s' failed\n", phrase.data());
			mTelnet.Close(false);
			return false;
		}

		if(!mTelnet.SendString("set wlan ssid ", ssid)) {
				Trace(ZONE_ERROR, "set wlan ssid to '%s' failed\n", ssid.data());
			mTelnet.Close(false);
			return false;
		}
		return mTelnet.Close(true);
	}

	bool ConfigControl::SetDeviceId(const std::string& name) const
	{
		static const size_t NAME_MAX_LEN = 32;

		if((name.size() < 1) || (name.size() > NAME_MAX_LEN)) {
				Trace(ZONE_WARNING, "Invalid device name '%s'\n", name.data());
			return false;
		}

		if(!mTelnet.Open()) {
				Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}

		if(!mTelnet.SendString("set opt deviceid ", name)) {
				Trace(ZONE_ERROR, "set device name to '%s' failed\n", name.data());
			mTelnet.Close(false);
			return false;
		}

		return mTelnet.Close(true);
	}

	bool ConfigControl::RebootWlanModule(void) const
	{
		if(!mTelnet.Open()) {
				Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}

		static const std::string command = "reboot";

		if(!mTelnet.SendRebootCommand()) {
				Trace(ZONE_ERROR, "send reboot command failed\n");
			mTelnet.Close(false);
			return false;
		}
		return true;
	}

	bool ConfigControl::ChangeWlanChannel(void) const
	{
		if(!mTelnet.Open()) {
				Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}
		std::string result;
		if(!mTelnet.PerformWifiScan(result)) {
				Trace(ZONE_ERROR, "wifi scan failed\n");
			return mTelnet.Close(false);
		}

		unsigned int newChannel = mTelnet.ComputeFreeChannel(result);

		if(!newChannel) {
				Trace(ZONE_ERROR, "compute new channel failed\n");
			return mTelnet.Close(false);
		}
		mTelnet.ChangeWifiChannel(newChannel);
		return mTelnet.Close(false);
	}

	bool ConfigControl::GetSoftAp(void) const
	{
		std::string result {};
		if(mTelnet.Open()) {
			mTelnet.RecvString("get wlan\r\n", "Join=", result);
			mTelnet.Close(false);
		}
		return (0 == result.compare("7"));
	}

	std::string ConfigControl::Get(const std::string& searchKey, const std::string& getCmd) const
	{
		std::string result {};
		if(mTelnet.Open()) {
			mTelnet.RecvString(getCmd, searchKey, result);
			mTelnet.Close(false);
		}
		return result;
	}

	std::string ConfigControl::GetDeviceId(void) const
	{
		return Get("DeviceId=", "get opt\r\n");
	}

	std::string ConfigControl::GetPassphrase(void) const
	{
		return Get("Passphrase=");
	}

	std::string ConfigControl::GetSsid(void) const
	{
		return Get("SSID=");
	}

	bool ConfigControl::ModuleAsSoftAP(const std::string& accesspointName) const
	{
		if(!FactoryReset()) {
			Trace(ZONE_ERROR, "factory RESET failed\n");
			return false;
		}

		if(!SetDeviceId(accesspointName)) {
				Trace(ZONE_ERROR, "set DeviceId failed\n");
			return false;
		}

		static const size_t SSID_MAX = 32;

		if((accesspointName.size() < 1) || (accesspointName.size() > SSID_MAX)) {
				Trace(ZONE_WARNING, "Invalid wlan ssid '%s'\n", accesspointName.data());
			return false;
		}

		if(!mTelnet.Open()) {
				Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}

		if(!mTelnet.SendString("set wlan ssid ", accesspointName)) {
				Trace(ZONE_ERROR, "set wlan ssid to '%s' failed\n", accesspointName.data());
			mTelnet.Close(false);
			return false;
		}

		if(!mTelnet.Close(true)) {
				Trace(ZONE_ERROR, "save changes failed\n");
			return false;
		}

		if(!SetParameters(RN171_SOFT_AP_DEFAULT_PARAMETERS)) {
				Trace(ZONE_ERROR, "set defaults failed\n");
			return false;
		}

		return RebootWlanModule();
	}
} /* namespace WyLight */
