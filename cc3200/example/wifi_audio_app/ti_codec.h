//*****************************************************************************
// ti_codec.h
//
// Driver for TI TLV320AIC3110 CODEC
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
// ti_codec.h - Macros and Prototypes for TI TLV320AIC3110 CODEC
//
//*****************************************************************************
#ifndef __TI_CODEC_H__
#define __TI_CODEC_H__
#ifdef TI_CODEC

#define CODEC_I2C_SLAVE_ADDR      ((0x30 >> 1)) 
#define PAGE_CTRL_REG           0x00

//******************************************************************************
//
// Supported Wordlength
//
//******************************************************************************
#define CODEC_I2S_WORD_LEN_16           0x00
#define CODEC_I2S_WORD_LEN_20           0x10
#define CODEC_I2S_WORD_LEN_24           0x20
#define CODEC_I2S_WORD_LEN_32           0x30

//******************************************************************************
//
// Codec Page 0 Registers
//
//******************************************************************************

#define CODEC_REG_SOFT_RESET            0x01
#define CODEC_REG_CLK_MUX               0x04
#define CODEC_REG_PLL                   0x05
#define CODEC_REG_IF_CTRL               0x1B
#define CODEC_REG_NDAC                  0x0B
#define CODEC_REG_MDAC                  0x0C
#define CODEC_REG_DOSR_MSB              0x0D
#define CODEC_REG_DOSR_LSB              0x0E
#define CODEC_REG_DAC_PRB               0x3C
#define CODEC_REG_DAC_VOL               0x40
#define CODEC_REG_DAC_LEFT_VOL          0x41
#define CODEC_REG_DAC_RIGHT_VOL         0x42

//******************************************************************************
//
// Codec Page 1 Registers
//
//******************************************************************************

#define CODEC_REG_HP                    0x1F
#define CODEC_REG_OUTPUT_MIXER          0x23
#define CODEC_REG_HPL_DRIVER            0x28
#define CODEC_REG_HPR_DRIVER            0x29
#define CODEC_REG_SPL_DRV               0x2A
#define CODEC_REG_SPR_DRV               0x2B
#define CODEC_REG_CLASS_D_AMP           0x20
#define CODEC_REG_HPL_VOL               0x24
#define CODEC_REG_HPR_VOL               0x25
#define CODEC_REG_SPL_VOL               0x26
#define CODEC_REG_SPR_VOL               0x27
#define CODEC_REG_DAC_DATA_PATH         0x3F

//******************************************************************************
//
// Codec Page 8 Registers
//
//******************************************************************************
#define CODEC_REG_DAC_COFF_BUF          0x01

//******************************************************************************
//
// Prototypes for the APIs.
//
//******************************************************************************

extern unsigned long CodecRegWrite(unsigned long ulRegAddr,unsigned char ucRegValue);
extern unsigned long CodecPageSelect(unsigned long ulPageAddress);
extern void CodecReset();
extern void AudioVolumeControl(signed char ucVolume);
extern void CodecMute();
extern void CodecUnmute();
extern void ConfigureAudioCodec(unsigned long ulWordLength);
extern unsigned long ConfigureAudioCodecMic(unsigned long ulWordLength);

#endif
#endif //__TI_CODEC_H__
