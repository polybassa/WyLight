/*
 Copyright (C) 2013 Nils Weiss, Patrick Bruenn.
 
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

#ifndef __WyLight__MessageQueue__
#define __WyLight__MessageQueue__

#include <iostream>
#include <condition_variable>
#include <thread>
#include <queue>

namespace WyLight {

template <typename T>
class MessageQueue {
	
	std::deque<T> mQueue;
	std::mutex mMutex;
	std::condition_variable mCondVar;
	
public:
	MessageQueue(void) {};
	MessageQueue(MessageQueue const&& other) : mQueue(other.mQueue), mMutex(other.mMutex), mCondVar(other.mCondVar) {};
	
	void send(const T&& message);
	
	void sendDirect(const T&& message);
	
	void clear(void);
	
	void sendOnlyThis(const T&& message);
	
	T receive(void);
};
}

template <typename T>
void WyLight::MessageQueue<T>::send(const T&& message)
{
	{
		std::lock_guard<std::mutex> lg(this->mMutex);
		this->mQueue.push_back(std::move(message));
	}
	this->mCondVar.notify_one();
}

template <typename T>
void WyLight::MessageQueue<T>::sendDirect(const T&& message)
{
	{
		std::lock_guard<std::mutex> lg(this->mMutex);
		this->mQueue.push_front(std::move(message));
	}
	this->mCondVar.notify_one();
}

template <typename T>
void WyLight::MessageQueue<T>::clear(void)
{
	std::lock_guard<std::mutex> lg(this->mMutex);
	this->mQueue.clear();
}

template <typename T>
void WyLight::MessageQueue<T>::sendOnlyThis(const T&& message)
{
	this->clear();
	this->sendDirect(std::move(message));
}

template <typename T>
T WyLight::MessageQueue<T>::receive(void)
{
	std::unique_lock<std::mutex> ul(this->mMutex);
	this->mCondVar.wait(ul, [this]{ return !this->mQueue.empty(); });
	T message = std::move(this->mQueue.front());
	this->mQueue.pop_front();
	return message;
}

#endif /* defined(__WyLight__MessageQueue__) */
