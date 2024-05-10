#include <stdint.h>
typedef unsigned int	UINT32;
typedef int	INT32;
typedef unsigned short UINT16;
typedef unsigned char  UINT8;
typedef uint64_t      UINT64, *PUINT64;


#ifndef _WINNT_
#ifndef WCHAR
typedef char                                    CHAR;
typedef unsigned short                          WCHAR;
#endif
#endif


#define EP_BLK_OUT_ADDR		0x02
#define EP_BLK_IN_ADDR		0x81
#define EP_INT_IN_ADDR		0x83

#define PRIMARY_DISPLAY	0
#define SECONDARY_DISPLAY 1

/*
 * Standard requests
 */
#define USB_REQ_GET_STATUS		    0x00
#define USB_REQ_CLEAR_FEATURE	    0x01
/* 0x02 is reserved */
#define USB_REQ_SET_FEATURE		    0x03
/* 0x04 is reserved */
#define USB_REQ_SET_ADDRESS		    0x05
#define USB_REQ_GET_DESCRIPTOR		0x06
#define USB_REQ_SET_DESCRIPTOR		0x07
#define USB_REQ_GET_CONFIGURATION	0x08
#define USB_REQ_SET_CONFIGURATION	0x09
#define USB_REQ_GET_INTERFACE		0x0A
#define USB_REQ_SET_INTERFACE		0x0B
#define USB_REQ_SYNCH_FRAME		    0x0C

//T6 Vendor requests
#define VENDOR_REQ_SPI_ROM_WRITE			0x01	//TBD
#define VENDOR_REQ_I2C_WRITE				0x02	//wValue: address wIndex: (set)	wLength: 
#define VENDOR_REQ_SET_MONITOR_CTRL			0x03	//wValue: 0/primary 1/secondary wIndex: 0/off 1/on
#define VENDOR_REQ_SET_CURSOR1_POS			0x04	//wValue: xPos, wIndex: yPos
#define VENDOR_REQ_SET_CURSOR1_STATE		0x05	//wValue: cursor index(0~9) wIndex: 0/off 1/on
#define VENDOR_REQ_SET_CURSOR2_POS			0x06	//wValue: xPos, wIndex: yPos
#define VENDOR_REQ_SET_CURSOR2_STATE		0x07	//wValue: cursor index(0~9) wIndex: 0/off 1/on
#define VENDOR_REQ_SET_RESOLUTION			0x08	//wValue: 0/primary, 1/secondary wIndex: index
#define VENDOR_REQ_SET_CURSOR_SHAPE			0x09	//wValue: cursor index(0~9) wIndex: display set, wLength: Header+shape data
#define VENDOR_REQ_SET_CURSOR1_SHAPE		0x10	//wValue: cursor index (0~9) wIndex: data offset, wLength: datal length
#define VENDOR_REQ_SET_CURSOR2_SHAPE		0x11	//wValue: cursor index (0~9) wIndex: data offset, wLength: datal length
#define VENDOR_REQ_SET_RESOLUTION_DETAIL_TIMING 0x12
#define VENDOR_REQ_WRITE_ROM_DATA				0x13
#define VENDOR_REQ_SET_CANCEL_BULK_OUT			0x14
#define VENDOR_REQ_SET_SOFTWARE_READY  0x31


