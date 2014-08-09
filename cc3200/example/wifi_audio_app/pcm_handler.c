//*****************************************************************************
// pcm_handler.c
//
// PCM Handler Interface
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

//*****************************************************************************
//
//! \addtogroup WifiAudioPlayer
//! @{
//
//*****************************************************************************
/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* Scheduler includes. */
#ifdef ewarm
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#endif

/* Hardware & DriverLib library includes. */
#include "hw_types.h"
#include "hw_mcasp.h"
#include "hw_udma.h"
#include "hw_ints.h"
#include "rom.h"
#include "rom_map.h"
#include "debug.h"
#include "interrupt.h"
#include "hw_memmap.h"
#include "i2s.h"
#include "udma.h"
#include "pin.h"
#include "mcasp_if.h"
#include "circ_buff.h"
#include "pcm_handler.h"
#include "udma_if.h"
#include "i2c_if.h"
#include "uart_if.h"

#ifndef TI_CODEC
#include "wolfson.h"
#else
#include "ti_codec.h"
#endif
//*****************************************************************************
//                          LOCAL DEFINES
//*****************************************************************************
#define UDMA_DSTSZ_32_SRCSZ_16          0x21000000
#define mainQUEUE_SIZE		        3

//*****************************************************************************
//                          GLOBAL VARIABLES
//*****************************************************************************
unsigned char gaucZeroBuffer[(CB_TRANSFER_SZ * HALF_WORD_SIZE)];
volatile unsigned int *puiTxSrcBuf;
volatile unsigned short *pusTxDestBuf;
volatile unsigned short *pusRxSrcBuf;
volatile unsigned int *puiRxDestBuf;
volatile unsigned int guiDMATransferCountTx = 0, guiDMACount = 0;
volatile unsigned int guiDMATransferCountRx = 0;
unsigned int g_uiBuffLevel=0;
unsigned int g_uiNoDataLeft;
extern tCircularBuffer *pTxBuffer;
extern tCircularBuffer *pRxBuffer;
extern unsigned int g_uiPlayWaterMark;
int g_iReadFlag = 0;
unsigned int guiDMAEmptyCount = 0;
int iCount1,SPEAKER,MIC,iCount3;
//*****************************************************************************
//
//! Callback function implementing ping pong mode DMA transfer
//!
//! \param None
//!
//! This function
//!        1. sets the primary and the secondary buffer for ping pong transfer.
//!		   2. sets the control table.
//!		   3. transfer the appropriate data according to the flags.
//!
//! \return None.
//
//*****************************************************************************
void DMAPingPongCompleteAppCB_opt()
{
    unsigned long ulPrimaryIndexTx = 0x4, ulAltIndexTx = 0x24;
    unsigned long ulPrimaryIndexRx = 0x5, ulAltIndexRx = 0x25;
    tDMAControlTable *pControlTable;
    tCircularBuffer *pAudOutBuf = pRxBuffer;
    tCircularBuffer *pAudInBuf = pTxBuffer;
    unsigned int uiBufferEmpty = 0;
    unsigned char *pucDMADest;
    unsigned char *pucDMASrc;

    if(uDMAIntStatus() & 0x00000010)
    {
        HWREG(0x4402609c) = (1<<10);
        guiDMACount++;
        if(!(guiDMACount & 0x000003FF))
        {
            //DBG_PRINT("DMACount = %u\n\r",guiDMACount);
        }
        //
        // Get the base address of the control table.
        //
        //pControlTable = (tDMAControlTable *)HWREG(DMA_BASE + UDMA_O_CTLBASE);
        pControlTable = MAP_uDMAControlBaseGet();
        uiBufferEmpty = GetBufferEmptySize(pAudInBuf);

        if((pControlTable[ulPrimaryIndexTx].ulControl & UDMA_CHCTL_XFERMODE_M) == 0)
        {
            if(uiBufferEmpty < 2048)
            {   
                if(pAudInBuf->pucWritePtr < pAudInBuf->pucBufferStartPtr)
                {
                    pAudInBuf->pucWritePtr = pAudInBuf->pucBufferStartPtr;
                }
                
            }
            else
            {
                pusTxDestBuf += CB_TRANSFER_SZ;
                guiDMATransferCountTx += CB_TRANSFER_SZ;
                MIC++;
            }
            
            pucDMADest = (unsigned char *)pusTxDestBuf;
            pControlTable[ulPrimaryIndexTx].ulControl |= CTRL_WRD;
            pControlTable[ulPrimaryIndexTx].pvDstEndAddr = (void *)(pucDMADest + END_PTR);
            MAP_uDMAChannelEnable(UDMA_CH4_I2S_RX);
        }
        else
        {
            if((pControlTable[ulAltIndexTx].ulControl & UDMA_CHCTL_XFERMODE_M) == 0)
            {
               
                if(uiBufferEmpty < 2048)
                {
                    if(pAudInBuf->pucWritePtr < pAudInBuf->pucBufferStartPtr)
                    {
                        pAudInBuf->pucWritePtr = pAudInBuf->pucBufferStartPtr;
                    }
                }
                else
                {
                    pusTxDestBuf += CB_TRANSFER_SZ;
                    guiDMATransferCountTx += CB_TRANSFER_SZ;
                    MIC++;
                }
                pucDMADest = (unsigned char *)pusTxDestBuf;
                pControlTable[ulAltIndexTx].ulControl |= CTRL_WRD;
                pControlTable[ulAltIndexTx].pvDstEndAddr = (void *)(pucDMADest + END_PTR);
                MAP_uDMAChannelEnable(UDMA_CH4_I2S_RX);
            }
        }
        
        if(guiDMATransferCountTx >= CB_TRANSFER_SZ)
        {
              pAudInBuf->pucWritePtr += (CB_TRANSFER_SZ*2);
              if((unsigned int)pAudInBuf->pucWritePtr >= (unsigned int)pAudInBuf->pucBufferEndPtr)
              {
                  pAudInBuf->pucWritePtr = pAudInBuf->pucBufferStartPtr;
              }
              pusTxDestBuf = (unsigned short *)pAudInBuf->pucWritePtr;
              pusTxDestBuf -= CB_TRANSFER_SZ;
              guiDMATransferCountTx = 0;
              
        }   
        g_iReadFlag++;
    }
    
    
    if(uDMAIntStatus() & 0x00000020)
    {
        HWREG(0x4402609c) = (1<<11);
        pControlTable = MAP_uDMAControlBaseGet();
        iCount1++;
        if((pControlTable[ulPrimaryIndexRx].ulControl & UDMA_CHCTL_XFERMODE_M) == 0)
        {
            if((pAudOutBuf->pucReadPtr == pAudOutBuf->pucWritePtr) || (g_uiPlayWaterMark == 0))
            {
                pucDMASrc = &gaucZeroBuffer[0];
                if(pAudOutBuf->pucReadPtr == pAudOutBuf->pucWritePtr)
                {
                    g_uiPlayWaterMark = 0;
                    guiDMAEmptyCount++;
                    //DBG_PRINT("Buffer Empty %d\n\r",guiDMAEmptyCount );
                }
                guiDMATransferCountRx = 0;
                iCount3++;
            }
            else
            {
                pusRxSrcBuf += CB_TRANSFER_SZ;
                guiDMATransferCountRx += CB_TRANSFER_SZ;
                pucDMASrc = (unsigned char *)pusRxSrcBuf;
                SPEAKER++;
            }
            pControlTable[ulPrimaryIndexRx].ulControl |= CTRL_WRD;
            //pControlTable[ulPrimaryIndex].pvSrcEndAddr = (void *)((unsigned long)&gaucZeroBuffer[0] + 15);
            pControlTable[ulPrimaryIndexRx].pvSrcEndAddr = (void *)((unsigned long)pucDMASrc + END_PTR);
            //HWREG(DMA_BASE + UDMA_O_ENASET) = (1 << 5);
            MAP_uDMAChannelEnable(UDMA_CH5_I2S_TX);
        }
        else
        {
            if((pControlTable[ulAltIndexRx].ulControl & UDMA_CHCTL_XFERMODE_M) == 0)
            {
                if((pAudOutBuf->pucReadPtr == pAudOutBuf->pucWritePtr) || (g_uiPlayWaterMark == 0))
                {
                    pucDMASrc = &gaucZeroBuffer[0];
                    if(pAudOutBuf->pucReadPtr == pAudOutBuf->pucWritePtr)
                    {
                      g_uiPlayWaterMark = 0;
                      guiDMAEmptyCount++;
                      //DBG_PRINT("Buffer Empty %d\n\r",guiDMAEmptyCount );
                    }
                    guiDMATransferCountRx = 0;
                }
                else
                {
                    pusRxSrcBuf += CB_TRANSFER_SZ;
                    guiDMATransferCountRx += CB_TRANSFER_SZ;
                    pucDMASrc = (unsigned char *)pusRxSrcBuf;
                    SPEAKER++;
                }
                pControlTable[ulAltIndexRx].ulControl |= CTRL_WRD;
                pControlTable[ulAltIndexRx].pvSrcEndAddr = (void *)((unsigned long)pucDMASrc + END_PTR);
                //HWREG(DMA_BASE + UDMA_O_ENASET) = (1 << 5);
                MAP_uDMAChannelEnable(UDMA_CH5_I2S_TX);
            }
        }

        if(guiDMATransferCountRx >= CB_TRANSFER_SZ)
        {
              pAudOutBuf->pucReadPtr += (CB_TRANSFER_SZ*2);
              if((unsigned int)pAudOutBuf->pucReadPtr >= (unsigned int)pAudOutBuf->pucBufferEndPtr)
              {
                 pAudOutBuf->pucReadPtr = pAudOutBuf->pucBufferStartPtr;
              }
              
              pusRxSrcBuf = (unsigned short *)pAudOutBuf->pucReadPtr;
              pusRxSrcBuf -= CB_TRANSFER_SZ;
              guiDMATransferCountRx = 0;
        }
    }
    
}

