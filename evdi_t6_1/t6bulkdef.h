/*****************************************************************************
 * t6bulkdef.h
 *****************************************************************************
 * Copyright (c) MCT Corporation.  All rights reserved.
 * 
 * Module Name:
 *      t6bulkdef.h
 *
 * Abstract:
 *      Defines the T6 bulk & interrupt transfer.
 * 
 * Environment:
 *      User / Kernel / Firmware mode
 *
 * Revision History:
 *      2016/04/11 Feng, Chun-Chiang (ccfeng@mct.com.tw)
 *          1) First Release.
 *      2016/04/20 Feng, Chun-Chiang (ccfeng@mct.com.tw)
 *          1) Added T6INT_ROM_EVENT_XXX for status monitoring.
 *      2016/06/15 Feng, Chun-Chiang (ccfeng@mct.com.tw)
 *          1) Added AudioRenderJack0 for audio render jack detection.
 *      2016/07/27 Feng, Chun-Chiang (ccfeng@mct.com.tw)
 *          1) Added SIGNATURE_IN, SIGNATURE_AUDIO_IN, SIGNATURE_ROM_IN.
 *      2016/09/13 CC & Ivan
 *          1) Added AudioRenderDVOH, AudioRenderLVDSH, AudioCaptureJackMic, AudioCaptureDataIn.
 *		2017/04/27	Yulw
 *			1) Add T6_DISPLAY_EXT_FLIP_XXX and T6_DISPLAY_EXT_CLIP_XXX extension IDs
 *			2) Modify structure tT6BulkDmaHdr.FuncSpecific.Display for new Extention IDs.
 *
 */

#ifndef _T6_BULK_DEF_H_
#define _T6_BULK_DEF_H_

#define SIGNATURE_DISPLAY                       0x00
#define SIGNATURE_USB_HOST                      0x01
#define SIGNATURE_NETWORK                       0x02
#define SIGNATURE_AUDIO                         0x03
#define SIGNATURE_CAMERA                        0x04
#define SIGNATURE_ROM                           0x05
#define SIGNATURE_STORAGE                       0x06
#define SIGNATURE_NV12DMA                       0x07

#define SIGNATURE_IN                            0x80
#define SIGNATURE_AUDIO_IN                      (SIGNATURE_IN | SIGNATURE_AUDIO)
#define SIGNATURE_ROM_IN                        (SIGNATURE_IN | SIGNATURE_ROM)


// ============================================================================
// Bulk-Out Transfer
//

// T6_PACKET_FLAG_XXX
#define T6_PACKET_FLAG_NONE                     0x00
#define T6_PACKET_FLAG_CONTINUETOSEND           0x01

// T6_ROM_FLAGS_XXX
#define T6_ROM_FLAGS_NONE                       0x00
#define T6_ROM_FLAGS_DONE_THEN_RESET            0x01    // after burned, reset hw
#define T6_ROM_FLAGS_INTERRUPT                  0x02    // interrupt with fence id.

// T6_ROM_VERB_XXX
#define T6_ROM_VERB_BURN_IMAGE2                 0x00    // 0x00: burn to image-2 region;
#define T6_ROM_VERB_BURN_IMAGE1                 0x01    // 0x01: burn to image-1 region;
#define T6_ROM_VERB_BURN_BOOT                   0x02    // 0x02: burn to boot region.

//T6_DISPLAY_EXT_XXX
#define T6_DISPLAY_EXT_NONE                     0x00
#define T6_DISPLAY_EXT_SET_CURSOR1_SHPAE        0x01    // parameter1: cursor index
#define T6_DISPLAY_EXT_SET_CURSOR2_SHPAE        0x02    // parameter1: cursor index
#define T6_DISPLAY_EXT_FLIP_PRIMARY				0x03	// 
#define T6_DISPLAY_EXT_FLIP_SECONDARY			0x04	//
#define T6_DISPLAY_EXT_CLIP_PRIMARY				0x05	//
#define T6_DISPLAY_EXT_CLIP_SECONDARY			0x06	//

#pragma pack(push, 1)

typedef struct tT6BulkDmaHdr
{
    UINT32                      Signature;
    UINT32                      PayloadLength;
    UINT32                      PayloadAddress;
    UINT32                      PacketSize;
    UINT32                      PacketOffset;
    UINT8                       PacketFlags;            // see T6_PACKET_FLAG_XXX.
    UINT8                       PacketRsvd[3];
    union
    {   // function specific (8 bytes)
        UINT64                  U64;
        UINT8                   U8[8];

        struct
        {
            UINT8               DisplayExtID;           // see T6_DISPLAY_EXT_XXXX
			union
			{
				struct
				{
					UINT8               Parameter1;             // depends on DisplayExtID
					UINT8               Rsvd[6];                // reserved.
				}ExtCursor;
				struct
				{
					UINT32		Y_StartAddresss;
					UINT8		Rsvd[3];
				}ExtFlipClip;
				UINT8			Rsvd[7];
			}ExtParams;
        } Display;

        struct
        {
            UINT8               Flags;                  // 0x00: 0x01:restart engine.
            UINT8               EngineIndex;            // index of engine.
            UINT8               Rsvd[6];                // reserved.
        } Audio;

        struct
        {
            UINT8               Flags;                  // see T6_ROM_FLAGS_XXX
            UINT8               Verb;                   // see T6_ROM_VERB_XXX
            UINT16              FenceId;                // fence id. D0-D7:seq_no D8-D15:signature
            UINT32              StartOffset;            // start offset from region
        } Rom;
    } FuncSpecific;
} T6BULKDMAHDR, *PT6BULKDMAHDR;