#define VENDOR_REQ_GET_EDID					0x80	//wValue: offset wIndex: 0/primary, 1/secondary	wLength <=256
#define VENDOR_REQ_I2C_READ					0x81	//wValue: address windex: (set)
#define VENDOR_REQ_SPI_ROM_READ				0x82	//TBD
#define VENDOR_REQ_GET_MAC_ID				0x83	//wValue: 0, wIndex:0 wLength: 6
#define VENDOR_REQ_GET_RESOLUTION_TABLE_NUM	0x84	//wValue:0/primary,1/secondary	wIndex:0, wLength:4
#define VENDOR_REQ_GET_RESOLUTION_TABLE_DATA	0x85	//wValue:0/primary,1/secondary wIndex:0, wLength: Num*sizeof(solution_entry)
#define VENDOR_REQ_RESET_JPEG_ENGINE		0x86
#define VENDOR_REQ_QUERY_MONITOR_CONNECTION_STATUS	0x87//	View Index			0					1									0:disconnect;  1:connect.
#define VENDOR_REQ_QUERY_VIDEO_RAM_SIZE			0x88	//	0					0					1									Unit: MB
#define VENDOR_REQ_GET_RESOLUTION_TIMING_TABLE	0x89	//	View Index			StartIndex			Num*sizeof(timing)					Data
// ============================================================================
// Vendor Requests for Audio Function -- 0x20 ~ 0x27 (0xA0 ~ 0xA7)
//												bRequest	wValue				wIndex				wLength								Data							Description
//												----------- ------------------- ------------------- ----------------------------------- ------------------------------- --------------------------------
#define VENDOR_REQ_AUD_GET_NUM_ENGINES			0xA0	//	0x0000				0x0000				sizeof (UINT8)						UINT8 NumDmaEngines				Get number of DMA engines.
#define VENDOR_REQ_AUD_GET_ENGINE_CAPS			0xA1	//	0x0000				EngineIndex			sizeof (T6AUD_ENGINECAPS)			T6AUD_ENGINECAPS EngineCaps		Get DMA engine capabilities.
#define VENDOR_REQ_AUD_GET_NODE_CAPS			0xA2	//	NodeIndex			EngineIndex			sizeof (T6AUD_NODECAPS)				T6AUD_NODECAPS NodeCaps			Get node capabilities.
#define VENDOR_REQ_AUD_GET_NODE_VALUE			0xA3	//	NodeIndex			EngineIndex			sizeof (T6AUD_NODEVALUE)			T6AUD_NODEVALUE Value			Get node value.
#define VENDOR_REQ_AUD_SET_NODE_VALUE			0x23	//	NodeIndex			EngineIndex			sizeof (T6AUD_NODEVALUE)			T6AUD_NODEVALUE Value			Set node value.
#define VENDOR_REQ_AUD_GET_ENGINE_STATE			0xA4	//	0x0000				EngineIndex			sizeof (T6AUD_GETENGINESTATE)		T6AUD_GETENGINESTATE State		Get DMA engine state.
#define VENDOR_REQ_AUD_SET_ENGINE_STATE			0x24	//	0x0000				EngineIndex			sizeof (T6AUD_SETENGINESTATE)		T6AUD_SETENGINESTATE State		Set DMA engine state.
#define VENDOR_REQ_AUD_GET_FORMAT_LIST			0xA5	//	Start Index			EngineIndex			NumFmts * sizeof (T6AUD_FORMAT)		T6AUD_FORMAT array				Get format list of the DMA engine.

// ============================================================================
// Vendor Requests for Network Function -- 0x28 ~ 0x2F (0xA8 ~ 0xAF)
//													bRequest	wValue				wIndex				wLength								Data							Description
//													----------- ------------------- ------------------- ----------------------------------- ------------------------------- --------------------------------



// ============================================================================
// Vendor Requests for System -- 0x30 ~ 0x3F (0xB0 ~ 0xBF)
//												bRequest	wValue				wIndex				wLength								Data							Description
//												----------- ------------------- ------------------- ----------------------------------- ------------------------------- --------------------------------
#define VENDOR_REQ_RESET_HARDWARE				0x30	//	0x0000				ResetLevel			0x0000								None							Reset hardware.
#define VENDOR_REQ_SOFTWARE_READY				0x31	//	0x0000				Signature			0x0000								None							
//#define VENDOR_REQ_WRITE_ROM_DATA				0x32	//	Region				DataOffset			Length								Data							Region-0:boot data; 1:current image data; 2: user data
#define VENDOR_REQ_GET_VERSION					0xB0	//	0x0000				Type				sizeof (UINT32)						UINT32 Version					Type={0:HwPlatform; 1:boot_code; 2:image_code; 3:prjcode; 4:vendor cmd}
#define VENDOR_REQ_GET_FUNC_ID					0xB1	//	0x0000				Signature			sizeof (T6FUNCID)					T6FUNCID						See tT6FuncID
#define VENDOR_REQ_READ_ROM_DATA				0xB2	//	Region				DataOffset			Length								Data							Region-0:boot data; 1:current image data; 2: user data
#define VENDOR_REQ_QUERY_SECTION_DATA			0xB3	//	Signature			DataOffset			Length								Data							Query section data.

