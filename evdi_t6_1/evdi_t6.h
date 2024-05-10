#ifndef EVDI_T6_H
#define EVDI_T6_H
#include <stdint.h>
#include <libusb-1.0/libusb.h>
#include <evdi_lib.h>
#include "queue.h"

#define MAX_DIRTS 16
#define MAX_T6_DEVICES	16

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)    /* Don't do anything in release builds */
#endif
#include "simclist.h"


typedef unsigned int   UINT32;
typedef unsigned short UINT16;
typedef unsigned char  UINT8;
#pragma pack(1)



typedef struct T6evdi{
	libusb_device_handle*	t6usbdev;
	evdi_handle             ev_handle; 
	int                     display_id; 
	int                     usb_bus_id;
	int                     usb_dev_id;
	int                     image_work_process;
	int                     jpg_work_process;
	int                     audio_work_process;
	int                     event_process;
	int                     usb_process;
	int                     ramsize;
	int                     disp_set_mode;
	int                     fbAddr ;
	int                     cmdAddr;  
	int                     frameupdate ;
	int                     audio_only ;
	int                     interface_num;
	int                     jpg_reset_fun;
	int                     jpg_rotate ;
	UINT16      			Width;
	UINT16					Height;
	UINT8					disp_interface;
	UINT8					edid[512];
	UINT8                   *video_buffer;
	//queue_t*                audio_queue;
	//queue_t*                jpg_queue;
	list_t                  jpg_list_queue;
	struct evdi_box*        evdi_list_queue;
	pthread_mutex_t         *lock;
	pthread_mutex_t         image_mutex;
	pthread_mutex_t         bulkusb_mutex;
	struct T6evdi *next;
}T6EVDI, *PT6EVDI;


struct jpg_packet{
  unsigned long jpgImageSize;
  char *buffer ; 
};


struct evdi_box{
  char box[128];
};

#pragma pack()

#endif



