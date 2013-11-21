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
#include <unordered_set>
#include "WiflyControl.h"

namespace WyLight {
    
    class Version {
    public:
        Version(const std::string& versionString);
        Version(const unsigned int& major, const unsigned int& minor);
        Version(const Version& other) : mMinor(other.mMinor), mMajor(other.mMajor) {}
        Version(Version&& other) : mMinor(std::move(other.mMinor)), mMajor(std::move(other.mMajor)) {}
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
        UpdateExtension(const UpdateExtension& other) = delete;
        UpdateExtension(UpdateExtension&& other) = delete;
        
        void run(const Version& currentVersionOfTarget,
                 const Version& newVersionOfTarget,
                 const Control& ctrlForUpdate) const;
    private:
        const updateCheckFunction mUpdateNecessary;
        const updateFunction mUpdateFunction;
    };
    
    class StartupManager {
    public:
        static const UpdateExtension eepromClear;
        static const UpdateExtension setUdpInRN171;
        
        StartupManager(const Control& ctrl, const std::string& hexFilePath = "", const std::function<void(size_t newState)>& onStateChange = NULL) throw (InvalidParameter);
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
        void startup(void);
        
    private:
        Control mControl;
        std::function<void(size_t newState)> mOnStateChangeCallback;
        StartupManager::State mState = MODE_CHECK;
        Version mHexFileVersion = Version(0,0);
        std::unordered_set<UpdateExtension> mUpdateTaskSet = {eepromClear, setUdpInRN171};
        
        void setCurrentState(StartupManager::State newState);
    };
    
}

#endif /* defined(__WiflyControlCli__StartupManager__) */