#define VENDOR_REQ_READ_ROM_DATA				0xB2	//	Region				DataOffset			Length								Data							Region-0:boot data; 1:current image data; 2: user data
#define VENDOR_REQ_QUERY_SECTION_DATA			0xB3	//	Signature			DataOffset			Length								Data							Query section data.


#define VENDOR_REQ_CLEAR_BLOCK                  0x50
#define VENDOR_REQ_CLEAR_ALL                    0x51

#define VENDOR_REQ_SET_SSID1                  0x52
#define VENDOR_REQ_SET_SSID2                  0x53
#define VENDOR_REQ_SET_PW1                    0x54
#define VENDOR_REQ_SET_PW2                    0x55
#define VENDOR_REQ_SET_TEST                   0x56
#define VENDOR_REQ_SET_MSG                    0x57
#define VENDOR_REQ_SET_GSSID1                 0x58
#define VENDOR_REQ_SET_GSSID2                 0x59
#define VENDOR_REQ_SET_GPW1                   0x5a
#define VENDOR_REQ_SET_GPW2                   0x5b
#define VENDOR_REQ_SET_CLR                    0x5c



typedef struct _USBD_DISPLAY_CLEAR_BLOCK {

  unsigned int Width;
  unsigned int Height;
  int x;
  int y;
 
}__attribute__((packed)) USBD_DISPLAY_CLEAR_BLOCK, *PUSBD_DISPLAY_CLEAR_BLOCK;


typedef struct _USBD_DISPLAY_RESOLUTION_INFO {
  unsigned char  Index;
  unsigned char  Reduced;
  unsigned short Width;
  unsigned short Height;
  unsigned short Frequency;
}__attribute__((packed)) USBD_DISPLAY_RESOLUTION_INFO, *PUSBD_DISPLAY_RESOLUTION_INFO;

#define VIDEO_SIGNATURE	0

typedef struct _BULK_CMD_HEADERR
{
	UINT32	Signature;
	UINT32	PayloadLength;
	UINT32	PayloadAddress;
	UINT32	PacketLength;
	UINT32	Reserved2;
	UINT32	Reserved3;
	UINT8	Padding[8];
}__attribute__((packed))BULK_CMD_HEADER, *PBULK_CMD_HEADER;


typedef enum 
{
	VIDEO_CMD_NO_OP = 0,
	VIDEO_CMD_CLIP_PRIMARY,
	VIDEO_CMD_CLIP_SECONDARY,
	VIDEO_CMD_FLIP_PRIMARY,
	VIDEO_CMD_FLIP_SECONDARY,
	VIDEO_CMD_BLT_SOURCETOMEM,
	VIDEO_CMD_BLT_MEMTOMEM,
	VIDEO_CMD_FB_DIRECT,
	VIDEO_CMD_PREVIEW,           //new
	VIDEO_CMD_ACTIVE,	//new
}VIDEO_COMMAND;

typedef enum
{
	VIDEO_COLOR_YUYV = 0,
	VIDEO_COLOR_YV12 = 4,
	VIDEO_COLOR_NV12 = 6,
	VIDEO_COLOR_RGB32 = 8,
	VIDEO_COLOR_RGB24 = 9,
	VIDEO_COLOR_YUV24 = 10,
	VIDEO_COLOR_YUV444 = 11,
	VIDEO_COLOR_JPEG	= 13,
}VIDEO_COLOR_FORMAT;

