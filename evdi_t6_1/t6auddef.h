/*****************************************************************************
 * t6auddef.h
 *****************************************************************************
 * Copyright (c) MCT Corporation.  All rights reserved.
 * 
 * Module Name:
 *      t6auddef.h
 *
 * Abstract:
 *      Defines the T6 audio.
 * 
 * Environment:
 *      User / Kernel / Firmware mode
 *
 * Revision History:
 *      2016/04/11 Feng, Chun-Chiang (ccfeng@mct.com.tw)
 *          1) First Release.
 *      2016/06/15 Feng, Chun-Chiang (ccfeng@mct.com.tw)
 *          1) Added JackState (T6AUD_GETENGINESTATE).
 *      2016/09/19 CC & Ivan
 *          1) Added T6AUD_NODE_VOLUME_DB.
 *      2016/10/17 Feng, Chun-Chiang (ccfeng@mct.com.tw)
 *          1) Added T6AUD_ENGINEFLAG_JACK_STATE.
 *
 */

#ifndef _T6_AUD_DEF_H_
#define _T6_AUD_DEF_H_

// ================================================================================================
//
//  --------+       +-----------------------------------------------+       +---------------
//          |       |       Analog Playback Engine                  |       |               
//          |       |                                               |       |               
//          |       |       +-------+               +-------+       |       |               
//     DMA  |  ==>  |       | Mute  |               | Vol   |       |  ==>  |   I2S / DAC   
//          |       |       | Node  |               | Node  |       |       |               
//          |       |       +-------+               +-------+       |       |               
//          |       |                                               |       |               
//  --------+       +-----------------------------------------------+       +---------------
//
//  --------+       +-----------------------------------------------+       +---------------
//          |       |       Analog Capture Engine                   |       |               
//          |       |                                               |       |               
//          |       |       +-------+   +-------+   +-------+       |       |               
//     DMA  |  <==  |       |  Vol  |   | Mute  |   |  Mux  |       |  <==  |   I2S / ADC   
//          |       |       | Node  |   | Node  |   | Node  |       |       |               
//          |       |       +-------+   +-------+   +-------+       |       |               
//          |       |                                               |       |               
//  --------+       +-----------------------------------------------+       +---------------
//
// ================================================================================================

#pragma pack (1)

#define T6AUD_MAX_CHANNELS                      8

// ================================================================================================
// T6AUD_ENGINETYPE
//
typedef UINT8   T6AUD_ENGINETYPE;

#ifndef T6AUD_MAX_ENGINES
#define T6AUD_ENGINE_TYPE_RENDER                0x00
#define T6AUD_ENGINE_TYPE_CAPTURE               0x01
#define T6AUD_MAX_ENGINES                       0x02
#endif

// ================================================================================================
// T6AUD_ENGINEFLAG
//
typedef UINT8   T6AUD_ENGINEFLAG;
#define T6AUD_ENGINEFLAG_NONE                   0x00
#define T6AUD_ENGINEFLAG_JACK_STATE             0x01





// ================================================================================================
// T6AUD_NODETYPE
//
typedef UINT8   T6AUD_NODETYPE;
#define T6AUD_NODE_VOLUME                       0x00
#define T6AUD_NODE_MUTE                         0x01
#define T6AUD_NODE_MUX                          0x02
#define T6AUD_NODE_VOLUME_DB                    0x03 // Unit in dB that follow usb audio.
#define T6AUD_MAX_NODES                         0x04


// ================================================================================================
// T6AUD_MUX_TYPE
//
typedef UINT8   T6AUD_MUX_TYPE;
#define T6AUD_MUX_MIC_IN                        0x00
#define T6AUD_MUX_LINE_IN                       0x01


// ================================================================================================
// tT6AUD_EngineCaps
//
typedef struct tT6AUD_EngineCaps
{
    T6AUD_ENGINETYPE    Type;                   // see T6AUD_ENGINETYPE.
    T6AUD_ENGINEFLAG    Flags;                  // see T6AUD_ENGINEFLAG.
    UINT8               NumNodes;               // number of nodes.
    UINT8               Rsvd;                   // must be 0
    UINT16              NumFmts;                // number of available formats.
    UINT16              PreferredFmtIndex;      // index of preferred format.
    UINT32              MaxBandwidth;           // maximum bandwidth, in Bytes/Sec.
    UINT32              MaxBufferSize;          // maximum buffer size, in bytes.
} T6AUD_ENGINECAPS, *PT6AUD_ENGINECAPS;

