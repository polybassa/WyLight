//*****************************************************************************
// network_if.h
//
// Networking interface macro and function prototypes for CC3200 device
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

#ifndef __WY_BL_NETWORK_IF__H__
#define __WY_BL_NETWORK_IF__H__

#include "wlan.h"

#ifdef __cplusplus
extern "C" {
#endif

// Loop forever, user can change it as per application's requirement
#define LOOP_FOREVER(line_number) \
            {\
                while(1); \
            }

// check the error code and handle it
#define ASSERT_ON_ERROR(line_number, error_code) \
            {\
				if (error_code < 0){ \
					UART_PRINT("%d: Assert Error: %d\r\n", line_number, error_code); \
					return error_code;\
				} \
            }

#define SUCCESS         		0
#define SL_STOP_TIMEOUT         30

#ifdef NOTERM
#define UART_PRINT
#define DBG_PRINT
#else
#define UART_PRINT 				Report
#define DBG_PRINT  				Report
#endif

#define SET_STATUS_BIT(status_variable, bit) 	status_variable |= (1<<(bit))
#define CLR_STATUS_BIT(status_variable, bit) 	status_variable &= ~(1<<(bit))
#define CLR_STATUS_BIT_ALL(status_variable)   	(status_variable = 0)
#define GET_STATUS_BIT(status_variable, bit) 	(0 != (status_variable & (1<<(bit))))

#define IS_NW_PROCSR_ON(status_variable)     	GET_STATUS_BIT(status_variable,\
                                                            STATUS_BIT_NWP_INIT)
#define IS_CONNECTED(status_variable)        	GET_STATUS_BIT(status_variable,\
                                                         STATUS_BIT_CONNECTION)
#define IS_IP_LEASED(status_variable)        	GET_STATUS_BIT(status_variable,\
                                                           STATUS_BIT_IP_LEASED)
#define IS_IP_ACQUIRED(status_variable)       	GET_STATUS_BIT(status_variable,\
                                                          STATUS_BIT_IP_AQUIRED)
#define IS_CONNECT_FAILED(status_variable)   	GET_STATUS_BIT(status_variable,\
                                                   STATUS_BIT_CONNECTION_FAILED)
// Status bits - These are used to set/reset the corresponding bits in 
// given variable

typedef enum {
	STATUS_BIT_NWP_INIT = 0, // If this bit is set: Network Processor is
							 // powered up

	STATUS_BIT_CONNECTION,   // If this bit is set: the device is connected to
							 // the AP or client is connected to device (AP)

	STATUS_BIT_IP_LEASED,    // If this bit is set: the device has leased IP to
							 // any connected client

	STATUS_BIT_IP_AQUIRED,   // If this bit is set: the device has acquired an IP

	STATUS_BIT_SMARTCONFIG_START, // If this bit is set: the SmartConfiguration
								  // process is started from SmartConfig app

	STATUS_BIT_P2P_DEV_FOUND,    // If this bit is set: the device (P2P mode)
								 // found any p2p-device in scan

	STATUS_BIT_P2P_REQ_RECEIVED, // If this bit is set: the device (P2P mode)
								 // found any p2p-negotiation request

	STATUS_BIT_CONNECTION_FAILED, // If this bit is set: the device(P2P mode)
								  // connection to client(or reverse way) is failed

	STATUS_BIT_PING_DONE         // If this bit is set: the device has completed
								 // the ping operation

} e_StatusBits;

//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern void Network_IF_InitDriver(void);
extern void Network_IF_DeInitDriver(void);
extern long Network_IF_StartSimpleLinkAsAP(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif
#endif //  __MCU_COMMON_H__
