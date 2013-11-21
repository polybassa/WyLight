//
//  StartupManager.cpp
//  WiflyControlCli
//
//  Created by Nils Weiß on 20/11/13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#include "StartupManager.h"
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

namespace WyLight {
    
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
    
    Version::Version(const unsigned int& major, const unsigned int& minor) : mMajor(major), mMinor(minor) {};
    
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
    
    UpdateExtension::UpdateExtension(const updateCheckFunction& updateNecessary, const updateFunction& updateFunction)
            : mUpdateNecessary(updateNecessary), mUpdateFunction(updateFunction) {}
    
    void UpdateExtension::run(const WyLight::Version &currentVersionOfTarget,
                              const WyLight::Version &newVersionOfTarget,
                              const WyLight::Control &ctrlForUpdate) const {
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
    
    StartupManager::StartupManager(const Control& ctrl, const std::string& hexFilePath, const std::function<void(size_t newState)>& onStateChange) throw (InvalidParameter) : mControl(ctrl), mOnStateChangeCallback(onStateChange) {
        try {
            mHexFileVersion = Version(mControl.ExtractFwVersion(hexFilePath));
        } catch (std::exception &e) {
            throw InvalidParameter("Can not read version string from hexFile");
        }
    }
    
    void StartupManager::setCurrentState(StartupManager::State newState) {
        if (mOnStateChangeCallback) {
            mOnStateChangeCallback(newState);
        }
        mState = newState;
    }
    
    void StartupManager::startup() {
        while (mState != STARTUP_FAILURE || mState != STARTUP_SUCCESSFUL) {
            switch (mState) {
                case MODE_CHECK: {
                    try {
                        size_t currentMode = mControl.GetTargetMode();
                        if (currentMode == BL_IDENT) {
                            setCurrentState(BL_VERSION_CHECK);
                        } else if (currentMode == FW_IDENT) {
                            setCurrentState(FW_VERSION_CHECK);
                        } else {
                            setCurrentState(START_BOOTLOADER);
                        }
                    } catch (std::exception) {
                        setCurrentState(START_BOOTLOADER);
                    }
                } break;
                case START_BOOTLOADER: {
                    try {
                        mControl << FwCmdStartBl();
                        setCurrentState(MODE_CHECK);
                    } catch (std::exception) {
                        setCurrentState(STARTUP_FAILURE);
                    }
                } break;
                    
                default:
                    break;
            }
        }
        
    }
    
    
    
    
    
    
}