// ================================================================================================
// tT6AUD_NodeCaps
//
typedef struct tT6AUD_NodeCaps
{
    T6AUD_NODETYPE      Type;                   // see T6AUD_NODETYPE.
    UINT8               Rsvd[3];                // must be 0s.
    UINT32              SteppingDelta;          // step value.
    union
    {
        struct
        {
            INT32       Minimum;                // minimum signed value.
            INT32       Maximum;                // maximum signed value.
        } Signed;
        struct
        {
            UINT32      Minimum;                // minimum value.
            UINT32      Maximum;                // maximum value.
        } Unsigned;
    };
} T6AUD_NODECAPS, *PT6AUD_NODECAPS;


// ================================================================================================
// tT6AUD_NodeValue
//
typedef struct tT6AUD_NodeValue
{
    UINT8               ChannelMask;            // bit channel mask.
    UINT8               Rsvd[7];                // must be 0s.
    union
    {
        INT32           SignedValues[8];        // signed values.
        UINT32          UnsignedValues[8];      // unsigned values
    };
} T6AUD_NODEVALUE, *PT6AUD_NODEVALUE;


// ================================================================================================
// tT6AUD_GetEngineState (for polling mode)
//
typedef struct tT6AUD_GetEngineState
{
    UINT8               Activity;               // 0:stop; 1:run.
    UINT8               JackState;              // jack state,
                                                //    for render engine,
                                                //      D0: speaker jack state. 0:disconnect;  1:connect.
                                                //      D1: DVO HDMI state. 0:disconnect;  1:connect.
                                                //      D2: LVDS HDMI state. 0:disconnect;  1:connect.
                                                //      D3-D7: reserved to 0.
                                                //    for capture engine,
                                                //      D0: microphone jack state. 0:disconnect;  1:connect.
                                                //      D1-D7: reserved to 0.
    UINT16              FormatIndex;            // index of format.
    UINT32              HwPos;                  // hardware position, in bytes.
    UINT32              FwPos;                  // firmware position, in bytes.
    UINT32              CyclicBufferSize;       // cyclic buffer size, in bytes.
} T6AUD_GETENGINESTATE, *PT6AUD_GETENGINESTATE;


// ================================================================================================
// tT6AUD_SetEngineState
//
typedef struct tT6AUD_SetEngineState
{
    UINT8               Activity;               // for capture,  0:stop; 1:run.
                                                // for renderer, ignore it. the state was changed by bulk cmd.
    UINT8               Rsvd;                   // rsvd.
    UINT16              FormatIndex;            // index of format.
    UINT32              Rsvd2;                  // rsvd.
    UINT32              ReturnSize;             // for capture,  it means bulk transfer size, in bytes, per block data.
                                                // for renderer, it means interrupt interval, in bytes, per assertion.
    UINT32              CyclicBufferSize;       // cyclic buffer size, in bytes.
} T6AUD_SETENGINESTATE, *PT6AUD_SETENGINESTATE;


// ================================================================================================
// tT6AUD_Format
//
typedef struct tT6AUD_Format
{
    UINT32                      SamplingRate;               // 0000h Sampling rate, in Hz.
    UINT16                      fs;                         // 0004h 128/256/...
    UINT8                       BitsPerSample;              // 0006h Number of bits per sample.
    UINT8                       Channels;                   // 0007h Number of channels.
    UINT32                      PLLClk;                     // 0008h PLL clock, in Hz. (800MHz ~ 1600MHz)
    UINT32                      MClk;                       // 000Ch MCLK
    UINT32                      BClk;                       // 0010h BCLK
    UINT32                      PLLSetting1;                // 0014h PLL Setting 1
    UINT32                      PLLSetting2;                // 0018h PLL Setting 2
    UINT8                       EngineMask;                 // 001Ch Engine mask.
    UINT8                       Rsvd_1D_1F[3];              // 001Dh Reserved.
} T6AUD_FORMAT, *PT6AUD_FORMAT;

#pragma pack ()

#endif

