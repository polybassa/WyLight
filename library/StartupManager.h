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
#include <functional>
#include <list>
#include "WiflyControl.h"

namespace WyLight {
    
    
    class Version {
    public:
        Version(const std::string& versionString);
        Version(const unsigned int& major, const unsigned int& minor);
        Version(const Version& other) : mMajor(other.mMajor), mMinor(other.mMinor) {}
        Version(Version&& other) : mMajor(std::move(other.mMajor)), mMinor(std::move(other.mMinor)) {}
        Version& operator = (const Version & other) { mMajor = other.mMajor; mMinor = other.mMinor; return *this; }
        
        unsigned int getMajor(void) const;
        unsigned int getMinor(void) const;
        
        friend bool operator == (const Version& v1, const Version& v2);
        friend bool operator != (const Version& v1, const Version& v2);
        friend bool operator > (const Version& v1, const Version& v2);
        friend bool operator >= (const Version& v1, const Version& v2);
        friend bool operator < (const Version& v1, const Version& v2);
        friend bool operator <= (const Version& v1, const Version& v2);
        friend std::ostream& operator << (std::ostream& out, const Version& ref);
        
    private:
        unsigned int mMajor = 0;
        unsigned int mMinor = 0;
    };
    
    class UpdateExtension {
    public:
        typedef std::function<bool(const Version& currentVersionOfTarget, const Version& newVersionOfTarget)> updateCheckFunction;
        typedef std::function<void(const Control& ctrlForUpdate)> updateFunction;
        
        UpdateExtension(const updateCheckFunction& updateNecessary = NULL,
                        const updateFunction& updateFunction = NULL);
        UpdateExtension(const UpdateExtension& other);
        
        void run(const Version& currentVersionOfTarget,
                 const Version& newVersionOfTarget,
                 const Control& ctrlForUpdate) const;
        
        friend struct std::hash<UpdateExtension>;
    private:
        const updateCheckFunction mUpdateNecessary;
        const updateFunction mUpdateFunction;
    };

    class StartupManager {
    public:
        static const UpdateExtension eepromClear;
        static const UpdateExtension setUdpInRN171;
        
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
        
    private:
        std::function<void(size_t newState)> mOnStateChangeCallback;
        StartupManager::State mState = MODE_CHECK;
        Version mHexFileVersion = Version(0,0);
        Version mTargetVersion = Version(0,0);
        std::list<UpdateExtension> mUpdateTaskSet = {eepromClear, setUdpInRN171};
        
        void setCurrentState(StartupManager::State newState);
    };
    
}

#endif /* defined(__WiflyControlCli__StartupManager__) */
