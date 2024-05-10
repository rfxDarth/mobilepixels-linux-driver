#ifndef _T6USBDONGLE_H_
#define _T6USBDONGLE_H_

#include <libusb-1.0/libusb.h>
#include "t6.h"
#include "t6auddef.h"
#include "t6bulkdef.h"
#include "evdi_t6.h"

unsigned long GetTickCount();
void hex_dump(char *data, int size, char *caption);
void t6_save_file(char* p,int size);

int  t6_libusb_get_displaysectionheader(libusb_device_handle *t6usbdev);
int t6_libusb_get_jpegreset(libusb_device_handle *t6usbdev);
int t6_libusb_get_rotate(libusb_device_handle *t6usbdev);

int  t6_libusb_get_ram_size(PT6EVDI t6dev);

int  t6_libusb_set_monitor_power(PT6EVDI t6dev,char on);
void t6_libusb_donglereset(PT6EVDI t6dev);
int  t6_libusb_get_edid(PT6EVDI t6dev );
int  t6_libusb_set_AudioEngineStatus(PT6EVDI t6dev );
int   t6_libusb_set_softready(PT6EVDI t6dev);
int  t6_libusb_set_resolution(PT6EVDI t6dev, int w,int h);
int t6_libusb_Rgb24_full_block(PT6EVDI t6dev ,int fbaddr );

int  t6_libusb_FilpJpegFrame(PT6EVDI t6dev,char *jpgimage ,int jpgsize ,int flag);
int  t6_libusb_SendAudio(PT6EVDI t6dev,char * data , int len  );
int  t6_libusb_dongle_reset(PT6EVDI t6dev);
int t6_libusb_get_monitorstatus(PT6EVDI t6dev); // veiw = 0 hdmi , view =1 vga; 
int t6_libusb_test(libusb_device_handle* t6usbdev);
int t6_libusb_get_interrupt(PT6EVDI t6dev,char * data , int len );

int t6_libusb_get_touch(libusb_device_handle* t6usbdev);
int t6_libusb_set_touch(libusb_device_handle* t6usbdev,char on);


int t6_libusb_get_usagetime(libusb_device_handle* t6usbdev);
int t6_libusb_set_usagetime(libusb_device_handle* t6usbdev ,int min);

int t6_libusb_get_brightness(libusb_device_handle* t6usbdev);
int t6_libusb_set_brightness(libusb_device_handle* t6usbdev ,char bvalue);
	

int t6_libusb_get_version(libusb_device_handle* t6usbdev ,char index);
int t6_libusb_get_sn(libusb_device_handle* t6usbdev ,char* sn);

int t6_libusb_set_rotate(libusb_device_handle* t6usbdev ,char rotate);
int t6_libusb_set_custom(libusb_device_handle* t6usbdev );
int t6_write_rom_date(libusb_device_handle* t6usbdev , char *buf ,int len);
void t6_libusb_donglereset2(libusb_device_handle* t6usbdev);
int t6_libusb_get_interrupt2(libusb_device_handle* t6usbdev,char * data , int len );
int t6_libusb_set_monitor_power2(libusb_device_handle* t6usbdev,char on);
void ShowRomMsg(libusb_device_handle* t6usbdev);


#endif

