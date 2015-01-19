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

#ifndef _CONFIGCONTROL_H_
#define _CONFIGCONTROL_H_

#include "TelnetProxy.h"
#include <list>
#include <string>

namespace WyLight
{
	struct ConfigControl
	{
		static const std::list<std::string> RN171_DEFAULT_PARAMETERS;
		static const std::list<std::string> RN171_BASIC_PARAMETERS;
		static const std::list<std::string> RN171_SOFT_AP_DEFAULT_PARAMETERS;
		static const std::list<std::string> RN171_FACTORY_RESET_PARAMETER;

		ConfigControl(const TelnetProxy &telnet);

		/**
		 * Read the device id from WyLight module
		 * @return an empty string or the device id
		 */
		std::string GetDeviceId(void) const;

		/**
		 * Read the currently configured wlan passphrase from WyLight module
		 * @return an empty string or the wlan passphrase
		 */
		std::string GetPassphrase(void) const;

		/**
		 * Read the currently configured mode(client/SoftAP) from WyLight module
		 * @return true if WyLight module is configured for SoftAP mode
		 */
		bool GetSoftAp(void) const;

		/**
		 * Read the currently configured ssid from WyLight module
		 * @return an empty string or the ssid
		 */
		std::string GetSsid(void) const;

		/**
		 * Configurates the WyLight module as stand alone accesspoint. With accesspoint name you can change the ssid for this accesspoint.
		 * @param accesspointName 1 - 32 characters
		 * @return false, in case of an error
		 */
		bool ModuleAsSoftAP(const std::string& accesspointName = "Wifly_Light") const;

		/**
		 * Configurates the WyLight module as client for an existing wlan network with WPA2 protection
		 * @param phrase WPA2 passphrase 1 - 63 characters
		 * @param ssid 1 - 32 characters
		 * @param deviceId 1 - 32 characters, unique name which apperas in the broadcast message
		 * @return false, in case of an error
		 */
		bool ModuleForWlan(const std::string& phrase, const std::string& ssid, const std::string& deviceId = "Wifly_Light") const;

		/**
		 * Reboot the modul. ATTENTION: You have to reconnect after a reboot
		 * @return false, in case of an error
		 */
		bool RebootWlanModule(void) const;

		/**
		 * Allows you to give every Wifly_Light device an unique name
		 * @param name 1 - 32 characters
		 * @return false, in case of an error
		 */
		bool SetDeviceId(const std::string& name) const;

		/**
		 * Wlan module performs a wifi scan and changes to the next free channel. This function can take some time.
		 * @return false, in case of an error
		 */
		bool ChangeWlanChannel(void) const;

		/**
		 *
		 */
		bool SetParameters(std::list<std::string> commands) const;

		/**
		 * Set the WyLight module communication parameters to defaults
		 * @return false, in case of an error
		 */
		bool SetDefaults(void) const;

		/**
		 * Set the WyLight module wlan connection parameters
		 * @param phrase WPA2 passphrase 1 - 63 characters
		 * @param ssid 1 - 32 characters
		 * @return false, in case of an error
		 */
		bool SetWlan(const std::string& phrase, const std::string& ssid) const;

		bool FactoryReset(void) const;

	private:
		const TelnetProxy &mTelnet;

		/**
		 * Read the currently configured wlan passphrase from WyLight module
		 * @param searchKey to search the wlan settings for
		 * @return an empty string or the value of the specified wlan setting
		 */
		std::string Get(const std::string& searchKey, const std::string& getCmd = "get wlan\r\n") const;
	};
}
#endif /* #ifndef _CONFIGCONTROL_H_ */
