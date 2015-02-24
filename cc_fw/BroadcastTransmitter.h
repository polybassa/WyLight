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

#ifndef __BROADCAST_TRANSMITTER_H_
#define __BROADCAST_TRANSMITTER_H_

#include <stdint.h>
#include "CPPTask.h"
#include "SimplelinkCustomer.h"
#include "BroadcastMessage.h"

class BroadcastTransmitter final : public Task, SimplelinkCustomer {
    static const uint16_t port = 55555;

public:
    BroadcastTransmitter(void);
    BroadcastTransmitter(const BroadcastTransmitter&) = delete;
    BroadcastTransmitter& operator=(const BroadcastTransmitter&) = delete;
    BroadcastTransmitter(BroadcastTransmitter&&) = delete;

    virtual void run(void);
    virtual void stop(void);
};

#endif