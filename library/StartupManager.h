/*
 Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.

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

#ifndef __WiflyControlCli__StartupManager__
#define __WiflyControlCli__StartupManager__

#include <iostream>
#include "WiflyControl.h"
#include "WiflyControlNoThrow.h"

namespace WyLight {

	class StartupManager {
	public:
		StartupManager(const std::function<void(size_t newState)>& onStateChange = NULL);
		StartupManager(const StartupManager& other) = delete;
		StartupManager(StartupManager&& other) = delete;

		enum State {
			MODE_CHECK = 0,
			START_BOOTLOADER,
			BL_VERSION_CHECK,
			FW_VERSION_CHECK,
			UPDATING,
			RUN_APP,
			STARTUP_FAILURE,
			STARTUP_SUCCESSFUL
		};

		StartupManager::State getCurrentState(void) const {return mState; }
		void startup(WyLight::Control& control, const std::string& hexFilePath) throw (InvalidParameter);
		void startup(WyLight::ControlNoThrow& control, const std::string& hexFilePath) throw (InvalidParameter);

	private:
		std::function<void(size_t newState)> mOnStateChangeCallback;
		StartupManager::State mState = MODE_CHECK;
		uint16_t mHexFileVersion = 0;
		uint16_t mTargetVersion = 0;

		void setCurrentState(StartupManager::State newState);
		void bootloaderVersionCheckUpdate(WyLight::Control& control, const std::string& hexFilePath);
		void startBootloader(WyLight::Control& control, const std::string& hexFilePath);
	};

}

#endif /* defined(__WiflyControlCli__StartupManager__) */
