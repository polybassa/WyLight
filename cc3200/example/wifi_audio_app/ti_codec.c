//*****************************************************************************
// ti_codec.c
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
//! \addtogroup wifi_audio_app
//! @{
//
//*****************************************************************************
#ifdef TI_CODEC
#include <stdint.h>
#include <stdbool.h>
#include "hw_types.h"
#include "i2c.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_i2c.h"
#include "rom.h"
#include "rom_map.h"
#include "stdlib.h"
#include "prcm.h"
#include "string.h"
#include "hw_common_reg.h"
#include "utils.h"
#include "pin.h"
#include "ti_codec.h"
#include "i2c_if.h"
#include "network.h"
//*****************************************************************************
//                          LOCAL DEFINES
//*****************************************************************************
#define CRYSTAL_FREQ               40000000
#define SYS_CLK                    CRYSTAL_FREQ
#define CODEC_I2C_SLAVE_ADDR      ((0x30 >> 1)) 
#define CLOCK_512KHZ               1
#define DRC                        1
//******************************************************************************
//            GLOBAL VARIABLES
//******************************************************************************
unsigned char val[2],I,J=0;

//******************************************************************************
//
// Selects the Page for reading/writing
//
//******************************************************************************
unsigned long CodecPageSelect(unsigned long ulPageAddress)
{ 
    return CodecRegWrite(PAGE_CTRL_REG,ulPageAddress);
}

//******************************************************************************
//
// Writes to specfied register
// ulRegAddr - Register Address
// ucRegValue - 8 bit Register Value
//
//******************************************************************************
unsigned long CodecRegWrite(unsigned long ulRegAddr,unsigned char ucRegValue)
{
    unsigned char ucData[2]; 
    ucData[0] = (unsigned char)ulRegAddr;
    ucData[1] = ucRegValue;
    val[1]=ucData[1];
#if 0
    if(I2C_IF_ReadFrom(0x44,&ucRegAddr, 1, 
                   &ucRegData[0], 2) != 0)
    return -1;
#endif   
    J=I2C_IF_Write(CODEC_I2C_SLAVE_ADDR, ucData, 2, 1);
    if(J !=0)
        return (1U<<31);
        MAP_UtilsDelay(27000);

    J=I2C_IF_ReadFrom(CODEC_I2C_SLAVE_ADDR, &ucData[0], 1,&val[0],1);
    if(J !=0)
        return (1U<<31);

    if(val[0] != val[1])
        ++I;

    return 0;
}

//******************************************************************************
//
// Codec Soft Reset
//
//******************************************************************************
void CodecReset()
{
    //
    // Select page 0
    //
    CodecPageSelect(0);

    //
    // Soft RESET
    //
    CodecRegWrite(CODEC_REG_SOFT_RESET,1);
}

//******************************************************************************
//
// ucVolume :- 
//              0 -> 0 bB (Highest) to 117 -> -75 dB (lowest)
//
//******************************************************************************
void AudioVolumeControl(signed char ucVolume)
{
    //
    // Select page 0
    //
    CodecPageSelect(1);

    //
    // Enable HPL output analog vloume
    //
    #ifdef DEFAULT
    CodecRegWrite(CODEC_REG_HPL_VOL,0x80 | (117 - ucVolume));
    #else
    //CodecRegWrite(0x16,0x80 | (117 - ucVolume));
    CodecRegWrite(0x16,0x25);
    #endif
    //
    // Enable HPR output analog vloume
    //
    #ifdef DEFAULT
    CodecRegWrite(CODEC_REG_HPR_VOL,0x80 | (117 - ucVolume) );
    #else
    //CodecRegWrite(0x17,0x80 | (117 - ucVolume));
     CodecRegWrite(0x17,0x25);
    #endif
}


//******************************************************************************
//
// Codec Mute
//
//******************************************************************************
void CodecMute()
{
    //
    // Select page 0
    //
    CodecPageSelect(0);

    //
    // Mute
    //
    CodecRegWrite(CODEC_REG_DAC_VOL,0x0C);
}