#pragma pack(pop)



// ============================================================================
// Interrupt-In Transfer
//

// T6INT_FUNC_MASK_XXX
#define T6INT_FUNC_MASK_CPU                     0x02
#define T6INT_FUNC_MASK_DISPLAY                 0x04
#define T6INT_FUNC_MASK_USB_HOST                0x08
#define T6INT_FUNC_MASK_NETWORK                 0x10
#define T6INT_FUNC_MASK_AUDIO                   0x20
#define T6INT_FUNC_MASK_SERIAL                  0x40
#define T6INT_FUNC_MASK_ROM                     0x80


// T6INT_DISP_EVENT_XXX
#define T6INT_DISP_EVENT_HDMI_CONNECT_STATUS    0x01    // Display 1 status change
#define T6INT_DISP_EVENT_VGA_CONNECT_STATUS     0x02    // Display 2 status change
#define T6INT_DISP_EVENT_FENCE_ID               0x04
#define T6INT_DISP_EVENT_JPEG_DECODER_ERROR     0x08

// T6INT_ROM_EVENT_XXX
#define T6INT_ROM_EVENT_FINISH                  0x00
#define T6INT_ROM_EVENT_CHECKSUM                0x01
#define T6INT_ROM_EVENT_ERASING                 0x02
#define T6INT_ROM_EVENT_WRITING                 0x03
#define T6INT_ROM_EVENT_VERIFYING               0x04
#define T6INT_ROM_EVENT_TRANSFERING             0x11    // software used.
#define T6INT_ROM_EVENT_ERROR                   0x80
#define T6INT_ROM_EVENT_ERR_CHECKSUM            (T6INT_ROM_EVENT_ERROR | T6INT_ROM_EVENT_CHECKSUM)
#define T6INT_ROM_EVENT_ERR_ERASE               (T6INT_ROM_EVENT_ERROR | T6INT_ROM_EVENT_ERASING)
#define T6INT_ROM_EVENT_ERR_WRITE               (T6INT_ROM_EVENT_ERROR | T6INT_ROM_EVENT_WRITING)
#define T6INT_ROM_EVENT_ERR_VERIFY              (T6INT_ROM_EVENT_ERROR | T6INT_ROM_EVENT_VERIFYING)
#define T6INT_ROM_EVENT_ERR_TRANSFER            (T6INT_ROM_EVENT_ERROR | T6INT_ROM_EVENT_TRANSFERING)   // software used.

#pragma pack(push, 1)

typedef struct tT6InterruptData
{
    UINT8                       FuncMask;               // 00h Function Mask. see T6INT_FUNC_MASK_XXX.
    UINT8                       Rsvd_01_03[3];          // 01h Reserved.

    // cpu
    UINT8                       CPUData[8];             // 04h cpu data

    // display
    UINT32                      DisplayData;            // 0Ch display data.
    UINT8                       DisplayRsvd[3];         // 10h display reserved.
    UINT8                       DisplayEvent;           // 13h see T6INT_DISP_EVENT_XXX.

    // network
    UINT8                       NetworkData[8];         // 14h network data.

    // audio
    UINT8                       AudioRsvd[4];           // 1Ch audio reserved.
    union
    {
        struct
        {
            UINT32              AudioRenderDmaPtr : 24; // audio render DMA pointer.
            UINT32              AudioRenderJack0 : 1;   // audio render jack-0 state. 0:unplug;  1:plug.
            UINT32              AudioRenderDVOH : 1;    // audio render DVO HDMI state. 0:DVI;  1:HDMI.
            UINT32              AudioRenderLVDSH : 1;   // audio render LVDS HDMI state. 0:DVI;  1:HDMI.
            UINT32              AudioRsvd : 3;          // audio reserved.
            UINT32              AudioCaptureJackMic : 1;// audio capture mic jack state. 0:unplug;  1:plug.
            UINT32              AudioCaptureDataIn : 1; // audio capture data in.
        };
        UINT32                  AudioRenderValue;
    } AudioRender;                                      // 20h audio render.

    // serial
    UINT8                       SerialData[8];          // 24h serial data?

    // rom
    UINT8                       RomEvent;               // 2Ch rom event. see T6INT_ROM_EVENT_XXX.
    UINT8                       RomRsvd;                // 2Dh rom reserved.
    UINT16                      RomFenceId;             // 2Eh rom fence id.
    UINT32                      RomProceedSize;         // 30h rom proceed size.

    // others
    UINT8                       Rsvd_34_3F[12];         // 34h reserved.
} T6INTERRUPTDATA, *PT6INTERRUPTDATA;

#pragma pack(pop)

#endif

