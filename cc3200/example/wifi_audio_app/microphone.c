//*****************************************************************************
// Microphone.c
//
// Line IN (Microphone interface)
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
#include "network.h"
#include "circ_buff.h"
#include "simplelink.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include "hw_ints.h"
//******************************************************************************
//							GLOBAL VARIABLES
//******************************************************************************

extern unsigned int clientIP;
extern tCircularBuffer *pTxBuffer;
extern tUDPSocket g_UdpSock;
int g_iSentCount =0;
unsigned long g_ulConnected = 0; 

extern tCircularBuffer *pRxBuffer;
extern int g_iReceiveCount;
extern unsigned int g_uiPlayWaterMark;
extern unsigned int g_uiIpObtained; 
extern volatile unsigned char g_ucMicStartFlag;
extern unsigned char speaker_data[16*1024];

#ifdef MULTICAST
//*****************************************************************************
//
//! Send Multicast Packet 
//!
//! \param none
//!
//! \return None
//
//*****************************************************************************
void SendMulticastPacket()
{
    int uiPort = 5050;
    struct sockaddr_in stClient;
    stClient.sin_family = AF_INET;
    stClient.sin_addr.s_addr = htonl(ADDR_MULTICAST_GROUP);
    stClient.sin_port = htons(uiPort);
  
    sendto(g_UdpSock.iSockDesc, (char*)(pTxBuffer->pucReadPtr),PACKET_SIZE,
        0,(struct sockaddr*)&(stClient),sizeof(stClient));	
}
#endif

//*****************************************************************************
//
//! Microphone Routine 
//!
//! \param pvParameters     Parameters to the task's entry function
//!
//! \return None
//
//*****************************************************************************

void Microphone( void *pvParameters )
{
#ifdef NETWORK
#ifdef MULTICAST
    //Wait for Network Connection
    while(g_uiIpObtained == 0)
    {
        
    }
#else
    //Wait for Network Connection and Speaker Discovery
    while(g_uiIpObtained == 0 || g_UdpSock.Client.sin_addr.s_addr==INVALID_CLIENT_ADDRESS)
    {
        
    }
#endif //MULTICAST
#endif //NETWORK
   
    while(1)
    {     
        while(g_ucMicStartFlag)
        {
            int iBufferFilled = 0;
            iBufferFilled = GetBufferSize(pTxBuffer);
            if(iBufferFilled >= (2*PACKET_SIZE))
            { 
#ifdef NETWORK

#ifndef MULTICAST          
                sendto(g_UdpSock.iSockDesc, (char*)(pTxBuffer->pucReadPtr),PACKET_SIZE,
                0,(struct sockaddr*)&(g_UdpSock.Client),sizeof(g_UdpSock.Client));
#else      //MULTICAST         
                SendMulticastPacket();
#endif     //MULTICAST      
#else   //NETWORK       
                FillBuffer(pRxBuffer,(unsigned char*)(pTxBuffer->pucReadPtr), PACKET_SIZE);
                if(g_uiPlayWaterMark == 0)
                {
                    if(IsBufferSizeFilled(pRxBuffer,PLAY_WATERMARK) == TRUE)
                    {                    
                        g_uiPlayWaterMark = 1;
                    }
                }      
                g_iReceiveCount++;

#endif   //NETWORK       
                 UpdateReadPtr(pTxBuffer, PACKET_SIZE);
                 g_iSentCount++;
            }

        }      
    MAP_UtilsDelay(1000);
    }
}
