/*
 * BroadcastTransmitter.h
 *
 *  Created on: 12.08.2014
 *      Author: nw
 */

#ifndef BROADCASTTRANSMITTER_H_
#define BROADCASTTRANSMITTER_H_

void BroadcastTransmitter_init(void);
int SendBroadcastMessage(void);

unsigned long g_ulTimerInterrupt;

#endif /* BROADCASTTRANSMITTER_H_ */