typedef enum
{
	FLAG_BIT_0 = 0x01,
	FLAG_BIT_1 = 0x02,
	FLAG_BIT_2 = 0x04,
	FLAG_BIT_3 = 0x08,
	FLAG_BIT_4 = 0x10,
	FLAG_BIT_5 = 0x20,
	FLAG_BIT_6 = 0x40,
	FLAG_RESET_JPEG = 0x80,
}VIDEO_CMD_FLAG;

typedef enum
{
	T6_CURSOR_MONO = 0,
	T6_CURSOR_ALPHA = 1,
	T6_CURSOR_MASK_COLOR = 2,
}T6_CURSOR_TYPE;

typedef struct _USBD_DISPLAY_CURSOR_SHAPE
{
	unsigned short        CursorType;     // 0 ¡Vmono,  1 ¡V alpha  2 ¡V mask color
	unsigned short        Width;
	unsigned short        Height;
	unsigned short        Pitch;
}USBD_DISPLAY_CURSOR_SHAPE;


typedef struct _VIDEO_COMMON_HEADER
{
	UINT8 Header[48];
}VIDEO_COMMON_HEADER, *PVIDEO_COMON_HEADER;

typedef struct _VIDEO_PACKET_HEADER
{
	BULK_CMD_HEADER BulkCmdHeader;
	VIDEO_COMMON_HEADER VideoCmdHeader;
}VIDEO_PACKET_HEADER, *PVIDEO_PACKET_HEADER;

typedef struct _VIDEO_FB_DIRECT_HEADER
{
	UINT32	Command;
	UINT32	PayloadSize;
	UINT32	FenceID;
	UINT32	TargetFormat;
	UINT16	Width;
	UINT16	Height;
	UINT16	Y_RGB_Pitch;
	UINT16	UV_Pitch;
	UINT32	Y_RGB_Data_FB_Offset;	//16 bytes alignment
	UINT32	U_UV_Data_Offset;		//16 bytes alignment
	UINT32	V_Data_Offset;			//16 bytes alignment
	UINT32	SourceFormat;
	UINT8	Padding[7];
	UINT8	Flag;
}__attribute__((packed))VIDEO_FB_DIRECT_HEADER, *PVIDEO_FB_DIRECT_HEADER;

typedef struct _VIDEO_CLIP_HEADER
{
	UINT32	Command;
	UINT32	PayloadSize;
	UINT32	FenceID;
	UINT32	TargetFormat;
	UINT16	StartX;
	UINT16	StartY;
	UINT16	Width;
	UINT16	Height;
	UINT16	Y_RGB_Pitch;
	UINT16	UV_Pitch;
	UINT32	Y_RGB_Data_FB_Offset;
	UINT32	U_UV_Data_Offset;
	UINT32	V_Data_Offset;
	UINT32	SourceFormat;
	UINT8	Padding[3];
	UINT8	Flag;
}__attribute__((packed))VIDEO_CLIP_HEADER, *PVIDEO_CLIP_HEADER;


typedef struct _VIDEO_FLIP_HEADER
{
	UINT32	Command;
	UINT32	PayloadSize;
	UINT32	FenceID;
	UINT32	TargetFormat;
	UINT16	Y_RGB_Pitch;
	UINT16	UV_Pitch;
	UINT32	Y_RGB_Data_FB_Offset;	//16bytes allignment
	UINT32	U_UV_Data_Offset;		//16 bytes allignment
	UINT32	V_Data_Offset;
	UINT32	SourceFormat;
	UINT8	Padding[11];
	UINT8	Flag;
}__attribute__((packed))VIDEO_FLIP_HEADER, *PVIDEO_FLIP_HEADER;

