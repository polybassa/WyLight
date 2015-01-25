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

#include "CPPTask.h"

Task::Task(const char *name, unsigned short stackSize,unsigned long priority, std::function<void (const bool&)> function) : mTaskFunction(function) {
	osi_SyncObjCreate(&this->mStartSemaphore);
	osi_SyncObjCreate(&this->mStopSemaphore);
	
	this->mStopFlag = false;
	osi_TaskCreate(Task::task, (signed portCHAR *) name, stackSize, this, priority, &this->mHandle);
}

void Task::run(void) {
	osi_SyncObjSignal(&this->mStartSemaphore);
}

void Task::stop(void) {
	this->mStopFlag = true;
	osi_SyncObjWait(&this->mStopSemaphore, OSI_WAIT_FOREVER);
}

void Task::task(void * pvParameters) {
	static_cast<Task*>(pvParameters)->taskFunction();
}

void Task::taskFunction(void) {
	osi_Sleep(200);
	for (;;) {
		osi_SyncObjWait(&this->mStartSemaphore, OSI_WAIT_FOREVER);
		this->mStopFlag = false;
		mTaskFunction(this->mStopFlag);
		osi_SyncObjSignal(&this->mStopSemaphore);
	}
	
}