//*****************************************************************************
//
//! configuring the DMA transfer
//!
//! \param psAudPlayerCtrl is the control structure managing the input data.
//!
//! This function
//!        1. setting the source and the destination for the DMA transfer.
//!		   2. setting the uDMA registers to control actual transfer.
//!
//! \return None.
//
//*****************************************************************************
void SetupPingPongDMATransferTx()
{
    puiTxSrcBuf = AudioCapturerGetDMADataPtr();
    pusTxDestBuf = (unsigned short*)GetWritePtr(pTxBuffer);

    SetupTransfer(UDMA_CH4_I2S_RX,
                  UDMA_MODE_PINGPONG,
                  CB_TRANSFER_SZ, 
                  UDMA_SIZE_16,
                  UDMA_ARB_8,
                  (void *)puiTxSrcBuf, 
                  UDMA_CHCTL_SRCINC_NONE,
                  (void *)pusTxDestBuf, 
                  UDMA_CHCTL_DSTINC_16);

    pusTxDestBuf += CB_TRANSFER_SZ;
    SetupTransfer(UDMA_CH4_I2S_RX|UDMA_ALT_SELECT,
                  UDMA_MODE_PINGPONG,
                  CB_TRANSFER_SZ, 
                  UDMA_SIZE_16,
                  UDMA_ARB_8,
                  (void *)puiTxSrcBuf, 
                  UDMA_CHCTL_SRCINC_NONE,
                  (void *)pusTxDestBuf, 
                  UDMA_CHCTL_DSTINC_16);
    
}

void SetupPingPongDMATransferRx()
{
    puiRxDestBuf = AudioRendererGetDMADataPtr();
    pusRxSrcBuf = (unsigned short *)GetReadPtr(pRxBuffer);

    SetupTransfer(UDMA_CH5_I2S_TX,
                  UDMA_MODE_PINGPONG,
                  CB_TRANSFER_SZ, 
                  UDMA_SIZE_16,
                  UDMA_ARB_8,
                  (void *)pusRxSrcBuf, 
                  UDMA_CHCTL_SRCINC_16,
                  (void *)puiRxDestBuf, 
                  UDMA_DST_INC_NONE);

    pusRxSrcBuf += CB_TRANSFER_SZ;
    guiDMATransferCountRx += CB_TRANSFER_SZ;

    SetupTransfer(UDMA_CH5_I2S_TX|UDMA_ALT_SELECT,
                  UDMA_MODE_PINGPONG,
                  CB_TRANSFER_SZ, 
                  UDMA_SIZE_16,
                  UDMA_ARB_8,
                  (void *)pusRxSrcBuf, 
                  UDMA_CHCTL_SRCINC_16,
                  (void *)puiRxDestBuf, 
                  UDMA_DST_INC_NONE);
    
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