typedef struct _VIDEO_JWR_HEADER
{
	UINT32	Command;
	UINT32	PayloadSize;
	UINT32	FenceID;
	UINT32	TargetFormat;
	UINT16	Width;
	UINT16	Height;
	UINT16	X;
	UINT16	Y;
	UINT32	SourceFormat;
	UINT8	Padding[19];
	UINT8	Flag;
}__attribute__((packed))VIDEO_JWR_HEADER, *P_VIDEO_JWR_HEADER;


typedef struct _INT_IN_PACKET
{
	UINT32	FuncIdMask;
	UINT8	Cpu[8];
	UINT8	Video[8];
	UINT8	Mac[8];
	UINT8	Audio[8];
	UINT8	Serial[8];
	UINT8	Padding[20];
}__attribute__((packed))INT_IN_PACKET, *PINT_IN_PACKET;

 

typedef struct tT6RomDisplayCaps
{
    UINT32                      LinkInterfaces : 4;         // D00-D03  See T6DISP_INTF_XXX. Can be one or more.
    UINT32                      Rsvd_04_07 : 4;             // D04-D07  Reserved.
    UINT32                      NumResTbls : 8;             // D08-D15  Number of resolution tables.
    UINT32                      O_ResTbl : 16;              // D16-D31  Offset of the 1st resolution table.
}__attribute__((packed)) T6ROMDISPLAYCAPS, *PT6ROMDISPLAYCAPS;


typedef struct tT6RomDisplayInterface
{
    UINT32                      DAC_I2C : 2;                // D00-D01  DAC I2C Index.
    UINT32                      DAC_Rsvd : 6;               // D02-D07  Reserved.

    UINT32                      DVO_I2C : 2;                // D08-D09  DVO I2C Index.
    UINT32                      DVO_Rsvd : 2;               // D10-D11  Reserved.
    UINT32                      DVO_Transmitter : 4;        // D12-D15  See T6VIDEOTM_TYPE_XXX.

    UINT32                      DVI_I2C : 2;                // D16-D17  DVI I2C Index.
    UINT32                      DVI_Rsvd : 6;               // D18-D23  Reserved.

    UINT32                      LVDS_I2C : 2;               // D24-D25  LVDS I2C Index.
    UINT32                      LVDS_Rsvd : 6;              // D26-D31  Reserved.
}__attribute__((packed)) T6ROMDISPLAYINTERFACE, *PT6ROMDISPLAYINTERFACE;

typedef struct tT6RomDisplaySectionHeader
{
    UINT32                      Tag;                        // 0000h T6ROMDISPLAYTAG
    UINT32                      ValidSize;                  // 0004h Valid Size = 
                                                            //       sizeof(tT6RomDisplaySectionHeader)
                                                            //       if view1 enable: + sizeof(ResolutionTiming)*Display1Caps::NumResTbls
                                                            //       if view2 enable: + sizeof(ResolutionTiming)*Display2Caps::NumResTbls
    UINT8                       Rsvd_08_0B[4];              // 0008h Reserved.
    UINT16                      vid;                        // 000Ch Function VID
    UINT16                      pid;                        // 000Eh Function PID
    WCHAR                       ProductString[32];          // 0010h Function Product String
    UINT32                      Version;                    // 0050h
    UINT32                      DispFunc;                   // 0054h Display special function
    T6ROMDISPLAYCAPS            Display1Caps;               // 0058h Display 1 Caps
    T6ROMDISPLAYCAPS            Display2Caps;               // 005Ch Display 2 Caps
    T6ROMDISPLAYINTERFACE       DisplayInterface;           // 0060h Display Interface
    UINT8                       Rsvd_64_6F[12];             // 0064h-006Fh
}__attribute__((packed)) T6ROMDISPLAYSECTIONHDR, *PT6ROMDISPLAYSECTIONHDR;

