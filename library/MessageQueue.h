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
	size_t mMessageLimit = 0;

	public:
		MessageQueue(void) {};
		MessageQueue(MessageQueue const&& other) : mQueue(std::move(other.mQueue)), mMutex(std::move(other.mMutex)), mCondVar(std::move(other.mCondVar)) {};

		void push_back(const T&& message);

		void push_front(const T&& message);

		void clear(void);

		void clear_and_push_front(const T&& message);

		bool empty(void) const;

		void setMessageLimit(size_t limit) { (limit < 5) ? mMessageLimit = 5 : mMessageLimit = limit; };

		T receive(void);
	};
}

template <typename T>
void WyLight::MessageQueue<T>::push_back(const T&& message)
{
	{
		std::lock_guard<std::mutex> lg(this->mMutex);
		if(mMessageLimit == 0 || (mMessageLimit > this->mQueue.size())) {
			this->mQueue.push_back(std::move(message));
		} else {
			this->mQueue.erase(++(this->mQueue.begin())); //delete the secound element in the queue
			this->mQueue.push_back(std::move(message));
		}
	}
	this->mCondVar.notify_one();
}

template <typename T>
void WyLight::MessageQueue<T>::push_front(const T&& message)
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
void WyLight::MessageQueue<T>::clear_and_push_front(const T&& message)
{
	this->clear();
	this->push_front(std::move(message));
}

template <typename T>
bool WyLight::MessageQueue<T>::empty(void) const
{
	std::lock_guard<std::mutex> lg(this->mMutex);
	return this->mQueue.empty();
}

template <typename T>
T WyLight::MessageQueue<T>::receive(void)
{
	std::unique_lock<std::mutex> ul(this->mMutex);
	this->mCondVar.wait(ul, [this] { return !this->mQueue.empty(); }
			    );
	T message = std::move(this->mQueue.front());
	this->mQueue.pop_front();
	return message;
}

#endif /* defined(__WyLight__MessageQueue__) */
