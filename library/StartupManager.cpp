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

#include "StartupManager.h"
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

namespace WyLight {
	
	static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;
	
	// trim from start
	static inline std::string &ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}
	
	// trim from end
	static inline std::string &rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}
	
	// trim from both ends
	static inline std::string &trim(std::string &s) {
		return ltrim(rtrim(s));
	}
	
	Version::Version(const std::string& versionString) {
		std::string trimmedString = versionString;
		trimmedString = trim(trimmedString);
		std::istringstream stream(trimmedString);
		std::string subStringMajor, subStringMinor;
		std::getline(stream, subStringMajor, '.');
		std::getline(stream, subStringMinor, '.');
		
		mMajor = std::stoi(subStringMajor);
		mMinor = std::stoi(subStringMinor);
	}
	
	Version::Version(const unsigned int& major, const unsigned int& minor) : mMajor(major), mMinor(minor) {}
	
	unsigned int Version::getMajor() const { return mMajor; }
	
	unsigned int Version::getMinor() const { return mMinor; }
	
	bool operator == (const Version& v1, const Version& v2) {
		return v1.mMinor == v2.mMinor && v1.mMajor == v2.mMajor;
	}
	
	bool operator != (const Version& v1, const Version& v2) {
		return !(v1 == v2);
	}
	
	bool operator > (const Version& v1, const Version& v2) {
		return v1.mMajor > v2.mMajor ? true : v1.mMinor > v2.mMinor;
	}
	
	bool operator >= (const Version& v1, const Version& v2) {
		return v1.mMajor >= v2.mMajor ? true : v1.mMinor >= v2.mMinor;
	}
	
	bool operator < (const Version& v1, const Version& v2) {
		return v1.mMajor < v2.mMajor ? true : v1.mMinor < v2.mMinor;
	}
	
	bool operator <= (const Version& v1, const Version& v2) {
		return v1.mMajor <= v2.mMajor ? true : v1.mMinor <= v2.mMinor;
	}
	
	std::ostream& operator << (std::ostream& out, const Version& ref) {
		return out << ref.mMajor << '.' << ref.mMinor;
	}
	
	UpdateExtension::UpdateExtension(const updateCheckFunction& updateNecessary, const updateFunction& updateFunction) : mUpdateNecessary(updateNecessary), mUpdateFunction(updateFunction) {}
	
	UpdateExtension::UpdateExtension(const UpdateExtension& other) : mUpdateNecessary(other.mUpdateNecessary), mUpdateFunction(other.mUpdateFunction) {}
	
	void UpdateExtension::run(const WyLight::Version &currentVersionOfTarget, const WyLight::Version &newVersionOfTarget, const WyLight::Control &ctrlForUpdate) const {
		if (mUpdateNecessary(currentVersionOfTarget, newVersionOfTarget)) {
			mUpdateFunction(ctrlForUpdate);
		}
	}
	
	const UpdateExtension StartupManager::eepromClear(
		[](const Version& currentVersion, const Version& newVersion) -> bool { return true; },
		[](const Control& ctrl){ ctrl.BlEraseEeprom(); });
	
	const UpdateExtension StartupManager::setUdpInRN171(
		[](const Version& currentVersion, const Version& newVersion) -> bool
		  {return currentVersion < Version(0,5) && newVersion >= Version(0,5); },
		[](const Control& ctrl)
		  { std::list<std::string> commands = {"set i p 11\r\n"}; ctrl.ConfSetParameters(commands); });
	
	StartupManager::StartupManager(const std::function<void(size_t newState)>& onStateChange) : mOnStateChangeCallback(onStateChange) {}
	
	void StartupManager::setCurrentState(StartupManager::State newState) {
		if (mState != newState) {
			if (mOnStateChangeCallback) {
				mOnStateChangeCallback(newState);
			}
		mState = newState;
		}
	}
	
	void StartupManager::startup(WyLight::ControlNoThrow &control, const std::string &hexFilePath) throw (InvalidParameter)
	{
		this->startup(control.mControl, hexFilePath);
	}
	
	void StartupManager::startup(WyLight::Control& control, const std::string& hexFilePath) throw (InvalidParameter)
	{
		try {
			mHexFileVersion = Version(control.ExtractFwVersion(hexFilePath));
		} catch (std::exception &e) {
			throw InvalidParameter("Can not read version string from hexFile");
			return;
		}
		
		setCurrentState(MODE_CHECK);
		
		while (mState != STARTUP_FAILURE || mState != STARTUP_SUCCESSFUL) {
			switch (mState) {
				case MODE_CHECK: {
					try {
						size_t currentMode = control.GetTargetMode();
						if (currentMode == BL_IDENT) {
							setCurrentState(BL_VERSION_CHECK);
						} else if (currentMode == FW_IDENT) {
							setCurrentState(FW_VERSION_CHECK);
						} else {
							setCurrentState(START_BOOTLOADER);
						}
					} catch (std::exception &e) {
						Trace(ZONE_ERROR, "StartupManager startup failure in state %d: %s\n", mState, e.what());
						setCurrentState(START_BOOTLOADER);
					}
				} break;
				case START_BOOTLOADER: {
					try {
						control << FwCmdStartBl();
						setCurrentState(MODE_CHECK);
					} catch (std::exception &e) {
						Trace(ZONE_ERROR, "StartupManager startup failure in state %d: %s\n", mState, e.what());
						setCurrentState(STARTUP_FAILURE);
					}
				} break;
				case FW_VERSION_CHECK: {
					try {
						mTargetVersion = Version(control.FwGetVersion());
						if (mTargetVersion != Version(0,0) && mTargetVersion == mHexFileVersion) {
							setCurrentState(STARTUP_SUCCESSFUL);
						} else {
							setCurrentState(START_BOOTLOADER);
						}
					} catch (std::exception &e) {
						Trace(ZONE_ERROR, "StartupManager startup failure in state %d: %s\n", mState, e.what());
						setCurrentState(START_BOOTLOADER);
					}
				} break;
				case BL_VERSION_CHECK: {
					try {
						mTargetVersion = Version(control.BlReadFwVersion());
						if (mTargetVersion != Version(0,0) && mTargetVersion == mHexFileVersion) {
							setCurrentState(RUN_APP);
						} else {
							setCurrentState(UPDATING);
						}
					} catch (std::exception &e) {
						Trace(ZONE_ERROR, "StartupManager startup failure in state %d: %s\n", mState, e.what());
						setCurrentState(STARTUP_FAILURE);
					}
				} break;
				case UPDATING: {
					try {
						control.BlProgramFlash(hexFilePath);
						for (const UpdateExtension& func : mUpdateTaskSet) {
							func.run(mTargetVersion, mHexFileVersion, control);
						}
						setCurrentState(RUN_APP);
					} catch (std::exception &e) {
						Trace(ZONE_ERROR, "StartupManager startup failure in state %d: %s\n", mState, e.what());
						setCurrentState(STARTUP_FAILURE);
					}
				} break;
				case RUN_APP: {
					try {
						control.BlRunApp();
						setCurrentState(STARTUP_SUCCESSFUL);
					} catch (std::exception &e) {
						Trace(ZONE_ERROR, "StartupManager startup failure in state %d: %s\n", mState, e.what());
						setCurrentState(STARTUP_FAILURE);
					}
				} break;
				case STARTUP_FAILURE: {
					return;
				} break;
				case STARTUP_SUCCESSFUL: {
					return;
				} break;
				default: break;
			}
		}
		
	}
	
	
	
	
	
	
}