//******************************************************************************
//
// Codec UnMute
//
//******************************************************************************
void CodecUnmute()
{
    //
    // Select page 0
    //
    CodecPageSelect(0);

    //
    // Unmute
    //
    CodecRegWrite(CODEC_REG_DAC_VOL,0x00);
}

#endif
//******************************************************************************
//
// Codec Configure
//
//******************************************************************************
void ConfigureAudioCodec(unsigned long ulWordLength)
{
 
    CodecReset();
    CodecPageSelect(0);

    // Set I2S Mode and Word Length
    CodecRegWrite(0x1b,0x00); //0x00 16bit
    // Clock settings
#if CLOCK_512KHZ      

    CodecRegWrite(0x04,0x03);
    CodecRegWrite(0x05,0x94);
    CodecRegWrite(0x06,0x2A);
    CodecRegWrite(0x07,0x00);  
        
    CodecRegWrite(0x0B,0x8E);
    CodecRegWrite(0x0C,0x81);
    CodecRegWrite(0x0D,0x01);
    CodecRegWrite(0x0E,0x80); 
    
    CodecRegWrite(0x12,0x95);    /* NADC is powered up and set to 1 */
    CodecRegWrite(0x13,0x82);      /* MADC is powered up and set to 1 */
    CodecRegWrite(0x14,0x80);    /* AOSR set t0 128 */ //0x30
    
#else
       //clock config 768Khz
    CodecRegWrite(0x04,0x03);
    CodecRegWrite(0x05,0x94);
    CodecRegWrite(0x06,0x20);
    CodecRegWrite(0x0B,0x82);
    CodecRegWrite(0x0C,0x88);
    CodecRegWrite(0x0D,0x01);
    CodecRegWrite(0x0E,0x80); //0x30
            
    CodecRegWrite(0x12,0x82);    /* NADC is powered up and set to 1 */
    CodecRegWrite(0x13,0x98);      /* MADC is powered up and set to 1 */
    CodecRegWrite(0x14,0x80);    /* AOSR set t0 128 */ //0x30
#endif
        
#ifdef SPEAKER
    // Volume ctrl
    CodecRegWrite(0x41,0x30);    //0x30
    CodecRegWrite(0x42,0x30);   //0x30
    // ##Configure Processing Blocks
    CodecRegWrite(0x3C,0x2);    //PRB_P2 selected
    CodecRegWrite(0x3D,0x2);
    // CodecRegWrite(0x1D,0x08);
    CodecPageSelect(0x2C);  //Select Page 44, Enable Adaptive filtering for DAC  
    CodecRegWrite(0x01,0x04);   //Enable Adaptive filtering for DAC
    // Select Page 8
    CodecPageSelect(8);         //Enable Adaptive filtering for ADC
    CodecRegWrite(0x01,0x04);   //Enable Adaptive filtering for ADC
        
#endif
    // ##Configure Power Supplies
    CodecPageSelect(1);
    CodecRegWrite(0x01,0x08);
    CodecRegWrite(0x02,0x01);
    CodecRegWrite(0x47,0x32);
    CodecRegWrite(0x7B,0x01);     

#ifdef MIC
    //##Configure ADC Channel
    //Select Page 1
    CodecPageSelect(1);

    CodecRegWrite(0x33,0x40);
#ifdef LINE_IN    
    //Route IN2L not routed
    CodecRegWrite(0x34,0x40); 

    //Route IN2R CM1L to LEFT_N with 10K input impedance
    CodecRegWrite(0x36,0x40); // 0x50 0x10 0x11

    //Route IN2R to RIGHT_P with 10K input impedance
    CodecRegWrite(0x37,0x40); // 0x50 0x10 0x11

    //Route CM1R to RIGHT_M with 10K input impedance
    CodecRegWrite(0x39,0x40); 

    CodecRegWrite(0x3a,0xC0);//Floating IN1L        
#elif MONO_MIC
    //Route IN2L not routed
    CodecRegWrite(0x34,0x00); 

    //Route IN2R CM1L to LEFT_N with 10K input impedance
    CodecRegWrite(0x36,0x00); // 0x50 0x10 0x11

    //Route IN2R to RIGHT_P with 10K input impedance
    CodecRegWrite(0x37,0x10); // 0x50 0x10 0x11

    //Route CM1R to RIGHT_M with 10K input impedance
    CodecRegWrite(0x39,0x40); 

    CodecRegWrite(0x3a,0x10);//Floating IN1L   
#elif ONBOARD_MIC     
    //Route IN2L not routed
    CodecRegWrite(0x34,0x00); 

    //Route IN2R CM1L to LEFT_N with 10K input impedance
    CodecRegWrite(0x36,0x00); // 0x50 0x10 0x11

    //Route IN2R to RIGHT_P with 10K input impedance
    CodecRegWrite(0x37,0x04); // 0x50 0x10 0x11

    //Route CM1R to RIGHT_M with 10K input impedance
    CodecRegWrite(0x39,0x40); 
    CodecRegWrite(0x3a,0x04);//Floating IN1L       
#else
    //Route IN2L not routed
    CodecRegWrite(0x34,0x40); 

    //Route IN2R CM1L to LEFT_N with 10K input impedance
    CodecRegWrite(0x36,0x54); // 0x40 is the original value

    //Route IN2R to RIGHT_P with 10K input impedance
    //CodecRegWrite(0x37,0x40); // 0x50 0x10 0x11
    CodecRegWrite(0x37,0x54); 

    //Route CM1R to RIGHT_M with 10K input impedance
    CodecRegWrite(0x39,0x40); 

    //CodecRegWrite(0x3a,0xC0);//Floating IN1L       
    CodecRegWrite(0x3a,0xD4);//Floating IN1L  
#endif
    //Unmute Left MICPGA, Gain selection of 6dB to
    //make channel gain 0dB, since 20K input
    //impedance is used single ended
    CodecRegWrite(0x3b,0x00); // 0x0c

    //Unmute Right MICPGA, Gain selection of 6dB to
    //make channel gain 0dB, since 20K input
    //impedance is used single ended
    CodecRegWrite(0x3c,0x00); //0x0c


    //Select Page 0
    CodecPageSelect(0);
#ifdef AGC        
    //AGC
    //    AGC Hysteresis = 4dB, Noise Threshold = -58 dB       
    CodecRegWrite(0x57,0x80);  //0x9E
    //    AGC Max Gain = 23 dB
    CodecRegWrite(0x58,0x28);  //0x2E
    //     Attack = 15.2 ms
    CodecRegWrite(0x59,0x50);
    //    Decay = 11.61 ms (default)
    CodecRegWrite(0x5A,0x00);
    //    Noise debounce = 185.76 ms
    CodecRegWrite(0x5B,0x0C);
    //     Signal debounce = 0.363 ms
    CodecRegWrite(0x5C,0x03);
    //     Powerup LAGC, Target = -14dB, Gain Hysteresis = +/- 1
    CodecRegWrite(0x5D,0x20);  //0x28
    CodecRegWrite(0x56,0xC2);

    //    AGC Max Gain = 23 dB
    CodecRegWrite(0x60,0x28);  //0x2E
    //    AGC Max Gain = 23 dB
    CodecRegWrite(0x61,0x50);

    CodecRegWrite(0x63,0x03);

    CodecRegWrite(0x65,0x20); //0x28

    CodecRegWrite(0x5E,0xC2);

#endif       

    //Power up LADC/RADC
    CodecRegWrite(0x51,0xC0);  //0xC0

    //Unmute LADC/RADC
    CodecRegWrite(0x52,0x00);
        
#endif    

#ifdef SPEAKER       
    // ##Configure DAC Channel
    //Select Page 1
    CodecPageSelect(1);

    //De-pop: 5 time constants, 6k resistance
    CodecRegWrite(0x14,0x25);

    //Route LDAC/RDAC to HPL/HPR
    CodecRegWrite(0x0C ,0x08);
    CodecRegWrite(0x0D ,0x08);

    //Route LDAC/RDAC to LOL/LOR
    CodecRegWrite(0x0E, 0x08);
    CodecRegWrite(0x0F, 0x08);


    //Power up HPL/HPR and LOL/LOR drivers
    CodecRegWrite(0x09 ,0x3C);

    //Unmute HPL/HPR driver, 0dB Gain
    CodecRegWrite(0x10 ,0x00);
    CodecRegWrite(0x11 ,0x00);

    //Unmute LOL/LOR driver, 0dB Gain
    CodecRegWrite(0x12,0x0E);
    CodecRegWrite(0x13,0x0E);

    //Select Page 0
    CodecPageSelect(0);

    //DAC => 0dB
    CodecRegWrite(0x41, 0x00);  //0x00
    CodecRegWrite(0x42, 0x00);

    //DRC 
#ifdef DRC
    CodecRegWrite(0x44, 0x7F);   
    CodecRegWrite(0x45, 0x00);
    CodecRegWrite(0x46, 0xB6);
    CodecPageSelect(0x2E);
    CodecRegWrite(0x34,0x7F);
    CodecRegWrite(0x35,0xAB);
    CodecRegWrite(0x36,0x00);
    CodecRegWrite(0x37,0x00);
    CodecRegWrite(0x38,0x80);
    CodecRegWrite(0x39,0x55);
    CodecRegWrite(0x3A,0x00);
    CodecRegWrite(0x3B,0x00);
    CodecRegWrite(0x3C,0x7F);
    CodecRegWrite(0x3D,0x56);
    CodecRegWrite(0x3E,0x00);
    CodecRegWrite(0x3F,0x00);        
    CodecRegWrite(0x40,0x00);
    CodecRegWrite(0x41,0x11);
    CodecRegWrite(0x42,0x00);
    CodecRegWrite(0x43,0x00);
    CodecRegWrite(0x44,0x00);
    CodecRegWrite(0x45,0x11);
    CodecRegWrite(0x46,0x00);
    CodecRegWrite(0x47,0x00);
    CodecRegWrite(0x48,0x7F);
    CodecRegWrite(0x49,0xDE);
    CodecRegWrite(0x4A,0x00);
    CodecRegWrite(0x4B,0x00);
#endif  //DRC        

    //Select Page 0
    CodecPageSelect(0);

    //Power up LDAC/RDAC
    CodecRegWrite(0x3f, 0xd6);

    //Unmute LDAC/RDAC
    CodecRegWrite(0x40,0x00);
#endif        
}
//******************************************************************************
//
// TICodecMIC Configure
//
//******************************************************************************
void ConfigureTiCodecMic(unsigned long ulWordLength)
{
    CodecReset();
        CodecPageSelect(0);
        // Set I2S Mode and Word Length
    CodecRegWrite(0x1b,ulWordLength);
    CodecRegWrite(0x12,0x84);    /* NADC is powered up and set to 1 */
    CodecRegWrite(0x13,0x84);     /* MADC is powered up and set to 1 */
    CodecRegWrite(0x14,0x30);    /* AOSR set t0 128 */
    //CodecRegWrite(0x1C,0x01);
    CodecPageSelect(1);        /* Select page 1 */
    CodecRegWrite(0x2e,0x09);    /* MIC BIAS = AVDD */
    CodecRegWrite(0x2f,0x00);
    CodecRegWrite(0x30,0x20);    /* MICPGA P = MIC 40k */
    CodecRegWrite(0x31,0x40);    /* MICPGA M - CM 10k */\
    CodecPageSelect(0);        /* Select page 0 */
    CodecRegWrite(0x40,0x0C);
    //CodecRegWrite(0x75,0x24);
    CodecRegWrite(0x51,0x80);    /* POWERUP ADC channel */
    CodecRegWrite(0x52,0x00);    /* UNMUTE ADC channel */
    CodecRegWrite(0x53,0x18);
    //return(0);     
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