typedef struct _ResolutionTiming
{
    UINT32                      PixelClock;                 // 0000h Pixel clock, in KHz.
    UINT8                       Frequency;                  // 0004h Refresh rate, in Hz.
    UINT8                       Reserved;                   // 0005h Reserved.
    UINT16                      HorTotalTime;               // 0006h Horizontal Total
    UINT16                      HorAddrTime;                // 0008h Horizontal Active
    UINT16                      HorSyncStart;               // 000Ah Sync Start = active + front porch
    UINT16                      HorSyncTime;                // 000Ch Sync
    UINT16                      VerTotalTime;               // 000Eh Vertical Total
    UINT16                      VerAddrTime;                // 0010h Vertical Active
    UINT16                      VerSyncStart;               // 0012h Sync Start = active + front porch
    UINT16                      VerSyncTime;                // 0014h Sync
    UINT16                      FNUM;                       // 0016h PLL P
    UINT16                      FDEN;                       // 0018h PLL Q
    UINT8                       IDIV;                       // 001Ah PLL N
    UINT8                       OutputSelect;               // 001Bh Output Select
    UINT8                       HorSyncPolarity;            // 001Ch 0:Active Low; 1:Active High.
    UINT8                       VerSyncPolarity;            // 001Dh 0:Active Low; 1:Active High.
    UINT8                       Reduced;                    // 001Eh 
    UINT8                       Flag;                       // 001Fh D0: 0:standard timing; 1:customer timing;
                                                            //       D1-D7: reserved.
} RESOLUTIONTIMING, *PRESOLUTIONTIMING;

typedef struct _CTRL_VENDOR_CMD
{
    UINT8    Request;
    UINT8    Rsvd;
    UINT16   Value;
    UINT16   Index;
    UINT16   Length;
    UINT16   Reserved[4];
}__attribute__((packed))CTRL_VENDOR_CMD,*PCTRL_VENDOR_CMD;






typedef enum _FUNC_ID_MASK
{
	FUNC_DUMMY		= 0x01,
	FUNC_CPU		= 0x02,
	FUNC_VIDEO		= 0x04,
	FUNC_USB_HOST	= 0x08,
	FUNC_MAC		= 0x10,
	FUNC_AUDIO		= 0x20,
	FUNC_SERIAL		= 0x40,
}FUNC_ID_MASK;

typedef enum _VIDEO_EVENT_MASK
{
	DISPLAY_1_CHANGE	= 0x1,
	DISPLAY_2_CHANGE	= 0x2,
	FENCE_ID			= 0x4,
	JPEG_ERROR			= 0x8,
}VIDEO_EVENT_MASK;


#define SIGNATURE_DISPLAY                       0x00
#define SIGNATURE_USB_HOST                      0x01
#define SIGNATURE_NETWORK                       0x02
#define SIGNATURE_AUDIO                         0x03
#define SIGNATURE_CAMERA                        0x04
#define SIGNATURE_ROM                           0x05
#define SIGNATURE_STORAGE                       0x06
#define SIGNATURE_NV12DMA                       0x07


#define USB_ENDPOINT_IN			0x80
#define USB_ENDPOINT_OUT		0x00

//LIBUSB DLL definition
#define USB_TYPE_STANDARD		(0x00 << 5)
#define USB_TYPE_CLASS			(0x01 << 5)
#define USB_TYPE_VENDOR			(0x02 << 5)
#define USB_TYPE_RESERVED		(0x03 << 5)

//LIBUSB kernel driver definition
#define LIBUSB_TYPE_STANDARD		0x00
#define LIBUSB_TYPE_CLASS			0x01
#define LIBUSB_TYPE_VENDOR			0x02
#define LIBUSB_TYPE_RESERVED		0x03

#define USB_RECIP_DEVICE		0x00
#define USB_RECIP_INTERFACE	0x01
#define USB_RECIP_ENDPOINT	0x02
#define USB_RECIP_OTHER			0x03


#define JPEG_PADDING_SIZE		1024



