//*****************************************************************************
// aes_vector.h
//
// Test Vector data for AES Module
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

#ifndef __AES_VECTOR_H__
#define __AES_VECTOR_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Test Vector Structure
//
//*****************************************************************************

unsigned int pui32AES128Key[4] = {
    0x16157e2b, 0xa6d2ae28, 0x8815f7ab, 0x3c4fcf09
};

unsigned int pui32AES192Key[6] = {
    0xf7b0738e, 0x52640eda, 0x2bf310c8, 0xe5799080,
    0xd2eaf862, 0x7b6b2c52
};

unsigned int pui32AES256Key[8] = {
    0x10eb3d60, 0xbe71ca15, 0xf0ae732b, 0x81777d85,
    0x072c351f, 0xd708613b, 0xa310982d, 0xf4df1409
};

unsigned int pui32AESPlainText[16] = {
    0xe2bec16b, 0x969f402e, 0x117e3de9, 0x2a179373,
    0x578a2dae, 0x9cac031e, 0xac6fb79e, 0x518eaf45,
    0x461cc830, 0x11e45ca3, 0x19c1fbe5, 0xef520a1a,
    0x45249ff6, 0x179b4fdf, 0x7b412bad, 0x10376ce6
};

unsigned int pui32AESCipherText[16] = {
         0xb47bd73a, 0x60367a0d, 0xf3ca9ea8, 0x97ef6624,
         0x85d5d3f5, 0x9d69b903, 0x5a8985e7, 0xafbafd96,
         0x7fcdb143, 0x23ce8e59, 0xe3001b88, 0x880603ed,
         0x5e780c7b, 0x3fade827, 0x71202382, 0xd45d7204
};

typedef struct AESTestVectorStruct
{

    unsigned int ui32KeySize;
    unsigned int *pui32Key1;
    unsigned int *pui32Key2;
    unsigned int *pui32Key3;
    unsigned int pui32IV[4];
    unsigned int pui32Tag[4];
    unsigned int ui32DataLength;
    unsigned int *pui32PlainText;
    unsigned int ui32AuthDataLength;
    unsigned int *pui32AuthData;
    unsigned int pui32CipherText[16];
} sAESTestVector;


sAESTestVector psAESCBCTestVectors =
{

        AES_CFG_KEY_SIZE_128BIT,
        pui32AES128Key,
        0,
        0,
        {0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c},
        {0x00000000, 0x00000000, 0x00000000, 0x00000000},
        64,
        pui32AESPlainText,
        0,
        0,
        {0xacab4976, 0x46b21981, 0x9b8ee9ce, 0x7d19e912,
         0x9bcb8650, 0xee197250, 0x3a11db95, 0xb2787691,
         0xb8d6be73, 0x3b74c1e3, 0x9ee61671, 0x16952222,
         0xa1caf13f, 0x09ac1f68, 0x30ca0e12, 0xa7e18675}

};
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif

